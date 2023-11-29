/*==============================================================================
* Copyright 2023 AIPro Inc.
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
 * @param cfg configuration struct
 * @return initialization result(true: success, false: fail)
 */
GENERATOR_API bool initModel(Config &cfg);

/** @brief Run the SR model for a frame
 *
 * @param frame input frame
 * @param srFrame output SR frame
 * @param scaleFactor scale factor from frame to srFrame
 * @return flag for the running result(true: success, false: fail)
 */
GENERATOR_API bool runModel(cv::Mat &frame, cv::Mat &srFrame, double scaleFactor);

/** @brief Destroy all models
 *
 * @param None
 * @return flag for the destruction result(true: success, false: fail)
 */
GENERATOR_API bool destroyModel();