/*
  This code for ESP32 asks api.ipify.org for your public IP address, receives it and prints it in serial terminal every 30 seconds.
  I have seen the same code for ESP8266 on github. Here's the link: https://gist.github.com/worthapenny/a343ed3e7767e18a318e1cb7fa2d6c30
  
  Этот код для ESP32 спрашивает ваш публичный IP адрес у api.ipify.org (аналог 2ip.ru), получает его и выводит в терминал раз в 30 секунд.
  Я видел такой же код для ESP8266. Вот ссылка: https://gist.github.com/worthapenny/a343ed3e7767e18a318e1cb7fa2d6c30
  
  Mautoz Tech https://www.youtube.com/MautozTech
  Заметки ESPшника - https://www.youtube.com/@ESPdev
*/  
#include <WiFi.h>

const char* ssid     = "Wi-Fi Name";
const char* password = "PASSWORD";
const char* host = "api.ipify.org";

void setup(){
    Serial.begin(115200);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("Local IP address: ");
    Serial.println(WiFi.localIP());
}

void loop(){
    WiFiClient client;
    while (!client.connect(host, 80)) {
        Serial.println("connection failed");
        delay(5000);
    }
    client.print(String("GET / HTTP/1.1\r\nHost: ") + host + "\r\nConnection: close\r\n\r\n"); // String(
    unsigned long timeout = millis();
    while (client.available() == 0 and millis() - timeout < 5000) {}
    String line = "";
    while(client.available()){
        line = client.readStringUntil('\r'); 
    }
    Serial.println("IP address: " + line.substring(1));
    delay(30000);
}
