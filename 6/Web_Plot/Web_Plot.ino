tr/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  Default DHT pin is D4 (GPIO2)
*********/
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
  #include <FS.h>
#endif
#include "DHT.h"
#define DHTPIN 2
DHT dht(DHTPIN, DHT11);

const char* ssid = "Wi-Fi";
const char* password = "Password";

AsyncWebServer server(80);
String temperature;
String humidity;

/*String temp(){
  return temperature;
}
String hum(){
  return humidity;
}*/

void setup(){
  Serial.begin(115200);
  dht.begin();
  if (!SPIFFS.begin ()) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  //temperature=String(dht.readTemperature());
  //humidity=String(dht.readHumidity());
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(dht.readTemperature()).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(dht.readHumidity()).c_str());
  });
  server.begin();
}
 
void loop(){
  temperature=String(dht.readTemperature());
  humidity=String(dht.readHumidity());
  delay(50);
}
