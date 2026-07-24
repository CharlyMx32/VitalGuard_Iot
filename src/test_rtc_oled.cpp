#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

bool oledOK = false;
bool rtcOK = false;

void mostrarOLED(int linea, const char *texto)
{
  if (!oledOK) return;
  oled.setCursor(0, linea * 10);
  oled.println(texto);
  oled.display();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== TEST RTC + OLED ===");

  Wire.begin(SDA_PIN, SCL_PIN);

  oledOK = oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (oledOK)
  {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    mostrarOLED(0, "VitalGuard TEST");
    mostrarOLED(1, "----------------");
    mostrarOLED(2, "Init I2C... OK");
  }

  rtcOK = rtc.begin();
  if (rtcOK)
  {
    Serial.println("[RTC] OK");
    if (oledOK) mostrarOLED(3, "RTC: OK");
    if (rtc.lostPower())
    {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      if (oledOK) mostrarOLED(4, "RTC: Ajustado");
    }
  }
  else
  {
    Serial.println("[RTC] No encontrado");
    if (oledOK) mostrarOLED(3, "RTC: NO ENCONTRADO");
    if (oledOK) mostrarOLED(4, "Conecta RTC y resetea");
  }

  if (oledOK) mostrarOLED(5, "----------------");
  if (oledOK) mostrarOLED(6, "Esperando datos...");

  Serial.println("Listo!");
}

void loop()
{
  if (!rtcOK)
  {
    if (oledOK)
    {
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setTextColor(SSD1306_WHITE);
      mostrarOLED(0, "VitalGuard TEST");
      mostrarOLED(1, "----------------");
      mostrarOLED(3, "RTC: NO ENCONTRADO");
      mostrarOLED(4, "Conecta RTC");
      mostrarOLED(5, "y presiona RESET");
    }
    delay(1000);
    return;
  }

  DateTime now = rtc.now();

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  mostrarOLED(0, "VitalGuard TEST");
  mostrarOLED(1, "----------------");

  char line[22];
  snprintf(line, sizeof(line), "%02d/%02d/%04d", now.day(), now.month(), now.year());
  mostrarOLED(2, line);

  snprintf(line, sizeof(line), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  mostrarOLED(3, line);

  snprintf(line, sizeof(line), "Temp: %.1f C", rtc.getTemperature());
  mostrarOLED(5, line);

  Serial.printf("[RTC] %02d/%02d/%04d %02d:%02d:%02d Temp: %.2f C\n",
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second(),
    rtc.getTemperature());

  delay(1000);
}
