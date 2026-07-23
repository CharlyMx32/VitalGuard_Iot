#pragma once

// ─── PINES ──────────────────────────────────────────
#ifndef TFT_BL
#define TFT_BL 5
#endif

#define BOTON_RESET 0

// ─── MQTT ───────────────────────────────────────────
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "VitalGuard-ESP32"
#define MQTT_TOPIC_PREFIX "vitalguard/"

// ─── WIFI PORTAL ────────────────────────────────────
#define WIFI_AP_NAME "VitalGuard-AP"
#define PORTAL_TIMEOUT 180
#define WIFI_CONNECT_TIMEOUT 30

// ─── PREFERENCES ────────────────────────────────────
#define PREFS_NAMESPACE "vitalguard"
#define PREFS_KEY_SSID "ssid"
#define PREFS_KEY_PASS "pass"

// ─── COLORES TFT ────────────────────────────────────
#define COLOR_HEADER 0x0515
#define COLOR_CARD 0x3186
