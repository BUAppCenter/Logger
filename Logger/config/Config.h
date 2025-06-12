#ifndef CONFIG_H
#define CONFIG_H

#include <string>
using namespace std;

class Config_cls {
private:
    int width;
    int height;
    int fps;
    string orifilePath;
    string objectDetectorIp;
    int objectDetectorPort;
    string mqttTopic;
    string mqttClientId;
    string mqttBrokerIp;
    int mqttBrokerPort;

public:
    Config_cls();  
    ~Config_cls();  

    void Read_Logger_cfg(); 

    int getWidth() const;
    int getHeight() const;
    int getFps() const;
    string getOrifilePath() const;
    string getObjectDetectorIp() const;
    int getObjectDetectorPort() const;
    string getMqttTopic() const;
    string getMqttClientId() const;
    string getMqttBrokerIp() const;
    int getMqttBrokerPort() const;
};

#endif // CONFIG_H
