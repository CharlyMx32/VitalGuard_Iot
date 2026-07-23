#include <Arduino.h>
#include <esp_task_wdt.h>
#include "config.h"
#include "display.h"
#include "wifi_connect.h"
#include "mqtt_connect.h"



void borrarCredencialesPantalla()
{
  mostrarBorrandoCredenciales();
  borrarCredenciales();
  wm.resetSettings();
  delay(1000);
  mostrarEstado("Credenciales borradas!", TFT_GREEN);
  delay(2000);
  ESP.restart();
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.println("\n=== VITALGUARD ST7789 ===");

  esp_task_wdt_init(10, true);
  esp_task_wdt_add(NULL);

  pinMode(BOTON_RESET, INPUT_PULLUP);
  initDisplay();

  if (digitalRead(BOTON_RESET) == LOW)
  {
    borrarCredencialesPantalla();
    while (1);
  }

  if (conectarWiFi())
  {
    mostrarEstado("WiFi: Conectado!", TFT_GREEN);
    Serial.printf("[VitalGuard] IP: %s\n", WiFi.localIP().toString().c_str());
    delay(500);

    conectarMQTT();

    mostrarSistemaListo(WiFi.SSID().c_str());
  }
  else
  {
    mostrarPortalInfo();
  }

  Serial.println("Setup completado!");
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!mqttClient.connected())
    {
      static unsigned long lastMqttAttempt = 0;
      if (millis() - lastMqttAttempt > MQTT_RECONNECT_DELAY)
      {
        lastMqttAttempt = millis();
        Serial.println("[MQTT] Reconnectando...");
        conectarMQTT();
      }
    }
    else
    {
      mqttLoop();
    }
  }

  if (digitalRead(BOTON_RESET) == LOW)
  {
    delay(2000);
    if (digitalRead(BOTON_RESET) == LOW)
    {
      borrarCredencialesPantalla();
    }
  }

  esp_task_wdt_reset();
  delay(100);
}
