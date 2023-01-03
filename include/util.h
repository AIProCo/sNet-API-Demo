/*==============================================================================
* Copyright 2023 AIPro Inc.
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
    VideoWriter *videoWriterFilter;

    string inPath;
    string outPath;
    int frameWidth;
    int frameHeight;
    float fps;
    bool filterFlag;

    VideoCapture &cap() const {
        return *(videoCapturer);
    }
    VideoWriter &writer() const {
        return *(videoWriter);
    }
    void writeFilterOutput(Mat &frame) const {
        videoWriterFilter->write(frame);
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
    bool filterEnable;

   public:
    VideoCW &operator[](int idx) const {
        return *videoCWs[idx];
    }

    VideoDir() {
    }

    VideoDir(const vector<string> &inFilepaths, const vector<string> &outFilepaths,
             const vector<string> &bilinearFilepaths = {}) {
        init(inFilepaths, outFilepaths, bilinearFilepaths);
    }

    void init(const vector<string> &inFilepaths, const vector<string> &outFilepaths,
              const vector<string> &bilinearFilepaths = {}, Size size = Size(0, 0), bool _filterEnable = false) {
        filterEnable = _filterEnable;

        setVideoCWFiles(inFilepaths, outFilepaths, bilinearFilepaths, size);
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

    void setVideoCWFiles(const vector<string> &inFilepaths, const vector<string> &outFilepaths,
                         const vector<string> &filterFilepaths, Size &size) {
        for (size_t i = 0; i < inFilepaths.size(); i++) {
            string inFilepath = inFilepaths[i];
            string outFilepath = outFilepaths[i];
            string filterFilepath = "";

            if (filterFilepaths.size() > i)
                filterFilepath = filterFilepaths[i];

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
            videoCW->filterFlag = false;

            if (size.width != 0 && size.height != 0) {
                videoCW->videoWriter =
                    new VideoWriter(outFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), videoCW->fps, size);

                if (filterEnable && filterFilepath != "") {
                    videoCW->filterFlag = true;

                    videoCW->videoWriterFilter =
                        new VideoWriter(filterFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), videoCW->fps, size);
                }
            } else {
                videoCW->videoWriter = new VideoWriter(outFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'),
                                                       videoCW->fps, Size(videoCW->frameWidth, videoCW->frameHeight));

                if (filterEnable && filterFilepath != "") {
                    videoCW->filterFlag = true;

                    videoCW->videoWriterFilter =
                        new VideoWriter(filterFilepath, VideoWriter::fourcc('m', 'p', '4', 'v'), videoCW->fps,
                                        Size(videoCW->frameWidth, videoCW->frameHeight));
                }
            }

            videoCWs.push_back(videoCW);
        }
    }

    void freeVideoCWFiles(vector<VideoCW *> &videoCWs) {
        for (int i = 0; i < videoCWs.size(); i++) {
            videoCWs[i]->videoCapturer->release();
            delete videoCWs[i]->videoCapturer;

            videoCWs[i]->videoWriter->release();
            delete videoCWs[i]->videoWriter;

            if (filterEnable && videoCWs[i]->filterFlag) {
                videoCWs[i]->videoWriterFilter->release();
                delete videoCWs[i]->videoWriterFilter;
            }

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