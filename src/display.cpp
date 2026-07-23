#include "display.h"
#include "config.h"

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
  tft.setCursor(30, 140);
  tft.println("SISTEMA LISTO");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(40, 190);
  tft.println("Conectado a:");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(40, 220);
  tft.println(ssid);
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
