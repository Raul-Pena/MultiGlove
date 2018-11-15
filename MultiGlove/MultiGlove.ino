#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//#define LCDADDR 0x3F // Select correct I2C address of the I2C LCD (0x27 or 0x3F)
#define LCDADDR 0x27 // Uncomment for next address, and comment above line!
#define LCDCOLS 16
#define LCDROWS 2
LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS); // LCD object

const int buzzer = 9, vd = 7, va = 8, r = 10, ct = 11, ca = 12, id = 13; //pin value for buzzer (digital pin 9)
int cnt = 0, wrldPin = 0;

void contTest();
void voltDC();
void voltAC();

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  lcd.begin (16,2);
  lcd.print(" *Voltmeter (+-) * ");
  
  pinMode(buzzer,OUTPUT);  //buzzer for cont. test
  pinMode(vd,INPUT);
  pinMode(va,INPUT);
  pinMode(r,INPUT);
  pinMode(ct,INPUT);
  pinMode(ca,INPUT);
  pinMode(id,INPUT);
  
}

void loop() {
  if(digitalRead(vd)){
    voltDC();
  }else if(digitalRead(va)){
    voltAC();
  }else if(digitalRead(r)){
  }else if(digitalRead(ct)){
    contTest();
  }else if(digitalRead(ca)){
  }else if(digitalRead(id)){
  } 
}

void voltDC(){/////////////////////////////////////////////////////////volt dc meter
  int Pin = analogRead(A0);
  int Pin2 = analogRead(A1);
  float Vin = 0.0;
                                      
  if(Pin > 0 ){///////////////////////////////////////////////////////////////////pos volt dc
    Vin = (Pin * (5.0*11.90556667/ 1024));  //*.98; *12.19512195 11.99601159 
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    //lcd.print(Pin, 3);
    lcd.print(Vin, 3);
    //lcd.print(" Pin Val");
    lcd.print(" VOLT DC ");
    delay(500);
  }else if(Pin2 > 0 ){///////////////////////////////////////////////////////////////////neg volt dc
    Vin = Pin2 * (5.0*12.58714362 / 1024);   
    lcd.setCursor(0, 1);
    lcd.print("-");
    lcd.setCursor(1, 1);
    //lcd.print(Pin2, 3);
    lcd.print(Vin, 3);
    //lcd.print("  -Pin Val");
    lcd.print(" VOLT DC ");
    delay(500);
  }else {////////////////////////////////////////////////////output 0v
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(500);  
  } 
  
}

void voltAC(){/////////////////////////////////////////////////////////volt ac meter
  int Pin = analogRead(A0), bigPin = 0;
  double Vin = 0.0;
  
  if(Pin > 0){     
      if(cnt == 0){ 
        for(int i = 0; i < 11; i++){
          if(bigPin < Pin){
            bigPin = Pin;  
          }
          delay(1);   //comment out for experiment
          Pin = analogRead(A0); 
        }
         wrldPin = bigPin;
         cnt = 1;
       } 
    Vin = ((wrldPin * (5.0/ 1024)));
    Vin = sqrt(Vin);
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    //lcd.print(Pin, 3);
    lcd.print(Vin, 3);
    //lcd.print(" Pin Val");
    lcd.print(" VOLT AC ");
    delay(500); 
  } else {
    cnt = 0, wrldPin = 0;
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT AC ");
    delay(500); 
  }
}

void contTest(){/////////////////////////////////////---Continuity Test DO NOT DELETE YET
  int pin = analogRead(A0);
  if(pin > 0){
      tone(buzzer,1);
      delay(10);
      noTone(buzzer);
      delay(1000);
  }
}
