#include "display.h"
#include "config.h"
#include "mqtt_connect.h"
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

void initDisplay()
{
  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(true);
  pinMode(TFT_BL, OUTPUT);
  analogWrite(TFT_BL, 128);
}

void mostrarEstado(const char *msg, uint16_t color)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(color, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 140);
  tft.println(msg);
}

void mostrarSistemaListo(const char *ssid)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(30, 120);
  tft.println("SISTEMA LISTO");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 170);
  tft.println("WiFi:");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(80, 170);
  tft.println(WiFi.SSID().c_str());

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(20, 200);
  tft.println("MQTT:");
  if (mqttClient.connected())
  {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(80, 200);
    tft.println("Conectado");
  }
  else
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(80, 200);
    tft.println("Desconectado");
  }

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 240);
  tft.print("IP: ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(WiFi.localIP().toString().c_str());
}

void mostrarPortalInfo()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 100);
  tft.println("MODO CONFIG");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 160);
  tft.println("Conectate a:");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 200);
  tft.println(WIFI_AP_NAME);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 250);
  tft.println("IP: 192.168.4.1");
}

void mostrarBorrandoCredenciales()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 130);
  tft.println("Borrando credenciales...");
}
