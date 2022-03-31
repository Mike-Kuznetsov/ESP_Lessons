/*
  Заметки ESPшника - Урок 4 - Файловая система. Вывод картинки по HTTP.
  Загрузка картинки из памяти ESP и передача по HTTP.
  Code adapted from here https://randomnerdtutorials.com/display-images-esp32-esp8266-web-server/
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
  Github - https://github.com/Mike-Kuznetsov
*/
/*
  Для работы необходимо установить:
    Для ESP32:
      https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
      https://github.com/me-no-dev/ESPAsyncWebServer
      https://github.com/me-no-dev/AsyncTCP
    Для ESP8266:
      https://github.com/esp8266/arduino-esp8266fs-plugin/releases
      https://github.com/me-no-dev/ESPAsyncTCP
      https://github.com/me-no-dev/ESPAsyncWebServer 
*/

#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "FS.h"
#endif

const char* ssid = "Wi-Fi";
const char* password = "";

AsyncWebServer server(80); // 80 - port

const char html = "<img src='img'>";

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", html);
  });
  server.on("/img", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/nodemcu.png", "image/png");
  });
  server.begin();
}
void loop(){
}
