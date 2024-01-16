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
#define SR_X2_FILEPATH "inputs/aipro_sr_x2_1_5.net"
#define SR_X1_5_FILEPATH "inputs/aipro_sr_x1_5_1_5.net"

using namespace std;
using namespace cv;
using json = nlohmann::json;

bool parseConfigAPI(Config &cfg, VideoDir &videoDir, int argc, char *argv[]);

int main(int argc, char *argv[]) {
    Config cfg;
    VideoDir videoDir;

    // cout << argv[0] << ", " << argv[1] << ", " << argv[2] << endl;

    if (argc != 1 && argc != 3) {
        cout << "Command Error. Enter an input file and scale factor\n";
        cout << "Example) snet.exe inputfile.mp4 2.0\n";
        return -1;
    }

    if (!parseConfigAPI(cfg, videoDir, argc, argv)) {
        cout << "Parsing Error!\n";
        return -1;
    }

    if (!initModel(cfg.srModelFileX2, cfg.srModelFileX1_5)) {
        cout << "Initialization of the solution failed!\n";
        return -1;
    }

    bool endOfFrames = false;
    unsigned int frameCnt = 0;                 // should be unsigned int
    unsigned int frameLimit = cfg.frameLimit;  // number of frames to be processed

    clock_t start, end;
    vector<float> infs;

    while (1) {
        frameCnt++;

        for (int vchID = 0; vchID < cfg.numChannels; vchID++) {
            Mat frame, srFrame;
            (videoDir[vchID]) >> frame;

            // if the frame is empty, break immediately
            if (frame.empty()) {
                cout << "End of Frames\n";
                endOfFrames = true;
                break;
            }

            double scaleFactor = cfg.scaleFactors[vchID];

            start = clock();

            // reconstruct srFrames using frames
            if (!runModel(frame, srFrame, scaleFactor))
                break;

            end = clock();

            (videoDir[vchID]) << srFrame;  // write a frame

            float inf = end - start;

            if (frameCnt > 10 && frameCnt < 500)  // skip the start frames and limit the number of elements
                infs.push_back(inf);

            cout << "Frame " << frameCnt << "/" << (videoDir[vchID]).totalFrames << ">\tInference Time: " << inf << "ms\n";

            if (cfg.filterEnable) {
                Mat filterFrame;
                resize(frame, filterFrame, Size(0, 0), scaleFactor, scaleFactor, INTER_LINEAR);
                (videoDir[vchID]).writeFilterOutput(filterFrame);  // write a filter frame
            }
        }

        if ((frameLimit != -1 && frameCnt >= frameLimit) || endOfFrames)
            break;
    }

    destroyModel();  // destroy sr models

    if (infs.size() > 1) {
        float avgInf = accumulate(infs.begin(), infs.end(), 0) / infs.size();
        cout << "\nAverage Inference Time(including post processing): " << avgInf << "ms\n";
    }

    cout << "\nOutput file(s):\n";
    for (auto &outFile : cfg.outputFiles)
        cout << "\t" << outFile << endl;

    for (auto &filterFile : cfg.filterFiles)
        cout << "\t" << filterFile << endl;

    cout << "\nTerminate program!\n";

    return 0;
}

bool parseConfigAPI(Config &cfg, VideoDir &videoDir, int argc, char *argv[]) {
    string jsonCfgFile = CFG_FILEPATH;
    ifstream cfgFile(jsonCfgFile);
    json js;
    cfgFile >> js;

    if (argc != 3) {
        // apikey
        cfg.frameLimit = js["global"]["frame_limit"];
        cfg.filterEnable = js["global"]["filter_enable"];

        cfg.inputFiles = js["global"]["input_files"].get<vector<string>>();
        cfg.outputFiles = js["global"]["output_files"].get<vector<string>>();
        cfg.filterFiles.clear();

        // sr scaling factors
        cfg.scaleFactors = js["sr"]["scale_factors"].get<vector<double>>();
    } else {
        // apikey
        cfg.frameLimit = 500000;
        cfg.filterEnable = false;

        string inputFile = string("C:\\aipro\\videos\\") + string(argv[1]);

        cfg.inputFiles.push_back(inputFile);

        string outputFile = inputFile;
        const size_t period_idx = outputFile.rfind('.');
        if (string::npos != period_idx) {
            outputFile.erase(period_idx);
        }

        outputFile = outputFile + "-sr" + ".mp4";
        cfg.outputFiles.push_back(outputFile);

        // sr scaling factors
        cfg.scaleFactors.push_back((double)atof(argv[2]));
    }

    if (cfg.inputFiles.size() != cfg.outputFiles.size()) {
        cout << "input_files and output_files should be the same size!!";
        return false;
    }

    if (cfg.filterEnable) {
        for (auto &inputFile : cfg.inputFiles) {
            string filterFile = inputFile;
            const size_t period_idx = filterFile.rfind('.');
            if (string::npos != period_idx) {
                filterFile.erase(period_idx);
            }

            string filterFileAppend = js["global"]["filter_file_append"];
            filterFile = filterFile + filterFileAppend + ".mp4";
            cfg.filterFiles.push_back(filterFile);
        }
    }

    cfg.srModelFileX1_5 = SR_X1_5_FILEPATH;
    cfg.srModelFileX2 = SR_X2_FILEPATH;

    // read the list of filepaths
    videoDir.init(cfg.inputFiles, cfg.outputFiles, cfg.filterFiles, cfg.scaleFactors, cfg.filterEnable);

    cfg.numChannels = videoDir.size();
    cfg.frameWidths = videoDir.getFrameWidths();
    cfg.frameHeights = videoDir.getFrameHeights();
    cfg.fpss = videoDir.getFpss();

    return true;
}