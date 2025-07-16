#include <iostream>
#include "config/Config.h"
#include "Mqtt/Mqtt.h"
#include "camera/camera.h"
#include "object_detection/object_detection.h"
#include "result_data/result_data.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h> 

using namespace std;    

int main() {
    Config_cls config_inst;

    Camera_cls camera_inst(config_inst);
    Object_detection Object_detection_inst(config_inst);
    Mqtt_cls mqtt_inst(config_inst);

    queue<result_data> result_data_queue;
    mutex queue_mutex;

    //스레드 시작
    camera_inst.start_capture_thread(result_data_queue);
    camera_inst.start_save_thread(result_data_queue);
    Object_detection_inst.start_send_request_thread(result_data_queue);
    mqtt_inst.start_publish_threads(result_data_queue, queue_mutex, config_inst.getNumMqttThreads());

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    //스레드 종료
    return 0;
}
