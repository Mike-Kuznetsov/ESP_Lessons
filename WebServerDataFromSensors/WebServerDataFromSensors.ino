/*
  When you press the physical button connected to ESP, button state on the Webpage will be changed to ON.
  You can send data from any sensor to Webpage, not only button.
  Button  is connected between PIN D3 and GND.
  При нажатии на физическую кнопку подключенную к ESP, состояние кнопки на Веб-странице изменится на "Включено".
  Таким образом можно передавать данные с датчиков подключенных к ESP на веб-страницу.
  Кнопка подключена между пинами D3 и GND. 
  
  Заметки ESPшника - Урок 5 - Веб-сервер.
  By Mautoz Tech.  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
*/
#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  WebServer server(80);
#else
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif
uint8_t button = 0; // Button  is connected between PIN D3 and GND

const char* ssid     = "Wi-Fi";
const char* password = "Password";

void handleRoot()
{
  String webpage;
  if(digitalRead(button)==HIGH){
    webpage = "Button: OFF";
  }
  else{
    webpage = "Button: ON";
  }
  webpage = "<html><meta http-equiv=\"refresh\" content=\"2\"> <body>"
                + webpage + "</body></html>";
  server.send(200, "text/html", webpage);
}

void setup()
{
  Serial.begin(115200);
  pinMode(button, INPUT);
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); 
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  server.handleClient();
}
