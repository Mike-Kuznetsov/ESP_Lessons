/*
  This code for ESP writes and reads variables using ESP flash permanent memory
  Этот код для ESP записывает и читает переменные используя постоянную память ESP

  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
*/  

#include <ESP_EEPROM.h>

void setup() {
//Initialization
  Serial.begin(9600);
  Serial.println();
  EEPROM.begin(16);

//Writing
  int int1 = 123;
  EEPROM.put(0, int1);
  EEPROM.commit();

//Reading
  int newVariable;
  EEPROM.get(0, newVariable);
  Serial.print("Saved variable 1: ");
  Serial.println(newVariable);

//Writing
  int int2 = 456;  
  EEPROM.put(0, int2);
  EEPROM.commit();

//Reading
  EEPROM.get(0, newVariable);
  Serial.print("Saved variable 2: ");
  Serial.println(newVariable);
}

void loop() {
  delay(1000);
}
