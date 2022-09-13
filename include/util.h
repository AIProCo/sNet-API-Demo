/*==============================================================================
* Copyright 2022 AIPro Inc.
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
struct VideoCW {
    VideoCapture *videoCapturer;
    VideoWriter *videoWriter;
    string inPath;
    string outPath;
    int frameWidth;
    int frameHeight;
    float fps;

    VideoCapture &cap() const {
        return *(videoCapturer);
    }
    VideoWriter &writer() const {
        return *(videoWriter);
    }

    friend VideoCW &operator>>(VideoCW &videoCW, Mat &frame) {
        videoCW.cap() >> frame;
        return videoCW;
    }

    friend VideoCW &operator<<(VideoCW &videoCW, Mat &frame) {
        videoCW.writer().write(frame);
        return videoCW;
    }
};

/// this is a helper class to read video files in a certain directory
class VideoDir {
   private:
    vector<string> inPaths;
    vector<string> outPaths;
    vector<VideoCW *> videoCWs;
    vector<int> frameWidths;
    vector<int> frameHeights;
    vector<float> fpss;

   public:
    VideoCW &operator[](int idx) const {
        return *videoCWs[idx];
    }

    VideoDir() {
    }

    VideoDir(const vector<string> &inFilepaths, const vector<string> &outFilepaths) {
        init(inFilepaths, outFilepaths);
    }

    void init(const vector<string> &inFilepaths, const vector<string> &outFilepaths, Size size = Size(0, 0)) {
        setVideoCWFiles(inFilepaths, outFilepaths, size);
        setFrameWidthsHeights(videoCWs);
    }

    virtual ~VideoDir() {
        freeVideoCWFiles(this->videoCWs);
    }

    int size() {
        return videoCWs.size();
    }

    vector<int> getFrameWidths() {
        return frameWidths;
    }

    vector<int> getFrameHeights() {
        return frameHeights;
    }

    vector<float> getFpss() {
        return fpss;
    }

    void setVideoCWFiles(const vector<string> &inFilepaths, const vector<string> &outFilepaths, Size &size) {
        for (size_t i = 0; i < inFilepaths.size(); i++) {
            string inFilepath = inFilepaths[i];
            string outFilepath = outFilepaths[i];

            VideoCW *videoCW;
            VideoCapture *capturer = new VideoCapture(inFilepath);

            if (!capturer->isOpened()) {
                cout << "Error opening video stream" << inFilepath << endl;
                break;
            }

            videoCW = new VideoCW();

            if (videoCW == NULL) {
                cout << "videoCW memory allocation error!!" << endl;
                break;
            }

            videoCW->videoCapturer = capturer;
            videoCW->frameWidth = capturer->get(CAP_PROP_FRAME_WIDTH);
            videoCW->frameHeight = capturer->get(CAP_PROP_FRAME_HEIGHT);
            videoCW->fps = capturer->get(CAP_PROP_FPS);

            if (size.width != 0 && size.height != 0)
                videoCW->videoWriter =
                    new VideoWriter(outFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), videoCW->fps, size);
            else
                videoCW->videoWriter = new VideoWriter(outFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'),
                                                       videoCW->fps, Size(videoCW->frameWidth, videoCW->frameHeight));

            videoCWs.push_back(videoCW);
        }
    }

    static void freeVideoCWFiles(vector<VideoCW *> &videoCWs) {
        for (int i = 0; i < videoCWs.size(); i++) {
            videoCWs[i]->videoCapturer->release();
            videoCWs[i]->videoWriter->release();
            delete videoCWs[i]->videoCapturer;
            delete videoCWs[i]->videoWriter;
            delete videoCWs[i];
        }
    }

    void setFrameWidthsHeights(vector<VideoCW *> &videoCWs) {
        for (int i = 0; i < videoCWs.size(); i++) {
            frameWidths.push_back(videoCWs[i]->frameWidth);
            frameHeights.push_back(videoCWs[i]->frameHeight);
            fpss.push_back(videoCWs[i]->fps);
        }
    }
};

/// @brief  A utility function to print out the elements of vector
template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> vec) {
    os << "{ ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
    os << "}";
    return os;
}
#endif  // UTIL_H