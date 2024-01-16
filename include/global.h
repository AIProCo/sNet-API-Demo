/*==============================================================================
* Copyright 2024 AIPro Inc.
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
    uint frameLimit;                       /// number of frames to be processed
    std::vector<std::string> inputFiles;   /// list of the input files
    std::vector<std::string> outputFiles;  /// list of the SR output files
    std::vector<std::string> filterFiles;  /// list of the filter output files

    std::string srModelFileX1_5;  /// x1.5 SR model
    std::string srModelFileX2;    /// x2 SR model

    // sr config
    bool filterEnable;                 /// Enable filter
    std::vector<double> scaleFactors;  /// scale factor from net input to sr output

    int numChannels;                /// number of video channels (unlimited)
    std::vector<int> frameWidths;   /// widths of the input frames
    std::vector<int> frameHeights;  /// heights of the input frames
    std::vector<float> fpss;        /// fpss of the input frames
};
