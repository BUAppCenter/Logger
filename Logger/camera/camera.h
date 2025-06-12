#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <opencv2/opencv.hpp>
#include "../config/Config.h"
#include "../result_data/result_data.h"
#include "create_cid.h"
#include <spdlog/spdlog.h>

using namespace std;
using namespace cv;

class Camera_cls {
public:
    Camera_cls(const Config_cls& config);
    ~Camera_cls();

    void open_camera();
    void lamping_time();
    int set_frame();

    void capture_frame(result_data& result_data);
    void image_save(result_data& result_data);

private:
    int width, height, fps, frame_count;
    string orifile_path;
    
    VideoCapture cap;
    Mat currentFrame;
};

#endif
