/*
  You can use function wait() in your programms instead of delay(). This function doesn't freeze your program.
  Вы можете использовать эту функцию wait() в своих программах вместо delay(). Она не останавливает работу ESP или Arduino.
  
  Written by Mautoz Tech.  
  Mautoz Tech https://www.youtube.com/channel/UCWN_KT_CAjGZQG5grHldL8w
  Заметки ESPшника - https://www.youtube.com/channel/UCQAbEIaWFdARXKqcufV6y_g
  Github - https://github.com/Mike-Kuznetsov
*/

void wait(int waitingTime){
  int waitingStartTime = millis();
  while (millis() - waitingStartTime < waitingTime){
    // You can put your code here. It'll be executed even if other program is waiting
    // Вы можете вставлять свой код сюда, если хотите чтобы он выполнялся даже когда остальная программа в режиме ожидания.
  }
}


// put your setup code here, to run once:
void setup() {
  
}

// put your main code here, to run repeatedly:
void loop() {
  wait(5000); // Starts waiting for 5 seconds
}
