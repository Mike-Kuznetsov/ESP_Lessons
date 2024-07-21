/*
 * This program reads temperature & pressure from BMP280 via Grove connector (i2c mode) and sends data to a PC via serial port (USB)
 * Эта программа читает температуру и давление с BMP280 через Grove-разъём (i2c) и отправляет их на компьютер через serial port (USB)
 * 
 * It requires Adafruit_BMP280 library and all the libraries that are required by LilyGo T-RGB library.
 * Она требует наличия библиотеки Adafruit_BMP280 и всех библиотек необходимых для LilyGo T-RGB
 * LilyGo T-RGB repository: https://github.com/Xinyuan-LilyGO/LilyGo-T-RGB
 * 
 * Mautoz Tech - https://www.youtube.com/c/MautozTech
 * Заметки ESPшника - https://www.youtube.com/@espdev
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(9600);
  Wire.begin(8, 48);
  unsigned status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    while (1) delay(10);
  }
}
void loop() {
    Serial.println("Temperature: "+String(bmp.readTemperature()) + " *C");
    Serial.println("Pressure: "+String(bmp.readPressure()) + " *Pa");
    Serial.println("Approx altitude: "+String(bmp.readAltitude(1013.25)) + " m");
    Serial.println();
    delay(2000);
}
