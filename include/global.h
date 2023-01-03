/*==============================================================================
* Copyright 2023 AIPro Inc.
* Author: Chun-Su Park (cspk@skku.edu)
=============================================================================*/
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

typedef unsigned char uchar;
typedef unsigned int uint;

struct Config {
    std::string key;                       /// authorization Key
    uint frameLimit;                       /// number of frames to be processed
    std::vector<std::string> inputFiles;   /// list of the input files
    std::vector<std::string> outputFiles;  /// list of the SR output files
    std::vector<std::string> filterFiles;  /// list of the filter output files

    // sr config
    bool srEnable;            /// Enable SR
    bool filterEnable;        /// Enable filter
    std::string srModelFile;  /// path to the od config file (ex:aipro_sr_x2_1_2.net)
    int netWidth;             /// width of model input
    int netHeight;            /// height of model input
    int srBatchSize;          /// batch size of the od model
    double scaleFactor;       /// scale factor from net input to sr output
    cv::Size outputSize;      /// size of output frame

    int numChannels;                /// number of video channels (unlimited)
    std::vector<int> frameWidths;   /// widths of the input frames
    std::vector<int> frameHeights;  /// heights of the input frames
    std::vector<float> fpss;        /// fpss of the input frames
};
