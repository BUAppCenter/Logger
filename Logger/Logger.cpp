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

    result_data result_data;

    for(int i = 0; i < 10; i++) {
    camera_inst.capture_frame(result_data);
    camera_inst.image_save(result_data);
    Object_detection_inst.send_request(result_data);
    mqtt_inst.publish_result(result_data);
}

    mqtt_inst.disconnect();

    return 0;
}
