#include "TVout.h"
#include "fontALL.h"
TVout TV;

void setup() 
 {
  TV.begin(NTSC,128,64); 
  TV.select_font(font8x8); 
 }
 
void loop()
 {    
      TV.set_cursor(20,10);
      TV.print("Hello world");
      TV.set_cursor(20,20);
      TV.print("Next line");
      delay(1000);
      //TV.clear();N
 }
