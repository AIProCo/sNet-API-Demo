/*==============================================================================
* Copyright 2023 AIPro Inc.
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
#define SR_X2_FILEPATH "inputs/aipro_sr_x2_1_3.net"
#define SR_X1_5_FILEPATH "inputs/aipro_sr_x1_5_1_3.net"

using namespace std;
using namespace cv;
using json = nlohmann::json;

bool parseConfigAPI(Config &cfg, VideoDir &videoDir);

int main() {
    Config cfg;
    VideoDir videoDir;

    if (!parseConfigAPI(cfg, videoDir)) {
        cout << "Parsing Error!\n";
        return -1;
    }

    if (!initModel(cfg)) {
        cout << "Initialization of the solution failed!\n";
        return -1;
    }

    bool endOfFrames = false;
    unsigned int frameCnt = 0;                 // should be unsigned int
    unsigned int frameLimit = cfg.frameLimit;  // number of frames to be processed
    int srBatchSize = cfg.srBatchSize;         // batch size of super resolution

    vector<Mat> frames, srFrames, bilinearFrames;
    vector<int> vchIDs;
    clock_t start, end;
    vector<float> infs;

    while (1) {
        frameCnt++;

        for (int i = 0; i < cfg.numChannels; i++) {
            Mat frame;
            (videoDir[i]) >> frame;

            // if the frame is empty, break immediately
            if (frame.empty()) {
                cout << "End of Frames\n";
                endOfFrames = true;
                break;
            }

            frames.push_back(frame);
            vchIDs.push_back(i);

            // accumulate frames as many as batch
            if (frames.size() < srBatchSize)
                continue;

            start = clock();

            // reconstruct srFrames using frames
            if (!runModel(frames, srFrames, vchIDs))
                break;

            end = clock();

            for (int b = 0; b < srBatchSize; b++) {
                int vchID = vchIDs[b];
                (videoDir[vchID]) << srFrames[b];  // write a frame
            }

            float inf = (end - start) / srBatchSize;

            if (frameCnt > 10 && frameCnt < 500)  // skip the start frames and limit the number of elements
                infs.push_back(inf);

            cout << "Frame " << frameCnt << ">\tInference Time: " << inf << "ms\n";

            if (cfg.filterEnable) {
                for (int b = 0; b < srBatchSize; b++) {
                    int vchID = vchIDs[b];

                    Mat filterFrame;

                    resize(frames[b], filterFrame, cfg.outputSize, 0.0, 0.0, INTER_LINEAR);
                    (videoDir[vchID]).writeFilterOutput(filterFrame);  // write a filter frame
                }
            }

            frames.clear();
            srFrames.clear();
            vchIDs.clear();
        }

        if (frameCnt >= frameLimit || endOfFrames)
            break;
    }

    destroyModel();  // destroy sr model

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

bool parseConfigAPI(Config &cfg, VideoDir &videoDir) {
    string jsonCfgFile = CFG_FILEPATH;
    ifstream cfgFile(jsonCfgFile);
    json js;
    cfgFile >> js;

    // apikey
    cfg.frameLimit = js["global"]["frame_limit"];
    cfg.key = js["global"]["apikey"];
    cfg.filterEnable = js["global"]["filter_enable"];

    cfg.inputFiles = js["global"]["input_files"].get<vector<string>>();
    cfg.outputFiles = js["global"]["output_files"].get<vector<string>>();
    cfg.filterFiles.clear();

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

    // sr config
    cfg.srEnable = true;
    cfg.srBatchSize = 1;  // should be 1
    cfg.scaleFactor = js["sr"]["scale_factor"];
    cfg.netHeight = js["sr"]["net_height"];
    cfg.netWidth = js["sr"]["net_width"];

    if (cfg.scaleFactor == 2.0) {
        cfg.srModelFile = SR_X2_FILEPATH;
    } else if (cfg.scaleFactor == 1.5) {
        cfg.srModelFile = SR_X1_5_FILEPATH;
    } else {
        cout << "Scaling factor error!" << endl;
        return false;
    }

    cfg.outputSize = Size(cfg.scaleFactor * cfg.netWidth, cfg.scaleFactor * cfg.netHeight);

    // read the list of filepaths
    videoDir.init(cfg.inputFiles, cfg.outputFiles, cfg.filterFiles, cfg.outputSize, cfg.filterEnable);

    cfg.numChannels = videoDir.size();
    cfg.frameWidths = videoDir.getFrameWidths();
    cfg.frameHeights = videoDir.getFrameHeights();
    cfg.fpss = videoDir.getFpss();

    return true;
}