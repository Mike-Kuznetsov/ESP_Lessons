/*
  This program is an example of how to change IP address of the ESP from a web browser.
  Программа является примером того как менять IP-адрес ESP из веб-браузера.
  
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
            <h1>IP address</h1>\
            <input type='text' value='192' id='ip1v' size=3 autofocus>.\
            <input type='text' value='168' id='ip2v' size=3 autofocus>.\
            <input type='text' value='1' id='ip3v' size=3 autofocus>.\
            <input type='text' value='247' id='ip4v' size=3 autofocus>\
            <div>\
            <br><button id=\"save_button\">Set IP address</button>\
            </div>\
      <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\    
      <script>\
        $('#save_button').click(function(e){\
          var ip1;\
          var ip2;\
          var ip3;\
          var ip4;\
          e.preventDefault();\
          ip1 = $('#ip1v').val();\
          ip2 = $('#ip2v').val();\
          ip3 = $('#ip3v').val();\        
          ip4 = $('#ip4v').val();\ 
          $.get('/save?ip1=' + ip1 + '&ip2=' + ip2 + '&ip3=' + ip3+'&ip4=' + ip4, function(data){\
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
  if (server.args()==4){
    Serial.println("New IP: " + server.arg("ip1")+"."+server.arg("ip2")+"."+server.arg("ip3")+"."+server.arg("ip4"));
    if ((server.arg("ip1").toInt()>0) &&(server.arg("ip1").toInt()<255)&&(server.arg("ip2").toInt()>=0) &&(server.arg("ip2").toInt()<255)&&(server.arg("ip3").toInt()>=0) &&(server.arg("ip3").toInt()<255)&&(server.arg("ip4").toInt()>=0) &&(server.arg("ip4").toInt()<255)){
      Serial.println("New IP: " + server.arg("ip1")+"."+server.arg("ip2")+"."+server.arg("ip3")+"."+server.arg("ip4"));
      IPAddress ip(server.arg("ip1").toInt(),server.arg("ip2").toInt(),server.arg("ip3").toInt(),server.arg("ip4").toInt());
      IPAddress gateway(192,168,1,1);
      IPAddress subnet(255,255,255,0);
      WiFi.begin(ssid, password);
      WiFi.config(ip, gateway, subnet);
      Serial.println("");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to "); 
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }  
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
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
