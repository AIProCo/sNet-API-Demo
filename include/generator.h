/*==============================================================================
* Copyright 2024 AIPro Inc.
* Author: Chun-Su Park (cspk@skku.edu)
=============================================================================*/
#pragma once

#ifdef GENERATOR_EXPORTS
#define GENERATOR_API __declspec(dllexport)
#else
#define GENERATOR_API __declspec(dllimport)
#endif

#include <iostream>
#include <opencv2/core.hpp>

#include "global.h"

/** @brief Initialize model
 *
 * @param _srModelFileX2 x2 SR model file path
 * @param _srModelFileX1_5 x1_5 SR model file path
 * @return initialization result(true: success, false: fail)
 */
GENERATOR_API bool initModel(std::string _srModelFileX2, std::string _srModelFileX1_5, std::string _nvModelFileFHD,
                             std::string _nvModelFileHD);

/** @brief Run the SR model for a frame
 *
 * @param frame input frame
 * @param srFrame output SR frame
 * @param scaleFactor scale factor(x10) from frame to srFrame
 * @return flag for the running result(true: success, false: fail)
 */
GENERATOR_API bool runModel(cv::Mat &frame, cv::Mat &srFrame, int scaleFactorX10);

/** @brief Run the SR model for a frame
 *
 * @param frame input frame
 * @param nvFrame output frame
 * @return flag for the running result(true: success, false: fail)
 */
GENERATOR_API bool runModelNV(cv::Mat &frame, cv::Mat &nvFrame);

/** @brief Destroy all models
 *
 * @param None
 * @return flag for the destruction result(true: success, false: fail)
 */
GENERATOR_API bool destroyModel();