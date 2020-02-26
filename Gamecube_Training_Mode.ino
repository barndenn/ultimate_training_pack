#include "Nintendo.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Dimensions
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Smash logo 
const unsigned char smashLogo [] PROGMEM = {
  // 'smashlogo, 40x40px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00,
  0x7f, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff,
  0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80,
  0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00,
  0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
  0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00,
  0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff,
  0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff,
  0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0x80, 0x00,
  0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00,
  0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00,
  0xff, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x0f,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char arrow [] PROGMEM = {
  // 'arrow, 5x5px
  0x20, 0x10, 0xf8, 0x10, 0x20
};

// Define a Gamecube Controller, Console, and controller data 
CGamecubeController GamecubeController(2);
CGamecubeConsole GamecubeConsole(3);
Gamecube_Data_t d = defaultGamecubeData;
// Pin definitions
#define pinLed LED_BUILTIN

// State Variables
uint8_t option;    // option range: 1 - 5
uint8_t mode;      // DI = 0x00 | OOS = 0x01 
uint8_t state = 0x01;  // Initialize Control Mode 
uint8_t saveState;
uint8_t OLEDFlag;

void setup()
{
  // Set Defaults
  option = 0x01;
  mode = 0x00;
  
  // Set up debug led
  pinMode(pinLed, OUTPUT);

  // Start debug serial
  Serial.begin(115200);
  
  // Mode Init | Hold down start button to enter OOS mode, default DI mode
  GamecubeController.read(); 
  auto report = GamecubeController.getReport(); 
  if (report.start == true){
    mode = 0x01;
  } else if (report.start == false){
    mode = 0x00;
  }
    
  // Setup Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Training Mode: ");
 
  if (mode == 0x00){
    drawModeDI();
  } else if (mode == 0x01) {
    drawModeOOS();
  }
}

void loop(){
  // Read controller data 
  GamecubeController.read();
  
  // Determines mode 
  switch (state){
    case 0x00:
      updateOLED();
      OLEDFlag = 0x00;
      state = saveState;
      break;
    case 0x01:
      control(); // Control Mode
      break;
    case 0x02:
      //randomLRDI();
      break;
    case 0x03:
      //setDI();
      break;
    case 0x04:
      //randomSDI();
      break;
    case 0x05:
      //setSDI();
      break;
  }
  digitalWrite(pinLed, LOW);
}

void control(){
    // Get data of controller
    auto r1 = GamecubeController.getReport();
    //Reporting all buttons, sticks, sliders

    d.report.xAxis = 255; 
    for (int i=0;i<1;i++){GamecubeConsole.write(d);}
    d.report.xAxis = 0; 
    for (int i=0;i<3;i++){GamecubeConsole.write(d);}
    
    
//    d.report.a = r1.a;
//    d.report.b = r1.b;
//    //Changes X to report Z button
//    d.report.x = r1.x;
//    d.report.y = r1.y;
//    d.report.start = r1.start;
//    
//    d.report.dleft = r1.dleft;
//    d.report.dright = r1.dright;
//    d.report.ddown = r1.ddown;
//    d.report.dup = r1.dup;
//
//    d.report.z = r1.z;
//    d.report.r = r1.r;
//    d.report.l = r1.l;
//    
//    d.report.xAxis = r1.xAxis;
//    d.report.yAxis = r1.yAxis;
//    d.report.cxAxis = r1.cxAxis;
//    d.report.cyAxis = r1.cyAxis;
//    d.report.left = r1.left;
//    d.report.right = r1.right;
//  
//    // Mirror the controller data to the console  
//    GamecubeConsole.write(d);
    
    // Enable rumble
    if (d.status.rumble) {
      GamecubeController.setRumble(true);
    }
    else {
      GamecubeController.setRumble(false);
    }
  
    // Update Option and State 
    if (r1.dup) {
    option--;
    state--;
    OLEDFlag = 1;
      if (option < 0x01) {
        option = 0x05;
        state = 0x05;
      } 
    }
    else if (r1.ddown) {
      option++;
      state++;
      OLEDFlag = 0x01;
      if (option > 0x05) {
        option = 0x01;
        state = 0x01;
      } 
    }
  
    switch (OLEDFlag){
      case 0x00: 
        break;
      case 0x01:
        saveState = state;
        state = 0x00;
        break;
    }
  }
