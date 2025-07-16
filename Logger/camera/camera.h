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

    void capture_frame(queue<result_data>& result_data_queue);
    void capture_frame_task(queue<result_data>& result_data_queue);
    void start_capture_thread(queue<result_data>& result_data_queue);
    void image_save(queue<result_data>& result_data_queue);
    void image_save_task(queue<result_data>& result_data_queue);
    void start_save_thread(queue<result_data>& result_data_queue);

private:
    int width, height, fps, frame_count;
    string orifile_path;
    
    VideoCapture cap;
    Mat currentFrame;

    thread capture_thread;
    thread image_save_thread;

};

#endif
