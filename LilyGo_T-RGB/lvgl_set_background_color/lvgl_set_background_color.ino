// This example program shows you how to set a background color of a screen using LVGL. I use version 8.3 recommended by LilyGo for my screen

#include <LilyGo_RGBPanel.h>
#include <LV_Helper.h>
LilyGo_RGBPanel panel;

void setup() {
  if (!panel.begin()) {
        while (1) {
            Serial.println("Error, failed to initialize T-RGB"); delay(1000);
        }
    }
    if (!panel.installSD()) {
        Serial.println("Can't install SD Card!");
    }
    beginLvglHelper(panel);
    static lv_style_t style; // creating style object
    lv_style_init(&style); // style initialization
    lv_style_set_bg_color(&style, lv_color_hex(0x363636)); // Installing dark grey color as a background of a style
    lv_obj_add_style(lv_scr_act(), &style, LV_PART_MAIN); // Setting a style
    lv_task_handler();
    panel.setBrightness(8);
}

void loop() {
  lv_task_handler();
  delay(100);
}
