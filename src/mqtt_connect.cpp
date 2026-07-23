#include "mqtt_connect.h"
#include "display.h"
#include "config.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void conectarMQTT()
{
  if (mqttClient.connected()) return;

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  if (mqttClient.connect(MQTT_CLIENT_ID))
  {
    Serial.println("[MQTT] Conectado al broker");
  }
  else
  {
    Serial.printf("[MQTT] Fallo, rc=%d\n", mqttClient.state());
  }
}

void mqttLoop()
{
  mqttClient.loop();
}
