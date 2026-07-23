#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

void initDisplay();
void mostrarEstado(const char *msg, uint16_t color);
void mostrarSistemaListo(const char *ssid);
void mostrarPortalInfo();
void mostrarBorrandoCredenciales();
