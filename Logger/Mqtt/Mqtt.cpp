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

void Mqtt_cls::publish_result(result_data& result_data) {

    json response = {
        {"cid", result_data.cid},
        {"result", result_data.object_Detection_result}
    };

    try {
        mqtt::message_ptr pubmsg = mqtt::make_message(m_topic, response.dump());
        pubmsg->set_qos(1);

        m_client.publish(pubmsg)->wait();

        std::cout << "[MQTT] Published result for cid: " << result_data.cid << std::endl;
    } catch (const mqtt::exception& e) {
        std::cerr << "[MQTT] Publish failed: " << e.what() << std::endl;
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
