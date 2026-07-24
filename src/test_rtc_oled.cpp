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

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== TEST RTC + OLED ===");

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("[OLED] No se encontro en 0x3C");
    while (1);
  }
  Serial.println("[OLED] OK");

  if (!rtc.begin())
  {
    Serial.println("[RTC] No se encontro");
    while (1);
  }
  Serial.println("[RTC] OK");

  if (rtc.lostPower())
  {
    Serial.println("[RTC] Perdio energia - ajustando a fecha de compilacion");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 0);
  oled.println("VitalGuard");
  oled.drawFastHLine(0, 10, 128, SSD1306_WHITE);
  oled.display();

  Serial.println("Listo!");
}

void loop()
{
  DateTime now = rtc.now();

  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 0);
  oled.println("VitalGuard");
  oled.drawFastHLine(0, 10, 128, SSD1306_WHITE);

  oled.setCursor(10, 20);
  oled.print("Fecha: ");
  oled.print(now.day());
  oled.print("/");
  oled.print(now.month());
  oled.print("/");
  oled.println(now.year());

  oled.setCursor(10, 35);
  oled.print("Hora:  ");
  oled.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  oled.setCursor(10, 50);
  oled.print("Temp:  ");
  oled.print(rtc.getTemperature());
  oled.print(" C");

  oled.display();

  Serial.printf("[RTC] %02d/%02d/%04d %02d:%02d:%02d Temp: %.2f C\n",
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second(),
    rtc.getTemperature());

  delay(1000);
}
