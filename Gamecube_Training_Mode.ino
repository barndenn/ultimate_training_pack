#include "Nintendo.h"
#include <SPI.h>
#include <Wire.h>

// Define a Gamecube Controller, Console, and controller data 
CGamecubeController GamecubeController(2);
CGamecubeConsole GamecubeConsole(3);
Gamecube_Data_t d = defaultGamecubeData;
// Pin definitions
#define pinLed LED_BUILTIN

void setup() {
  // Set up debug led
  pinMode(pinLed, OUTPUT);

  // Start debug serial
  Serial.begin(115200);
}

void loop() {
  GamecubeController.read();
  // Get data of controller
  auto r1 = GamecubeController.getReport();
  //Reporting all buttons, sticks, sliders

  d.report.a = r1.a;
  d.report.b = r1.b;
  //Changes X to report Z button
  d.report.x = r1.x;
  d.report.y = r1.y;
  d.report.start = r1.start;
  
  d.report.dleft = r1.dleft;
  d.report.dright = r1.dright;
  d.report.ddown = r1.ddown;
  d.report.dup = r1.dup;

  d.report.z = r1.z;
  d.report.r = r1.r;
  d.report.l = r1.l;
 
  d.report.yAxis = r1.yAxis;
  d.report.cxAxis = r1.cxAxis;
  d.report.cyAxis = r1.cyAxis;
  d.report.left = r1.left;
  d.report.right = r1.right;

  d.report.xAxis = 255; 
  for (int i=0;i<16;i++){GamecubeConsole.write(d);}
  d.report.xAxis = 0; 
  for (int i=0;i<16;i++){GamecubeConsole.write(d);}
    
  // Enable rumble
  if (d.status.rumble) {
    GamecubeController.setRumble(true);
  }
  else {
    GamecubeController.setRumble(false);
  }
}
