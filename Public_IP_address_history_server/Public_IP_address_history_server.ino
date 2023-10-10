/*
  This code for ESP saves your public IP address every hour and shows IP address history in a webpage when you access it from a browser.
  Standard parameters which you can adjust: interval - 1 hour, timezone offset - 3 hours, UTC+3, Moscow time. 
  Also you can uncomment serial.println functions for debugging if something doesn't work.
  You need to have NTPClient library installed. https://github.com/taranais/NTPClient 
  I tested it only with ESP32. Program doesn't use persistent memory storage so it will forget history after restart. Actually i have a tutorial in this repository so you can save history to persistent storage if you want.

  Этот код для ESP сохраняет ваш публичный IP адрес каждый час и выводит историю IP адресов на веб-странице ESP когда вы её запрашиваете из браузера.
  Стандартные параметры, которые можно изменять: интервал - 1 час, часовой пояс - Московский UTC+3.
  Вы можете расскомментировать Serial.println для отладки, если что-то не работает.
  Вам необходимо иметь библиотеку NTPClient. https://github.com/taranais/NTPClient 
  Я тестировал программу только с ESP32. Программа не использует постоянную память, так что она забудет историю после перезапуска ESP. В этом Github репозитории есть урок на тему того как использовать постоянную память, если вам интересно.
  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
*/  
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid     = "Wi-Fi Network Name";
const char* password = "Wi-Fi PASSWORD";
long interval = 3600000; // 1 HOUR in milliseconds.
long timeZoneOffset = 10800; // 3 HOURS in secounds. 1 hour = 3600 seconds. Moscow timezone UTC+3.

const char* host = "api.ipify.org";
String address [100];
WiFiUDP ntpUDP;
WebServer server(80);
NTPClient timeClient(ntpUDP);
String formattedDate;
String html;

void wait(long waitingTime){
  long waitingStartTime = millis();
  while (millis() - waitingStartTime < waitingTime){
    server.handleClient();
  }
}

void setup(){
    //Serial.begin(115200);
    for (int i = 0; i<100; i++){
      address[i] = "0";
    }
    //Serial.print("Connecting to ");
    //Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        //Serial.print(".");
    }
    //Serial.println("");
    //Serial.println("WiFi connected");
    //Serial.println("Local IP address: ");
    //Serial.println(WiFi.localIP());
    timeClient.begin();
    timeClient.setTimeOffset(timeZoneOffset); // 10800 by default. It's 3 hours, Moscow timezone UTC+3.
    server.on("/", [](){
      server.send(200, "text/html", html);
    });
    server.begin();
}

void loop(){
    server.handleClient();
    WiFiClient client;
    while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
    formattedDate = timeClient.getFormattedDate();
    while (!client.connect(host, 80)) {
        //Serial.println("connection failed");
        wait(5000);
    }
    client.print(String("GET / HTTP/1.1\r\nHost: ") + host + "\r\nConnection: close\r\n\r\n"); // String(
    unsigned long timeout = millis();
    while (client.available() == 0 and millis() - timeout < 5000) {}
    String line = "";
    while(client.available()){
        line = client.readStringUntil('\r'); 
    }
    for (int i = 99; i>0; i--){
      address[i] = address[i-1];
    }
    address[0] = formattedDate + " " + line.substring(1);
    //Serial.println("IP address: " + line.substring(1));
    html = "";
    for (int i = 0; i<100; i++){
      //Serial.println(address[i]);
      html += "<h4>" + address[i] + "</h6>";
    }
    wait(interval); //3600000, 1 hour by default
}
