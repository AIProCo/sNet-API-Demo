/*==============================================================================
* Copyright 2024 AIPro Inc.
* Author: Chun-Su Park (cspk@skku.edu)
=============================================================================*/
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>

// core
#include "global.h"
#include "generator.h"

// util
#include "json.hpp"
#include "util.h"

#define CFG_FILEPATH "inputs/config.json"
#define SR_X2_FILEPATH "inputs/aipro_sr_x2_1_7.net"
#define SR_X1_5_FILEPATH "inputs/aipro_sr_x1_5_1_7.net"
#define NV_FHD_FILEPATH "inputs/aipro_nv_1088_1920_1_7.net"
#define NV_HD_FILEPATH "inputs/aipro_nv_736_1280_1_7.net"

using namespace std;
using namespace cv;
using json = nlohmann::json;

bool parseConfigAPI(Config &cfg, VideoCW &videoCW, int argc, char *argv[]);

int main(int argc, char *argv[]) {
    Config cfg;
    VideoCW videoCW;

    // cout << argv[0] << ", " << argv[1] << ", " << argv[2] << endl;
    if (argc != 1 && argc != 3) {
        cout << "Command Error. Enter an input file and scale factor(x10).\n";
        cout << "Example) snet.exe inputfile.mp4 20\n";
        return -1;
    }

    if (!parseConfigAPI(cfg, videoCW, argc, argv)) {
        cout << "Parsing Error!\n";
        return -1;
    }

    if (!initModel(cfg.srModelFileX2, cfg.srModelFileX1_5, cfg.nvModelFileFHD, cfg.nvModelFileHD)) {
        cout << "Initialization of the solution failed!\n";
        return -1;
    }

    bool endOfFrames = false;
    unsigned int frameCnt = 0;                 // should be unsigned int
    unsigned int frameLimit = cfg.frameLimit;  // number of frames to be processed
    double scaleFactor = cfg.scaleFactorX10 / 10.0;

    clock_t start, end;
    vector<float> infs;

    while (1) {
        Mat frame, resFrame;
        videoCW >> frame;

        // if the frame is empty, break immediately
        if (frame.empty()) {
            cout << "End of Frames\n";
            break;
        }

        start = clock();

        if (cfg.scaleFactorX10 != 10) {
            if (!runModel(frame, resFrame, cfg.scaleFactorX10))  // SR
                break;
        } else {
            if (!runModelNV(frame, resFrame))  // NV
                break;
        }

        end = clock();

        videoCW << resFrame;  // write a frame

        float inf = end - start;

        if (frameCnt > 10 && frameCnt < 500)  // skip the start frames and limit the number of elements
            infs.push_back(inf);

        cout << "Frame " << frameCnt << ">\tInference Time: " << inf << "ms\n";

        if (cfg.filterEnable && cfg.scaleFactorX10 != 10) {
            Mat filterFrame;
            resize(frame, filterFrame, Size(0, 0), scaleFactor, scaleFactor, INTER_LINEAR);
            videoCW.writeFilterOutput(filterFrame);  // write a filter frame
        }

        if (frameLimit != -1 && frameCnt >= frameLimit)
            break;

        frameCnt++;
    }

    destroyModel();  // destroy sr models

    if (infs.size() > 1) {
        float avgInf = accumulate(infs.begin(), infs.end(), 0) / infs.size();
        cout << "\nAverage Inference Time(including post processing): " << avgInf << "ms\n";
    }

    cout << "\nOutput file(s):\n";
    cout << "\t" << cfg.outputFile << endl;
    if (cfg.filterEnable)
        cout << "\t" << cfg.filterFile << endl;

    cout << "\nTerminate program!\n";

    return 0;
}

bool parseConfigAPI(Config &cfg, VideoCW &videoCW, int argc, char *argv[]) {
    string jsonCfgFile = CFG_FILEPATH;
    ifstream cfgFile(jsonCfgFile);
    json js;
    cfgFile >> js;

    if (argc != 3) {
        // apikey
        cfg.frameLimit = js["frame_limit"];
        cfg.filterEnable = js["filter_enable"];
        cfg.inputFile = js["input_file"];
        cfg.outputFile = js["output_file"];

        // sr scaling factors
        cfg.scaleFactorX10 = js["scale_factor_x10"];
    } else {
        // apikey
        cfg.frameLimit = 100;
        cfg.filterEnable = false;
        cfg.inputFile = string("C:\\aipro\\videos\\") + string(argv[1]);

        string outputFile = cfg.inputFile;
        const size_t period_idx = outputFile.rfind('.');
        if (string::npos != period_idx) {
            outputFile.erase(period_idx);
        }

        outputFile = outputFile + "-sr" + ".mp4";
        cfg.outputFile = outputFile;

        // sr scaling factors
        cfg.scaleFactorX10 = (int)atof(argv[2]);
    }

    if (cfg.filterEnable) {
        string filterFile = cfg.inputFile;
        const size_t period_idx = filterFile.rfind('.');
        if (string::npos != period_idx) {
            filterFile.erase(period_idx);
        }

        string filterFileAppend("-filter");
        filterFile = filterFile + filterFileAppend + ".mp4";
        cfg.filterFile = filterFile;
    }

    cfg.srModelFileX1_5 = SR_X1_5_FILEPATH;
    cfg.srModelFileX2 = SR_X2_FILEPATH;
    cfg.nvModelFileHD = NV_HD_FILEPATH;
    cfg.nvModelFileFHD = NV_FHD_FILEPATH;

    // read the list of filepaths
    videoCW.init(cfg.inputFile, cfg.outputFile, cfg.filterFile, cfg.scaleFactorX10, cfg.filterEnable);

    cfg.frameWidth = videoCW.getFrameWidth();
    cfg.frameHeight = videoCW.getFrameHeight();
    cfg.fps = videoCW.getFps();

    return true;
}