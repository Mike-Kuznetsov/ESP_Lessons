/*
  This code for ESP8266/ESP32 creates Wi-Fi Access Point
  Этот код для ESP8266/ESP32 создает точку доступа Wi-Fi
  
  Mautoz Tech - https://youtube.com/c/MautozTech
  Заметки ESPшника - https://youtube.com/@espdev
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

const char *ssid = "ESP_AP";
const char *password = "12345678";

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
