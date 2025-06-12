#ifndef OBJECT_DETECTION_H
#define OBJECT_DETECTION_H

#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <queue>
#include "../config/Config.h"
#include "../result_data/result_data.h"
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
#include <fstream>


class Object_detection {
public:
    Object_detection(const Config_cls& config);
    void send_request(result_data& result_data);

private:
    std::string m_server_ip_;
    int m_server_port_;
};

#endif
