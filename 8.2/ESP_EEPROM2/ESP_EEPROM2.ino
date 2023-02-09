#include <ESP_EEPROM.h>

void setup() {
//Initialization
  Serial.begin(9600);
  Serial.println();
  EEPROM.begin(16);

//Reading
  int newVariable;
  EEPROM.get(0, newVariable);
  Serial.print("Saved variable 1: ");
  Serial.println(newVariable);

//Writing
  int int2 = 123;
  EEPROM.put(4, int2);
  EEPROM.commit();

//Reading
  EEPROM.get(4, newVariable);
  Serial.print("Saved variable 2: ");
  Serial.println(newVariable);
}

void loop() {
  delay(1000);
}
