/*
  This code for ESP creates Wi-Fi Access Point
  Этот код для ESP создает точку доступа Wi-Fi
  Заметки ESPшника.
  By Mautoz Tech.  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
*/

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  #include <WebServer.h>
  WebServer server(80);
#else
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif

#ifndef APSSID
#define APSSID "ESP_AP"
#define APPSK  "PASSWORD"
#endif

const char *ssid = APSSID;
const char *password = APPSK;

void handleRoot() {
  server.send(200, "text/html", "Web server works");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Starting access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
