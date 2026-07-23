#pragma once

#include <Arduino.h>
#include <WiFiManager.h>

extern WiFiManager wm;

void guardarCredenciales(const char *ssid, const char *pass);
bool cargarCredenciales(String &ssid, String &pass);
void borrarCredenciales();
void bindServerCallback();
void configModeCallback(WiFiManager *wm);
bool conectarWiFi();
