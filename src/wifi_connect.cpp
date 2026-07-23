#include "wifi_connect.h"
#include "display.h"
#include "portal.h"
#include "config.h"
#include <Preferences.h>
#include <WiFi.h>

WiFiManager wm;
Preferences prefs;

unsigned long lastScanTime = 0;
String cachedNetworks = "{\"networks\":[]}";

void iniciarEscaneo()
{
  WiFi.scanNetworks(true);
  lastScanTime = millis();
}

void guardarCredenciales(const char *ssid, const char *pass)
{
  prefs.begin(PREFS_NAMESPACE, false);
  prefs.putString(PREFS_KEY_SSID, ssid);
  prefs.putString(PREFS_KEY_PASS, pass);
  prefs.end();
  Serial.println("[Creds] Credenciales guardadas");
}

bool cargarCredenciales(String &ssid, String &pass)
{
  prefs.begin(PREFS_NAMESPACE, true);
  ssid = prefs.getString(PREFS_KEY_SSID, "");
  pass = prefs.getString(PREFS_KEY_PASS, "");
  prefs.end();
  return ssid.length() > 0;
}

void borrarCredenciales()
{
  prefs.begin(PREFS_NAMESPACE, false);
  prefs.remove(PREFS_KEY_SSID);
  prefs.remove(PREFS_KEY_PASS);
  prefs.end();
  Serial.println("[Creds] Credenciales borradas");
}

void configModeCallback(WiFiManager *)
{
  iniciarEscaneo();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 60);
  tft.println("=== PORTAL WiFi ===");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 110);
  tft.println("Conectate a:");

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 150);
  tft.println(WIFI_AP_NAME);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 200);
  tft.println("IP: 192.168.4.1");
}

void bindServerCallback()
{
  Serial.println("[Server] Configurando rutas...");

  wm.server->on("/", HTTP_GET, []()
  {
    Serial.println("[Server] Pagina principal");
    wm.server->send_P(200, "text/html", WIFI_PORTAL_HTML);
  });

  wm.server->on("/success", HTTP_GET, []()
  {
    Serial.println("[Server] Pagina de exito");
    wm.server->send_P(200, "text/html", SUCCESS_HTML);
  });

  wm.server->on("/scan", HTTP_GET, []()
  {
    if (millis() - lastScanTime < 30000 && cachedNetworks != "{\"networks\":[]}")
    {
      wm.server->send(200, "application/json", cachedNetworks);
      return;
    }

    int n = WiFi.scanComplete();
    if (n == -2)
    {
      iniciarEscaneo();
      wm.server->send(200, "application/json", "{\"networks\":[]}");
      return;
    }
    if (n == -1)
    {
      wm.server->send(200, "application/json", "{\"networks\":[]}");
      return;
    }

    String json = "{\"networks\":[";
    int count = 0;
    for (int i = 0; i < n; i++)
    {
      String ssid = WiFi.SSID(i);
      if (ssid.length() > 0)
      {
        if (count > 0) json += ",";
        json += "{\"ssid\":\"" + ssid + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
        count++;
      }
    }
    WiFi.scanDelete();
    json += "]}";

    cachedNetworks = json;
    lastScanTime = millis();
    wm.server->send(200, "application/json", json);
  });

  wm.server->on("/connect", HTTP_POST, []()
  {
    wm.server->sendHeader("Location", "/", true);
    wm.server->send(302, "text/plain", "");
  });

  wm.server->on("/wifi", HTTP_POST, []()
  {
    String ssid = wm.server->arg("s");
    String pass = wm.server->arg("p");
    Serial.printf("[Server] Conectando a: %s\n", ssid.c_str());

    WiFi.begin(ssid.c_str(), pass.c_str());
    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20)
    {
      delay(500);
      intentos++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      guardarCredenciales(ssid.c_str(), pass.c_str());
      Serial.println("[Server] Conexion exitosa!");
      wm.server->send_P(200, "text/html", SUCCESS_HTML);
      delay(3000);
      ESP.restart();
    }
    else
    {
      Serial.println("[Server] Conexion fallida");
      wm.server->send(200, "text/html",
        "<html><head><meta http-equiv='refresh' content='4;url=/'></head><body style='font-family:sans-serif;background:#0A192F;color:#E2E8F0;display:flex;align-items:center;justify-content:center;height:100vh;'><div style='text-align:center;padding:20px;'><h2 style='color:#FF6B6B;'>Error al conectar</h2><p style='color:#94A3B8;'>Verifica la contrasena e intenta de nuevo</p><p style='color:#64748B;font-size:13px;margin-top:20px;'>Volviendo a la configuracion...</p></div></body></html>");
    }
  });

  wm.server->onNotFound([]()
  {
    wm.server->send(200, "text/plain", "");
  });

  Serial.println("[Server] Rutas configuradas");
}

bool conectarWiFi()
{
  wm.setAPCallback(configModeCallback);
  wm.setWebServerCallback(bindServerCallback);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
  wm.setConnectTimeout(WIFI_AP_CONNECT_TIMEOUT);

  mostrarEstado("Buscando WiFi guardado...", TFT_CYAN);
  String savedSSID, savedPass;
  bool credencialesPrevias = cargarCredenciales(savedSSID, savedPass);

  if (credencialesPrevias)
  {
    Serial.printf("[WiFi] Credenciales encontradas para: %s\n", savedSSID.c_str());
    WiFi.begin(savedSSID.c_str(), savedPass.c_str());
    for (int i = 0; i < WIFI_CONNECT_TIMEOUT; i++)
    {
      if (WiFi.status() == WL_CONNECTED) break;
      delay(500);
    }
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("[WiFi] Credenciales invalidas, borrando...");
      borrarCredenciales();
    }
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("[WiFi] Sin credenciales validas - Abriendo portal...");
    wm.autoConnect(WIFI_AP_NAME);
  }

  return WiFi.status() == WL_CONNECTED;
}
