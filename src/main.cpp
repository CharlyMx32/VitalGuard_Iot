#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

// --- CONFIGURACIÓN MQTT ---
const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_topic_prefix = "vitalguard/";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Colores personalizados
#define COLOR_HEADER 0x0515
#define COLOR_CARD 0x3186

void dibujarPantalla()
{
  tft.fillScreen(TFT_BLACK);

  // === HEADER ===
  tft.fillRect(0, 0, 320, 55, COLOR_HEADER);
  tft.drawFastHLine(0, 55, 320, TFT_WHITE);

  // Título con fuente smooth grande
  tft.setTextColor(TFT_WHITE, COLOR_HEADER);
  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.setCursor(20, 32);
  tft.println("VitalGuard");

  // Hora
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setTextColor(TFT_CYAN, COLOR_HEADER);
  tft.setCursor(210, 28);
  tft.print("10:30");

  // Indicador de estado
  tft.fillCircle(290, 28, 8, TFT_GREEN);
  tft.fillCircle(290, 28, 4, TFT_BLACK);
  tft.fillCircle(290, 28, 2, TFT_GREEN);

  // === TARJETA DE MEDICAMENTOS ===
  int cardX = 20, cardY = 75, cardW = 280, cardH = 145;

  tft.fillRoundRect(cardX + 2, cardY + 2, cardW, cardH, 10, TFT_DARKGREY);
  tft.fillRoundRect(cardX, cardY, cardW, cardH, 10, COLOR_CARD);
  tft.drawRoundRect(cardX, cardY, cardW, cardH, 10, TFT_WHITE);

  // Título de la tarjeta
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setTextColor(TFT_WHITE, COLOR_CARD);
  tft.setCursor(cardX + 15, cardY + 28);
  tft.println("Proximas tomas:");

  // Medicamentos
  tft.setFreeFont(&FreeSansBold9pt7b);

  // Línea 1
  tft.setTextColor(TFT_YELLOW, COLOR_CARD);
  tft.setCursor(cardX + 15, cardY + 58);
  tft.print("- Paracetamol");
  tft.setTextColor(TFT_WHITE, COLOR_CARD);
  tft.setCursor(cardX + 200, cardY + 58);
  tft.print("7:10");

  // Línea 2
  tft.setTextColor(TFT_YELLOW, COLOR_CARD);
  tft.setCursor(cardX + 15, cardY + 88);
  tft.print("- Ibuprofeno");
  tft.setTextColor(TFT_WHITE, COLOR_CARD);
  tft.setCursor(cardX + 200, cardY + 88);
  tft.print("12:30");

  // Línea 3
  tft.setTextColor(TFT_YELLOW, COLOR_CARD);
  tft.setCursor(cardX + 15, cardY + 118);
  tft.print("- Amoxicilina");
  tft.setTextColor(TFT_WHITE, COLOR_CARD);
  tft.setCursor(cardX + 200, cardY + 118);
  tft.print("18:45");

  // Icono decorativo
  tft.fillCircle(270, 195, 12, TFT_RED);
  tft.fillCircle(270, 195, 8, TFT_WHITE);
  tft.fillCircle(270, 195, 4, TFT_RED);
}

void conectarWiFi()
{
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  wm.setConnectTimeout(15);

  if (!wm.autoConnect("VitalGuard-AP"))
  {
    Serial.println("WiFi: Falló conexión, reiniciando...");
    delay(3000);
    ESP.restart();
  }
  Serial.print("WiFi: Conectado a ");
  Serial.println(WiFi.SSID());
}

void conectarMQTT()
{
  mqttClient.setServer(mqtt_server, mqtt_port);
  if (mqttClient.connect("VitalGuard-ESP32"))
  {
    Serial.println("MQTT: Conectado al servidor");
  }
  else
  {
    Serial.print("MQTT: Falló, rc=");
    Serial.println(mqttClient.state());
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n=== VITALGUARD ST7789 ===");

  conectarWiFi();
  conectarMQTT();

  delay(500);

  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(true);

  tft.fillScreen(TFT_BLACK);
  delay(100);

  dibujarPantalla();

  Serial.println("¡Listo!");
}

void loop()
{
  mqttClient.loop();
  delay(1000);
}