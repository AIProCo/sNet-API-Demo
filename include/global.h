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
    uint frameLimit;         /// number of frames to be processed
    std::string inputFile;   /// list of the input files
    std::string outputFile;  /// list of the SR output files
    std::string filterFile;  /// list of the filter output files

    // sr config
    std::string srModelFileX1_5;  /// x1.5 SR model
    std::string srModelFileX2;    /// x2 SR model
    bool filterEnable;            /// Enable filter

    // nv config
    std::string nvModelFileHD;
    std::string nvModelFileFHD;

    // 10: Night Vision, 15: SR x1.5, 20: SR x2.0
    int scaleFactorX10;  /// scale factor from net input to output

    int frameWidth;   /// widths of the input frame
    int frameHeight;  /// heights of the input frame
    float fps;        /// fpss of the input frame
};
