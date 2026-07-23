#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;

void conectarMQTT();
void mqttLoop();
