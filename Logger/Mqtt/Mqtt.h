#ifndef MQTT_H
#define MQTT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "../result_data/result_data.h"
#include "../config/Config.h"
#include <mqtt/async_client.h>

class Mqtt_cls {
public:
    explicit Mqtt_cls(const Config_cls& config);

    void publish_result(result_data& result_data);
    void disconnect();

private:
    std::string m_topic;
    mqtt::async_client m_client;
};

#endif // MQTT_H
