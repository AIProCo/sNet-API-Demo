/*==============================================================================
* Copyright 2022 AIPro Inc.
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

/** @brief Run the SR model for a frame batch
 *
 * @param frames batch of frames
 * @param srFrames batch of output SR frames
 * @param vchIDs vchIDs of batched frames
 * @return flag for the running result(true: success, false: fail)
 */
GENERATOR_API bool runModel(std::vector<cv::Mat> &frames, std::vector<cv::Mat> &srFrames, std::vector<int> &vchIDs);

/** @brief Destroy all models
 *
 * @param None
 * @return flag for the destruction result(true: success, false: fail)
 */
GENERATOR_API bool destroyModel();