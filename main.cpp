/*==============================================================================
* Copyright 2022 AIPro Inc.
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
#define SR_X2_FILEPATH "inputs/aipro_sr_x2_1_2.net"
#define SR_X1_5_FILEPATH "inputs/aipro_sr_x1_5_1_2.net"

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

    vector<Mat> frames, srFrames;
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

    cfg.inputFiles = js["global"]["input_files"].get<vector<string>>();
    cfg.outputFiles = js["global"]["output_files"].get<vector<string>>();

    if (cfg.inputFiles.size() != cfg.outputFiles.size()) {
        cout << "input_files and output_files should be the same size!!";
        return false;
    }

    // sr config
    cfg.srEnable = true;
    cfg.srBatchSize = 4;  // from 1 to 4

    cfg.job = js["sr"]["job"];
    transform(cfg.job.begin(), cfg.job.end(), cfg.job.begin(), ::toupper);

    if (cfg.job == "SR_X2") {
        cfg.scaleFactor = 2;  // fixed
        cfg.srModelFile = SR_X2_FILEPATH;
        cfg.netWidth = 960;   // fixed
        cfg.netHeight = 540;  // fixed

    } else if (cfg.job == "SR_X1_5") {
        cfg.scaleFactor = 1.5; // fixed
        cfg.srModelFile = SR_X1_5_FILEPATH;
        cfg.netWidth = 1280;   // fixed
        cfg.netHeight = 720;   // fixed
    } else {
        cout << "SR job setting Error" << endl;
        return false;
    }

    // read the list of filepaths
    videoDir.init(cfg.inputFiles, cfg.outputFiles,
                  Size(cfg.scaleFactor * cfg.netWidth, cfg.scaleFactor * cfg.netHeight));

    cfg.numChannels = videoDir.size();
    cfg.frameWidths = videoDir.getFrameWidths();
    cfg.frameHeights = videoDir.getFrameHeights();
    cfg.fpss = videoDir.getFpss();

    return true;
}