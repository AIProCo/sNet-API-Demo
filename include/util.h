/*==============================================================================
* Copyright 2024 AIPro Inc.
* Author: Chun-Su Park (cspk@skku.edu)
=============================================================================*/
#ifndef UTIL_H
#define UTIL_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "global.h"

using namespace std;
using namespace cv;

/// Data structure to represent Video Caputer, Writer
class VideoCW {
   public:
    VideoCapture *videoCapturer;
    VideoWriter *videoWriter;
    VideoWriter *videoWriterFilter;

    string inPath;
    string outPath;
    int frameWidth;
    int frameHeight;
    float fps;
    bool filterEnable;

    VideoCW() {
    }

    ~VideoCW() {
        videoCapturer->release();
        delete videoCapturer;

        videoWriter->release();
        delete videoWriter;

        if (filterEnable) {
            videoWriterFilter->release();
            delete videoWriterFilter;
        }
    }
    void writeFilterOutput(Mat &frame) const {
        videoWriterFilter->write(frame);
    }

    void operator>>(Mat &frame) {
        *(videoCapturer) >> frame;
    }

    void operator<<(Mat &frame) {
        videoWriter->write(frame);
    }

    void init(const string &inFilepath, const string &outFilepath, const string &bilinearFilepath = "",
              int scaleFactorX10 = 0, bool _filterEnable = false) {
        filterEnable = _filterEnable;
        setVideoFile(inFilepath, outFilepath, bilinearFilepath, scaleFactorX10);
    }

    int getFrameWidth() {
        return frameWidth;
    }

    int getFrameHeight() {
        return frameHeight;
    }

    float getFps() {
        return fps;
    }

   private:
    void setVideoFile(const string &inFilepath, const string &outFilepath, const string &filterFilepath,
                      const int &scaleFactorX10) {
        videoCapturer = new VideoCapture(inFilepath);
        double scaleFactor = scaleFactorX10 / 10.0;

        if (!videoCapturer->isOpened()) {
            cout << "Error opening video stream" << inFilepath << endl;
            exit(0);
        }

        frameWidth = videoCapturer->get(CAP_PROP_FRAME_WIDTH);
        frameHeight = videoCapturer->get(CAP_PROP_FRAME_HEIGHT);
        fps = videoCapturer->get(CAP_PROP_FPS);

        Size size(scaleFactor * frameWidth, scaleFactor * frameHeight);

        if (size.width != 0 && size.height != 0) {
            videoWriter = new VideoWriter(outFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, size);

            if (!videoWriter->isOpened()) {
                cout << "videoWriter Error in setVideoFile!\n";
                exit(0);
            }

            if (filterEnable) {
                videoWriterFilter = new VideoWriter(filterFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, size);

                if (!videoWriterFilter->isOpened()) {
                    cout << "videoWriterFilter Error in setVideoFile!\n";
                    exit(0);
                }
            }
        } else {
            cout << "Size Error in setVideoFile!\n";
            exit(0);
        }
    }
};
#endif  // UTIL_H