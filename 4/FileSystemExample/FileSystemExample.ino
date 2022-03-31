/*
  Заметки ESPшника - Урок 4 - Файловая система. Вывод картинки по HTTP.
  Программа выводит имена и размеры всех файлов в файловой системе ESP, а также содержимое файла kek.txt
  Для работы необходимо установить https://github.com/esp8266/arduino-esp8266fs-plugin/releases или https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
  Written by Mautoz Tech.  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
  Github - https://github.com/Mike-Kuznetsov
*/
#include "FS.h"

void setup() {
  Serial.begin (115200);
  if (!SPIFFS.begin ()) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println();
  Serial.println("FILES:");
  Dir dir = SPIFFS.openDir ("");
  while (dir.next ()) {
    Serial.println (dir.fileName ());
    Serial.print ("Size: ");
    Serial.println (dir.fileSize ());
  }
  File f = SPIFFS.open("/kek.txt", "r");
  if (!f) {
    Serial.println("file open failed");
  }
  else{
    Serial.print("Content of kek.txt: ");
    while(f.available()){
      Serial.write(f.read());
    }
    f.close();
  }
}

void loop() {
}
