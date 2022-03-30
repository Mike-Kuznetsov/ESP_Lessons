/*
  Сreated 4 Sep 2010
  by Michael Margolis
  modified 9 Apr 2012
  by Tom Igoe
  updated for the ESP8266 12 Apr 2015
  by Ivan Grokhotkov
  This code is in the public domain.
  -----
  Заметки ESPшника - Урок 3 - NTP-клиент - Версия 2
  Код показывает время при нажатии на кнопку, кнопка подключена к пинам D2 и GND.
  Code from ESP8266 examples. Adapted by Mautoz Tech.  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
  Github - https://github.com/Mike-Kuznetsov
*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#ifndef STASSID
#define STASSID "Wi-Fi"
#define STAPSK  "PASSWORD"
#endif
int day, year;
uint64_t syncTime;
uint8_t weekday, month, monthLength, seconds, hours, minutes;
uint8_t monthDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};
const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password
unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
unsigned long epochSaved;
WiFiUDP udp;
uint8_t button1 = 4;

void setup() {
  pinMode(button1, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  while(updateTime()){
    Serial.println("Error: Not synchonized");
  }
}

void loop() {
  if (digitalRead(button1)==0){
    calculate();
    while(digitalRead(button1)==0){
      delay(100);
    }
  }
}

bool updateTime(){
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb) {
    //Serial.println("no packet yet");
  } else {
    //Serial.print("packet received, length=");
    //Serial.println(cb);
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = ");
    //Serial.println(secsSince1900);
    //Serial.print("Unix time = ");
    const unsigned long seventyYears = 2208988800UL;
    epochSaved = secsSince1900 - seventyYears+10800; // In unix format time starts at 1970, not 1900
    syncTime=millis();
    return 0;
  }
  return 1;
}
void calculate(){    
    unsigned long epoch=epochSaved+(millis()-syncTime)/1000;
    //Serial.println(epoch);
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // hour
    hours=(epoch  % 86400L) / 3600;
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60);  // minute
    minutes=(epoch  % 3600) / 60;
    Serial.print(':');
    if ((epoch % 60) < 10) {
      Serial.print('0');
    }
    Serial.println(epoch % 60); // second
    seconds=epoch % 60;
    day=epoch/(86400);
    weekday = (day+3)%7 + 1;
    //Serial.println(String(weekday));
    year=1970+day/1461*4;
    day%=1461;
    day++;
    short visokosny=0;
    if (day>365){
      year++;
      day-=365;
      visokosny++;
    }
    if (day>365){
      year++;
      day-=365;
      visokosny++;
    }
    if (day>366){
      year++;
      day-=366;
      visokosny++;   
    }
    for (month=0; month<12; month++) {
      if (month==1 && visokosny==2) monthLength=29;
      else monthLength = monthDays[month];
      if (day>monthLength) day-=monthLength;
      else break;
    }
    month++;
    Serial.print("Date: ");
    Serial.print(String(day));
    Serial.print(".");
    Serial.print(String(month));
    Serial.print(".");
    Serial.println(String(year));
}

void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
