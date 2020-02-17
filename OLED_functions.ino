void updateOLED() {
  display.fillRect(47, 16, 5, 40, BLACK);
  switch (option) {
    case 0x01:
      display.drawBitmap(47, 16, arrow, 5, 5, WHITE);
      break;
    case 0x02:
      display.drawBitmap(47, 24, arrow, 5, 5, WHITE);
      break;
    case 0x03:
      display.drawBitmap(47, 32, arrow, 5, 5, WHITE);
      break;
    case 0x04:
      display.drawBitmap(47, 40, arrow, 5, 5, WHITE);
      break;  
    case 0x05:
      display.drawBitmap(47, 48, arrow, 5, 5, WHITE);
      break;    
  }
  display.display();
  delay(100);
}

void drawModeDI(){
  display.println("DI");
  display.drawCircle(20, 35, 20, WHITE);
  display.drawBitmap(0, 15, smashLogo, 40, 40, WHITE);
  display.drawBitmap(47, 16, arrow, 5, 5, WHITE);
  display.setCursor(55, 15);
  display.println("Control");
  display.setCursor(55, 23);
  display.println("Full L DI");
  display.setCursor(55, 31);
  display.println("Full R DI");
  display.setCursor(55, 39);
  display.println("Set DI");
  display.setCursor(55, 47);
  display.println("Set SDI");
  display.display();
  delay(2000);
}

void drawModeOOS(){
  display.println("OOS");
  display.drawCircle(20, 35, 20, WHITE);
  display.drawBitmap(0, 15, smashLogo, 40, 40, WHITE);
  display.drawBitmap(47, 16, arrow, 5, 5, WHITE);
  display.setCursor(55, 15);
  display.println("Control");
  display.setCursor(55, 23);
  display.println("USmash");
  display.setCursor(55, 31);
  display.println("Up B");
  display.setCursor(55, 39);
  display.println("Nair");
  display.setCursor(55, 47);
  display.println("Grab");
  display.display();
  delay(2000);
}
