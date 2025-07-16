#include "Mqtt.h"

using json = nlohmann::json;

Mqtt_cls::Mqtt_cls(const Config_cls& config)
    : m_topic(config.getMqttTopic()),
      m_client("tcp://" + config.getMqttBrokerIp() + ":" + std::to_string(config.getMqttBrokerPort()),
               config.getMqttClientId())
{
    try {
        m_client.connect()->wait();
        std::cout << "[MQTT] Connected to broker" << std::endl;
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Connection failed: " << e.what() << std::endl;
    }
}

Mqtt_cls::~Mqtt_cls() {
    for (auto& thread : m_publish_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    disconnect();
}

void Mqtt_cls::publish_result(queue<result_data>& result_data_queue, std::mutex& queue_mutex) {

    json response;

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (result_data_queue.empty()) {
            std::cerr << "[MQTT] No data to publish." << std::endl;
            return;
        }
        response = {
            {"cid", result_data_queue.front().cid},
            {"result", result_data_queue.front().object_Detection_result}
        };
        result_data_queue.pop();
    }

    try {
        mqtt::message_ptr pubmsg = mqtt::make_message(m_topic, response.dump());
        pubmsg->set_qos(1);

        m_client.publish(pubmsg)->wait();

        std::cout << "[MQTT] Published result" << std::endl;
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Publish failed: " << e.what() << std::endl;
    }

}

void Mqtt_cls::publish_result_task(queue<result_data>& result_data_queue, std::mutex& queue_mutex, int thread_id) {
    // MQTT publish 작업을 수행하는 스레드
    std::string thread_name = "mqtt_pub_" + std::to_string(thread_id);
    pthread_setname_np(pthread_self(), thread_name.c_str());

    while (true) {
        if (!result_data_queue.empty()) {
            if (result_data_queue.front().object_Detection_result_state == true &&
                result_data_queue.front().image_save_state == true) {

                auto total_start_time = std::chrono::steady_clock::now();
                publish_result(result_data_queue, queue_mutex);
                auto total_end_time = std::chrono::steady_clock::now();

                int duration = std::chrono::duration_cast<std::chrono::milliseconds>(total_end_time - total_start_time).count();
                // spdlog::info("mqtt_publish_thread: {} ms", duration);

                std::cout << "[" << thread_name << "]: " << duration << " ms" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Mqtt_cls::start_publish_threads(queue<result_data>& result_data_queue, std::mutex& queue_mutex, int num_threads) {
    for (int i = 0; i < num_threads; ++i) {
        m_publish_threads.emplace_back(&Mqtt_cls::publish_result_task, this, std::ref(result_data_queue), std::ref(queue_mutex), i);
    }
}
void Mqtt_cls::disconnect() {
    try {
        m_client.disconnect()->wait();
        std::cout << "[MQTT] Disconnected." << std::endl;
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Disconnect failed: " << e.what() << std::endl;
    }
}
