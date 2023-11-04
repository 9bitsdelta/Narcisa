#include <LiquidCrystal.h>

// PINS
enum { RS = 2, EN, D4, D5, D6, D7 };

// GLOBAL VARS
LiquidCrystal g_LCD(RS, EN, D4, D5, D6, D7);

void setup() {
  g_LCD.begin(16, 2);
  g_LCD.setCursor(0, 0);
  g_LCD.print("Hello World!");
}

void loop() {
  
}
