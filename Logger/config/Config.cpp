#include "Config.h"
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
using namespace std;

Config_cls::Config_cls() {
    Read_Logger_cfg(); 
}

Config_cls::~Config_cls() {
    
}

void Config_cls::Read_Logger_cfg() {
    string configPath = "../config.json"; 

    ifstream json_dir(configPath);
    if (!json_dir.is_open()) {
        cerr << "Could not open configuration file" << endl;
        exit(-1);
    }

    Json::CharReaderBuilder builder;                    //파서
    builder["collectComments"] = false;                 //주석
    Json::Value value;
    JSONCPP_STRING errs;

    bool ok = Json::parseFromStream(builder, json_dir, &value, &errs);
    if (!ok) {
        cerr << "Failed to parse configuration: " << errs << endl;
        exit(-1);
    }

    width = value["Logger"]["width"].asInt();
    height = value["Logger"]["height"].asInt();
    fps = value["Logger"]["fps"].asInt();
    orifilePath = value["Logger"]["original file path"].asString();
    objectDetectorIp = value["Logger"]["Object Detector Server IP addr"].asString();
    objectDetectorPort = value["Logger"]["Object Detector Server port"].asInt();
    mqttTopic = value["Logger"]["Mqtt Topic"].asString();
    mqttClientId = value["Logger"]["Mqtt Client Id"].asString();
    mqttBrokerIp = value["Logger"]["Mqtt Broker IP addr"].asString();
    mqttBrokerPort = value["Logger"]["Mqtt Broker port"].asInt();
    num_mqtt_threads = value["Logger"]["Mqtt Threads"].asInt();
}

int Config_cls::getWidth() const { return width; }
int Config_cls::getHeight() const { return height; }
int Config_cls::getFps() const { return fps; }
string Config_cls::getOrifilePath() const { return orifilePath; }
string Config_cls::getObjectDetectorIp() const { return objectDetectorIp; }
int Config_cls::getObjectDetectorPort() const { return objectDetectorPort; }
string Config_cls::getMqttTopic() const { return mqttTopic; }
string Config_cls::getMqttClientId() const { return mqttClientId; }
string Config_cls::getMqttBrokerIp() const { return mqttBrokerIp; }
int Config_cls::getMqttBrokerPort() const { return mqttBrokerPort; }
int Config_cls::getNumMqttThreads() const { return num_mqtt_threads; }
