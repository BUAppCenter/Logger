#ifndef MQTT_H
#define MQTT_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <queue>
#include "../result_data/result_data.h"
#include "../config/Config.h"
#include <mqtt/async_client.h>
#include <mutex>
#include <chrono>
#include <pthread.h>
#include <thread>
#include <spdlog/spdlog.h>

class Mqtt_cls {
public:
    explicit Mqtt_cls(const Config_cls& config);
    ~Mqtt_cls();

    void publish_result(queue<result_data>& result_data_queue, std::mutex& queue_mutex);
    void publish_result_task(queue<result_data>& result_data_queue, std::mutex& queue_mutex, int thread_id);
    void start_publish_threads(queue<result_data>& result_data_queue, std::mutex& queue_mutex, int num_threads);

    void disconnect();

private:
    std::string m_topic;
    mqtt::async_client m_client;

    std::vector<std::thread> m_publish_threads;
};


#endif // MQTT_H
