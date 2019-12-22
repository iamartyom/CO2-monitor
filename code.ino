#include <MHZ19.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define BAUDRATE 9600

#define INITIALIZE_DELAY 30000
#define LOOP_DELAY 500

#define RX_PORT 6
#define TX_PORT 7

#define MAX_GOOD_CO2_QUALITY 800
#define MAX_MEDIUM_CO2_QUALITY 1100

SoftwareSerial serial(RX_PORT, TX_PORT);
MHZ19 mhz19;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String lastStatusCO2;
String currentStatusCO2;

int lastCO2Value = 0;
int currentCO2Value = 0;

void setup() {
  Serial.begin(BAUDRATE);
  
  setupMhz19();
  setupLcd();
  initializeMhz19();
}

void setupMhz19() {
  serial.begin(BAUDRATE);
  mhz19.begin(serial);
  mhz19.autoCalibration(false);
}

void setupLcd() {
  lcd.begin(16, 2);
}

void initializeMhz19() {
  lcd.print("INITIALIZE");
  delay(INITIALIZE_DELAY);
}

void loop() {
  currentCO2Value = mhz19.getCO2(false);

  if (lastCO2Value != currentCO2Value) {
    lastCO2Value = currentCO2Value;
    printCO2Information();
  }

  delay(LOOP_DELAY);
}

void printCO2Information() {
  printStatusCO2();
  printCO2PPM();
}

void printStatusCO2() {
  currentStatusCO2 = getCO2Level();

  if (!lastStatusCO2.equals(currentStatusCO2)) {
    lastStatusCO2 = currentStatusCO2;
    clearLcdLine(0);
    lcd.setCursor(0, 0);
    lcd.print(currentStatusCO2);
  }
}

String getCO2Level() {
  if (currentCO2Value <= MAX_GOOD_CO2_QUALITY) {
    return "GOOD QUALITY";
  } else if (currentCO2Value <= MAX_MEDIUM_CO2_QUALITY) {
    return "MEDIUM QUALITY";
  } else {
    return "BAD QUALITY";
  }
}

void printCO2PPM() {
  clearLcdLine(1);
  lcd.setCursor(0, 1);
  lcd.print("CO2 - ");
  lcd.print(currentCO2Value);
  lcd.print(" PPM");
}

void clearLcdLine(byte indexLine) {
  lcd.setCursor(0, indexLine);
  lcd.print("                ");
}
