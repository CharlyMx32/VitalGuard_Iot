#include "mqtt_connect.h"
#include "display.h"
#include "config.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void conectarMQTT()
{
  mostrarEstado("Conectando MQTT...", TFT_YELLOW);
  delay(100);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  if (mqttClient.connect(MQTT_CLIENT_ID))
  {
    mostrarEstado("MQTT: Conectado!", TFT_GREEN);
    Serial.println("[MQTT] Conectado al broker");
  }
  else
  {
    mostrarEstado("MQTT: Fallo conexion", TFT_RED);
    Serial.printf("[MQTT] Fallo, rc=%d\n", mqttClient.state());
  }
  delay(800);
}

void mqttLoop()
{
  mqttClient.loop();
}
