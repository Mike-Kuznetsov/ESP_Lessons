/*
  Теперь вместо пароля от почты аккаунта Mail.ru необходимо использовать специальный токен, 
  который можно получить в меню "Пароль и безопасность" -> "Пароли для внешних приложений"
  Работает без SSL шифрования.
  This script doesn't use SSL encryption.
  -----
  Заметки ESPшника - Урок 2 - SMTP-клиент. Отправляем сообщения на почту.
  Mautoz Tech https://www.youtube.com/c/MautozTech
  Заметки ESPшника - https://www.youtube.com/@ESPdev
*/
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include "Mail.h"

const char* const STA_SSID = "WIFI_NAME";
const char* const STA_Pass = "WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print(F("Connecting to Wi-Fi"));
  Serial.print('"');
  WiFi.begin(STA_SSID, STA_Pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.print(' ');
  Serial.println(WiFi.localIP());
  if (sendMail("smtp.mail.ru", 465, "ADDRESS@mail.ru", "PASSWORD", "RECEIVER@gmail.com", "TITLE", "MESSAGE")) {
    Serial.println("Mail sended");
   } else {
    Serial.println("Error");
   }
  WiFi.disconnect();
}

void loop() {
}
