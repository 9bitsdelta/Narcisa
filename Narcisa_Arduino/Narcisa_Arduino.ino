#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// PINS
enum { RX = 10, TX};
enum { RS = 4, EN, D4, D5, D6, D7 };

// GLOBAL VARS

LiquidCrystal g_LCD(RS, EN, D4, D5, D6, D7); // Initialize LCD object
SoftwareSerial g_BT(RX, TX); // Initialize software UART object

char g_Disp[4][17]; // 4 lines of 16 characters +1 null terminator
String g_Buff; // Plain text buffer

uint8_t g_NumStr; // Number of non empty lines found in g_Disp
uint8_t g_Scroll; // Line of g_Disp from which the display will start printing

void PrintLines(char msg0[17], char msg1[17]); // Print two 16 character lines to the display
void ConvertBuffer(char dst[4][17], const char* buff); // Convert a string to the display format

void setup() {
  Serial.begin(9600);
  g_BT.begin(9600);

  g_LCD.begin(16, 2);
  g_LCD.clear();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  g_NumStr = 0;
  g_Scroll = 0;
  
  g_Buff.reserve(64);

  // Configure name of bluetooth module
  g_BT.print("AT+NAMENBT04-E\r\n");
  delay(2000);
  // Receive and display Name and OK
  if(g_BT.available()) {
    g_Buff = g_BT.readStringUntil('\n');
    g_Buff.trim();
    //Serial.println(g_Buff);
    ConvertBuffer(g_Disp, g_Buff.c_str());
    PrintLines(g_Disp[0], g_Disp[1]);
  }
  else {
    ConvertBuffer(g_Disp, "No response from BT module");
    PrintLines(g_Disp[0], g_Disp[1]);
  }
  
}

void loop() {
  
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  
  if(g_BT.available()) {
    g_Buff = g_BT.readStringUntil('\n');
    if(g_Buff.length() > 63) g_Buff.remove(63);
    g_Buff.trim();

    int8_t found = g_Buff.indexOf("ETTI");
    if(found > -1) {
      Serial.println("-- Found ETTI");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    
    Serial.println("-- Buffer: ");
    Serial.flush();
    Serial.println(g_Buff);
    
    ConvertBuffer(g_Disp, g_Buff.c_str());
    Serial.println("-- Formatted message:");
    Serial.flush();
    Serial.println(g_Disp[0]);
    Serial.println(g_Disp[1]);
    Serial.println(g_Disp[2]);
    Serial.println(g_Disp[3]);
    Serial.flush();
    
    g_NumStr = 0;
    g_Scroll = 0;
    if(strlen(g_Disp[0])!=0) g_NumStr++;
    if(strlen(g_Disp[1])!=0) g_NumStr++;
    if(strlen(g_Disp[2])!=0) g_NumStr++;
    if(strlen(g_Disp[3])!=0) g_NumStr++;
    
    Serial.println(g_NumStr);
    
    PrintLines(g_Disp[0], g_Disp[1]);
  }
  else if (g_NumStr > 2) {
    Serial.println("Scrolling");
    if(g_Scroll < 2) g_Scroll++;
    else g_Scroll = 0;
    PrintLines(g_Disp[g_Scroll], g_Disp[g_Scroll + 1]); 
  }
  else { }
}

void PrintLines(char msg0[17], char msg1[17]) {
  g_LCD.clear();
  g_LCD.print(msg0);
  g_LCD.setCursor(0, 1);
  g_LCD.print(msg1);
  Serial.println("-- LCD display:");
  Serial.println(msg0);
  Serial.println(msg1);
  Serial.flush();
}

void ConvertBuffer(char dst[4][17], const char* buff) {
  //memset(dst[0], 0, 17);
  //memset(dst[1], 0, 17);
  static char inter[64];
  memset(inter, 0, 64);
  strncpy(inter, buff, 63);
  strncpy(dst[0], inter   , 16);
  strncpy(dst[1], inter+16, 16);
  strncpy(dst[2], inter+32, 16);
  strncpy(dst[3], inter+48, 16);
}
