/*
  This code for ESP writes and reads variables using ESP flash permanent memory. You need to have ESP_EEPROM library installed.
  Этот код для ESP записывает и читает переменные используя постоянную память ESP. Вам необходима библиотека ESP_EEPROM.
  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
*/  

#include <ESP_EEPROM.h>

struct MyStruct{
  int     anInteger;
  float   aFloating;
  int     anotherInteger;
  boolean state;
} eeprom;

void setup() {
  Serial.begin(9600);
  EEPROM.begin(sizeof(MyStruct));
  if(EEPROM.percentUsed()>=0){
    EEPROM.get(0, eeprom);
    Serial.println(eeprom.anInteger);
    Serial.println(eeprom.aFloating);
    Serial.println(eeprom.anotherInteger);
    Serial.println(eeprom.state);
  } 
  else{
    Serial.println("EEPROM size have been changed or there are no stored data in memory");    
  }
  eeprom.anInteger = 37;
  eeprom.aFloating = 9.21;
  eeprom.anotherInteger = 42;
  eeprom.state = true;
  EEPROM.put(0, eeprom);
  EEPROM.commit();

  //boolean ok = EEPROM.commit();
  //Serial.println((ok) ? "Commit OK" : "Commit failed");
  
}

void loop() {
  delay(1000);
}
