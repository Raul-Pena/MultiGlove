#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//#define LCDADDR 0x3F // Select correct I2C address of the I2C LCD (0x27 or 0x3F)
#define LCDADDR 0x27 // Uncomment for next address, and comment above line!
#define LCDCOLS 16
#define LCDROWS 2
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS); // LCD object

const int buzzer = 9;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.begin (16,2);
  lcd.print(" *Voltmeter (+-) * "); // Custom Text
  
  //pinMode(buzzer,OUTPUT);
}

void loop() {
  //int num = 120;
  int Pin = analogRead(A0),Pin2 = analogRead(A1);
  float Vin = 0.0;
                                      
  if(Pin > 0 ){//&& Pin2 <= 0
    Vin = (Pin * (5.0*12.19512195/ 1024) * 0.92);  //*.98; *12.19512195 
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    //lcd.print(Pin, 3);
    lcd.print(Vin, 3);
    //lcd.print(" Pin Val");
    lcd.print(" VOLT DC ");
    delay(500);
    
  }else if(Pin2 > 0 ){//&& Pin <= 0
    Vin = Pin2 * (5.0*23.992 / 1024); // Pin to Vin (Reduction Factor 11)  
    lcd.setCursor(0, 1);
    lcd.print("-");
    lcd.setCursor(1, 1);
    //lcd.print(Pin2, 3);
    lcd.print(Vin, 3);
    //lcd.print("  -Pin Val");
    lcd.print(" VOLT DC "); // Custom Text
    delay(500);
    
  }else {//if(Pin <= 0 && Pin2 <= 0){
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC "); // Custom Text
    delay(500);  
  }
    /*ton
     * e(buzzer,1000);
    delay(1000);
    noTone(buzzer);
    delay(1000);*/
}
