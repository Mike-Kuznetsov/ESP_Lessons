/*
  Program sends text via ESP to Serial Port.
  Программа отправляет текст на ESP, а ESP отправляет его на монитор серийного порта.
  
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
const char* ssid     = "Wi-Fi";
const char* password = "Password";

char htmlResponse[3000];

void handleRoot() {
  snprintf ( htmlResponse, 3000,
  "<!DOCTYPE html>\
  <html lang=\"en\">\
    <head>\
      <meta charset=\"utf-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
    </head>\
    <body>\
            <h1>Screen message</h1>\
            <input type='text' id='scrmsg' size=3 autofocus>.\
            <div>\
            <br><button id=\"send_button\">Send message</button>\
            </div>\
      <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\    
      <script>\
        $('#send_button').click(function(e){\
          e.preventDefault();\
          var msg;\
          msg = $('#scrmsg').val();\
          $.get('/save?msg=' + msg, function(data){\
            console.log(data);\
          });\
        });\  
      </script>\
    </body>\
  </html>"); 
  server.send ( 200, "text/html", htmlResponse );  
  Serial.println("HANDLING ROOT");
}

void handleSave() {
  //Serial.println(server.args());
  if (server.args()==1){
    if (server.arg("msg")!=""){
      Serial.println("Message recieved:");
      Serial.println(server.arg("msg"));
    }
  }  
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Start serial
  Serial.begin(115200);
  delay(10);
  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on ( "/", handleRoot );
  server.on ("/save", handleSave);
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
}
