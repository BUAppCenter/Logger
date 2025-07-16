#ifndef RESULT_DATA_H
#define RESULT_DATA_H

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct result_data {
    string cid;
    Mat BGR_frame;
    vector<uchar> encoded_buffer;
    string object_Detection_result;

    bool image_save_state = false;
    bool object_Detection_result_state = false;
    bool mqtt_publish_state = false;
};
#endif
