#include <ESP8266WiFi.h>
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
