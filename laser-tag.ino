#include <IRremote.h> //IRremote by Arduino-IRremote
#include <LiquidCrystal_I2C.h> //LiquidCrystal_I2C by John Rickman
#include "pitches.h" //pitches.h by Mike Putnam

int IRPin = 9;
int IREPin = 10;
int buttonPin = 12;
int buzzPin = 13;
int health = 3;
bool firstDead = true;
bool hasBeenOff = false;
unsigned long previousMillis = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2); //pins 20 & 21

void setup() {
  Serial.begin(9600); 

  IrReceiver.begin(IRPin, DISABLE_LED_FEEDBACK);
  IrSender.begin(IREPin, DISABLE_LED_FEEDBACK);
  
  pinMode(buttonPin, INPUT_PULLUP);
  
  lcd.init();                     
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("PLAYER #"); //Replace # with player number
  lcd.setCursor(6,1); 
  lcd.print("HP ");
  lcd.print(health);
}

void loop() {
  if (IrReceiver.decode() && health) {
    tone(buzzPin, NOTE_F3, 200);
    health--;
    lcd.setCursor(6,1);
    lcd.print("HP ");
    lcd.print(health);
    IrReceiver.resume();
  }
  if (digitalRead(buttonPin)) {
    hasBeenOff = true;
  }
  if (!digitalRead(buttonPin) && health && millis() - previousMillis >= 500 && hasBeenOff) {
    tone(buzzPin, NOTE_A4, 200);
    IrSender.sendNEC(0, 0x19, 0);
    hasBeenOff = false;
    previousMillis = millis();
  }
  if (!health && firstDead) {
    tone(buzzPin, NOTE_F3, 2000);
    lcd.setCursor(4,1); 
    lcd.print("YOU LOSE");
    firstDead = false;
  }
} 
