#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//#define LCDADDR 0x3F // Select correct I2C address of the I2C LCD (0x27 or 0x3F)
#define LCDADDR 0x27 // Uncomment for next address, and comment above line!
#define LCDCOLS 16
#define LCDROWS 2
//#define analogPin A7
#define chrg 13
#define dchrg 10
#define res 10000.0F  //10k Resistor

LiquidCrystal_I2C lcd(LCDADDR, LCDCOLS, LCDROWS); // LCD object
RTC_DS1307 RTC;
DateTime now;
//const int OUTpin = A2;                         //vd=5           //id = 13
const int pBtn = 5,buzzer = 9, ct = 11, ca = 12, vd = 4, va = 6, /*id = 3,*/ rm = 8;
int count = 0; //cnt = 0, wrldPin = 0,
boolean state = false;
byte USB_Byte;               //used to store data coming from the USB stick
int timeOut = 2000;  //TimeOut is 2 seconds. This is the amount of time you wish to wait for a response from the CH376S module.


SoftwareSerial USB(0, 1);   

//void contTest();   -------------- SKETCH IS ABLE TO COMPILE WITHOUT THESE PROTOTYPES. CAN DELETE AT ANY TIME
//void voltDC();
//void voltAC();
//void InducMeter();

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  USB.begin(9600);
  //Wire.begin();
  //RTC.begin();
  lcd.begin (16,2);
  lcd.print("  Powering On");
  lcd.setCursor(0,1);
  lcd.print("Multi-Use Glove");
  delay(1500);
  lcd.clear();
  
  pinMode(buzzer,OUTPUT);  //buzzer for cont. test
  pinMode(vd,INPUT);
  pinMode(va,INPUT);
  pinMode(ct,INPUT);
  pinMode(rm,INPUT);
  //pinMode(11,INPUT);//COMMENT OUT LATER
  //pinMode(13,OUTPUT);//COMMENT OUT LATER
  pinMode(ca,INPUT);
  //pinMode(id,INPUT);
  pinMode(pBtn,INPUT);
  //pinMode(OUTpin, OUTPUT);
  pinMode(chrg, OUTPUT);
}

void loop() {
   //lcd.setCursor(0, 1);
    //lcd.print("   SELECT MODE  ");
  if(digitalRead(vd)){
    voltDC();
  }else if(digitalRead(va)){
    voltAC();
  }else if(digitalRead(ca)){
    capTest();
  }else if(digitalRead(rm)){
    resMeter();
  }/*else if(digitalRead(ct)){
    contTest();
  }else if(digitalRead(id)){
    InducMeter();
  }*/ 
}

void voltDC(){/////////////////////////////////////////////////////////volt dc meter
  //now = RTC.now();
  int Pin = analogRead(A4);
  int Pin2 = analogRead(A5);
  float Vin = 0.0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DC Voltmeter");                                   
  if(Pin > 0 && Pin <= 171){///////////////////////////////////////////////////////////////////pos volt dc
    Vin = ((Pin * ((5.0*12.19512195)/ 1024)) + 0.17);  //*.98; 11.99601159 
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(750);
    bttnPress(String(String(Vin,3) + " VOLT DC "));
    delay(750);
  } else if(Pin >= 172){//pos volt dc
    Vin = (Pin * (5.0*11.97556667/ 1024) + .3); 
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(750);
    bttnPress(String(String(Vin,3) + " VOLT DC "));
    delay(750);
  } else if(Pin2 > 0 && Pin2 <= 171 ){///////////////////////////////////////////////////////////////////neg volt dc
    Vin = (Pin2 * ((5.0*12.04714362) / 1024)) + 0.2;   
    lcd.setCursor(0, 1);
    lcd.print("-");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(750);
    bttnPress(String(String((-1*Vin),3) + " VOLT DC "));
    delay(750);
  }else if(Pin2 >= 172){////neg volt dc
    Vin = Pin2 * (((5.0*12.04714362)) / 1024) + .32;   
    lcd.setCursor(0, 1);
    lcd.print("-");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(750);  
    bttnPress(String(String((-1*Vin),3) + " VOLT DC "));
    delay(750);
  }else {////////////////////////////////////////////////////output 0v
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(1, 1);
    lcd.print(Vin, 3);
    lcd.print(" VOLT DC ");
    delay(750);
    bttnPress(String(String(Vin,3) + " VOLT DC "));
    delay(750);
  }  
}

void voltAC(){/////////////////////////////////////////////////////////volt ac meter
 // now = RTC.now();
  int Pin = analogRead(A4);
  double Vin = 0.0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AC Voltmeter");
  if(Pin > 0){     
    if(Pin < 390){                             
        Vin = (((Pin * (5.0/ 1024) * (2.376)) + .4))/sqrt(2);
       }else if(Pin > 390){                    
        Vin = (((Pin * (5.0/ 1024) * (2.376)) + .5))/sqrt(2);
       }
  }
  
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.setCursor(1, 1);
  lcd.print(Vin, 3);
  lcd.print(" VOLT RMS ");
  delay(750);
  bttnPress(String(String(Vin,3) + " VOLT RMS "));
  delay(750);
}

void contTest(){/////////////////////////////////////---Continuity Test DO NOT DELETE YET
  int pin = analogRead(A4);
  lcd.clear();
  if(pin > 0){
    lcd.setCursor(0, 1);
    lcd.print("CONT. TEST ON");
    tone(buzzer,1);
    delay(10);
    noTone(buzzer);
    delay(1000);
  }else{
    lcd.setCursor(0, 1);
    lcd.print("NO CONT.");
  }
}


void capTest(){
  //now = RTC.now();
  unsigned long timeStart, timeTook;
  float microF;
  String prnt;
  //float nanoF;
//while( ca > 0)
//{
  
  digitalWrite(chrg, HIGH);
  timeStart = micros();
                  //analogRead(analogPin)
  while(analogRead(A5) < 658){ }   //Pauses until Capacitor is 63.2% V
                      //658 is AnalogConversion
                      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("cap2");
  timeTook = micros() - timeStart;
  microF = ((float)timeTook / res);   //Assume in microFarad range

  /*if(microF > 1){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("100nF-4F");
    lcd.setCursor(0,1);
    lcd.print(microF);
    lcd.setCursor(14,1);
    lcd.print("µF");
    bttnPress(String(String(microF,3) + " µFARAD(S) "));
    delay(500);
  }
  else{
    nanoF = microF * 1000.0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("100nF-4F");
    lcd.setCursor(0,1);
    lcd.print(nanoF);
    lcd.setCursor(14,1);
    lcd.print("nF");
    bttnPress(String(String(nanoF,3) + " nFARAD(S) "));
    delay(500);

  }*/
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("100nF-4F");
    lcd.setCursor(0,1);
    if(microF > 1){
      lcd.print(microF);
      prnt = " µ";
    }else{
      lcd.print(microF * 1000.0);
      prnt = " n";
    }
    lcd.setCursor(14,1);
    lcd.print(String(prnt + "F"));
    prnt = String( prnt + "FARAD(S) ");
    delay(750);
    bttnPress(String(String(microF,3) + prnt));
    delay(750);
//}

  digitalWrite(chrg,LOW);       //DISCHARGES CAP for SAFETY
  pinMode(dchrg,OUTPUT);
  digitalWrite(dchrg,LOW);
                    //analogRead(analogPin)
  while(analogRead(A5) > 0){}

  pinMode(dchrg,INPUT);       //GIVES High Impedence

  /*lcd.setCursor(0,0);
  lcd.print("Dishcarging Capacitor");
  lcd.setCursor(0,1);
  delay(1000)*/
} 

void resMeter(){
  //now = RTC.now();
  int v1 = analogRead(A0), v2 = analogRead(A1), v3 = analogRead(A2), v4 = analogRead(A3);
  double r1 = 8107, r2 = 19911.0, r3 = 218760.0, r4 = 1080000.0, rx = 108030.0, vin = 4.989;
  float rM = 0, vout = 0;
  boolean zeroFlag = false;
  
  lcd.clear();
  lcd.setCursor(0,0);
  
  if(v1 <= 1023 && v1 >= 296.294){
    vout = (v1 * vin) / 1024.0;
    rM = ( r1 * ((vin/vout) - 1));

    //lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("Range: 0-19k Ohms");
    //lcd.setCursor(0,1);
    //lcd.print(rM);
    //delay(5000);
    
  }else if(v2 <= 511.98 && v2 >= 85.43){
    vout = (v2 * vin) / 1024.0;
    rM = ( r2 * ((vin/vout) - 1));

    //lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("Range: 20k Ohms");
    //lcd.setCursor(0,1);
    //lcd.print(rM);
    //delay(5000);
    
  }else if(v3 <= 510.83 && v3 >= 181.12 && v1 > 1 ){
    vout = (v3 * vin) / 1024.0;
    rM = ( r3 * ((vin/vout) - 1));

    //lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("Range: 200k Ohms");
    //lcd.setCursor(0,1);
    //lcd.print(rM);
    //delay(5000);
  }else if(v4 <= 511.75 && v4 >= 94.6117){
    vout = (v4 * vin) / 1024.0;
    rM = ( r4 * ((vin/vout) - 1));

    //lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("Range: 1M Ohms");
    //lcd.setCursor(0,1);
    //lcd.print(rM);
    //delay(5000);
  }else{
    zeroFlag = true;
    //lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print(" Insert Resistor ");
    delay(2000);
  }
  
  if(!zeroFlag){
    lcd.setCursor(0,1);
    lcd.print(rM);
    delay(750);
    bttnPress(String(String(rM,3) + " OHMS "));
    delay(750);
  }
}


/*void InducMeter(){
  //now = RTC.now();
  double pulse, F, Cap, Induc;
  digitalWrite(13, HIGH);
  delay(5);//give some time to charge inductor.
  digitalWrite(13, LOW);
  delay(.1); //make sure resination is measured
  pulse = pulseIn(11, HIGH, 5000);//returns 0 if timeout
  
  if (pulse > 0.1){ //if a timeout did not occur and it took a reading:
    
  Cap = 1.926E-6; // - insert value here
  
  
  F = 1.E6/(2*pulse);
  Induc = 1./(Cap*F*F*4.*3.14159*3.14159);//one of my profs told me just do squares like this
  Induc *= 1E6; //note that this is the same as saying inductance = inductance*1E6

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Inductance:");
  lcd.setCursor(0,1); 
  lcd.print(Induc);
  lcd.setCursor(14,1); 
  lcd.print("uH");          
  delay(10);
  //LCD print
  //lcd.setCursor(0,1); 
  //lcd.print(" ");
  //lcd.setCursor(1,1); 
  //lcd.print(Induc, 3); 
  //lcd.print("Inductance");          
  //delay(5000);
  }
}*/


void bttnPress(String data){
 
    if(checkConnection(0x01) && !state){
      lcd.clear(); lcd.print("connected success"); delay(750);
      state = true;   
    }
    if(digitalRead(pBtn) == HIGH && state){
      lcd.clear(); lcd.print("btn pressed2");delay(750);
      Serial.println("Button Pressed");
                                    //timeStamp(data)
      appendFile("TEST1.TXT",String(data + "\r\n")); 
      delay(1000); 
    }
    if(USB.available()){
    //^^^This is here to capture any unexpected data transmitted by the CH376S module
      USB.read();
    }   
}
/*String timeStamp(String edit){
    now = RTC.now();
    edit.concat(String(" " + String(now.hour(),DEC)));
    edit.concat(String(":" + String(now.minute(),DEC)));
    edit.concat(String(":" + String(now.second(),DEC)));
    edit.concat(String("  " + String(now.month(),DEC)));
    edit.concat(String("/" + String(now.day(),DEC)));
    edit.concat(String("/" + String(now.year(),DEC)));

    return edit;
}*/

void writeBytes(){
  USB.write(0x57);
  USB.write(0xAB); 
}
//This function is used to check for successful communication with the CH376S module. This is not dependant of the presence of a USB stick.
//Send any value between 0 to 255, and the CH376S module will return a number = 255 - value. 
bool checkConnection(byte value){
  writeBytes();
  USB.write(0x06);
  USB.write(value);
  
  if(waitForResponse()){       
    // ^^^ wait for a response from the CH376S. If CH376S responds, it will be true. If it times out, it will be false.
    if(getResponseFromUSB()==(255-value)){
      return true;
    } else {
      return false;
    }
  }
}

//Make sure that the USB is inserted when using 0x06 as the value in this specific code sequence
void set_USB_Mode (byte value){
  writeBytes();
  USB.write(0x15);
  USB.write(value);
  
  delay(20);
  
  if(USB.available()){
    USB_Byte=USB.read();
    //Check to see if the command has been transmitted and acknowledged.
    if(USB_Byte==0x51){                                   // If true - the CH376S has acknowledged the command.
        //The USB module will now monitor the port
        USB_Byte = USB.read();
        
        //Check to see if the USB stick is connected or not.
        /*if(USB_Byte==0x15){  // If true - there is a USB stick connected   ===== May use this to show that the USB is connected via the LCD screen
          } else {                                                                 dont delete pls.
          }*/
        
    }  
  }
  delay(20);
}


//Performs a hardware reset of the module.
void resetALL(){
    writeBytes();
    USB.write(0x05);
    delay(200);
}

//is used to write data to the end of the file, without erasing the contents of the file.
void appendFile(String fileName, String data){
    resetALL();                     //Reset the module
    set_USB_Mode(0x06);             //Set to USB Mode
    diskConnectionStatus();         //Check that communication with the USB device is possible
    USBdiskMount();                 //Prepare the USB for proper read/write operations.
    setFileName(fileName);          //Set File name
    fileOpen();                     //Open the file
    filePointer(false);             //filePointer(false) is to set the pointer at the end of the file.  filePointer(true) will set the pointer to the beginning.
    fileWrite(data);                //Write data to the end of the file
    fileClose();                    //Close the file 
}  

//This sets the name of the file to work with
void setFileName(String fileName){
  writeBytes();
  USB.write(0x2F);
  USB.write(0x2F);         // filename must have this byte to indicate the start of the file name.
  USB.print(fileName);     // "fileName" is a variable that holds the name of the file.
  USB.write((byte)0x00);   // you need to cast as a byte - otherwise it will not compile.  The null byte indicates the end of the file name.
  delay(20);
}

//Check the disk connection status
void diskConnectionStatus(){
  writeBytes();
  USB.write(0x30);
  if(waitForResponse()){       //wait for a response from the module. If it responds, it will be true, otherwise it will be false.
    if(getResponseFromUSB()!=0x14){               //The module will send 0x14 if this command was successful
      //lcd screen output about not being able to connect to usb
    }
  }
}

//initialise the USB disk and check that it is ready - this process is required if you want to find the manufacturing information of the USB disk
void USBdiskMount(){
  //Serial.println("Mounting USB disk");
  writeBytes();
  USB.write(0x31);

  if(waitForResponse()){       //wait for a response from the CH376S. If CH376S responds, it will be true. If it times out, it will be false.
    if(getResponseFromUSB()!=0x14){               //CH376S will send 0x14 if this command was successful
      //some error message on LCD for mounting the USB
    }
  }
}

//opens the file for reading or writing
void fileOpen(){  ///////////////////////////////////new fileOpen()
  //Serial.println("Opening file.");
  writeBytes();
  USB.write(0x32);
  if(waitForResponse()){                 //wait for a response from the CH376S. If CH376S responds, it will be true. If it times out, it will be false.
    if(getResponseFromUSB()!=0x14){                 //CH376S will send 0x14 if this command was successful  
      if(fileCreate()){               //Try to create a new file. If file creation is successful
        fileWrite(String("------Measurements Taken------\r\n"));              //write data to the file.
      } else {
        //probably get the lcd screen to display an error message. If not then just delete this else.
      }
    }
  }
}

/*/writes the file size to the serial Monitor.  /////////////////////////////////// MAY FIND SOME USE FOR THIS CODE, DO NOT DELETE
int getFileSize(){
  int fileSize=0;
  Serial.println("Getting File Size");
  writeBytes();
  USB.write(0x0C);
  USB.write(0x68);
  delay(100);
  Serial.print("FileSize =");
  if(USB.available()){
    fileSize = fileSize + USB.read();
  } 
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255);
  } 
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255*255);
  }
  if(USB.available()){
    fileSize = fileSize + (USB.read()*255*255*255);
  }     
  Serial.println(fileSize);
  delay(10);
  return(fileSize);
}*/

//the commands used to write to the file
void fileWrite(String data){
  byte dataLength = (byte) data.length();         // This variable holds the length of the data to be written (in bytes)
  delay(100);
  // This set of commands tells the CH376S module how many bytes to expect from the Arduino.  (defined by the "dataLength" variable)
  writeBytes();
  USB.write(0x3C);
  USB.write((byte) dataLength);
  USB.write((byte) 0x00);
  if(waitForResponse()){      // Wait for an acknowledgement from the CH376S module before trying to send data to it
    if(getResponseFromUSB()==0x1E){                // 0x1E indicates that the USB device is in write mode.
      writeBytes();
      USB.write(0x2D);
      USB.print(data);                             // write the data to the file
      if(waitForResponse()){   // wait for an acknowledgement from the module
      }
      USB.read();                // code is normally 0xFF
      writeBytes();
      USB.write(0x3D);                             // This is used to update the file size
      if(waitForResponse()){   // wait for an acknowledgement
      }
      USB.read();              //code is normally 0x14
    }
  }
}

//the command sequence to create a file
boolean fileCreate(){
  boolean createdFile = false;
  writeBytes();
  USB.write(0x34);
  if(waitForResponse()){       //wait for a response. If file has been created successfully, it will return true.
     if(getResponseFromUSB()==0x14){          //CH376S will send 0x14 if this command was successful
       createdFile=true;
     }
  }
  return(createdFile);
}

//is used to set the file pointer position. true for beginning of file, false for the end of the file.
void filePointer(boolean fileBeginning){
  writeBytes();
  USB.write(0x39);
  if(fileBeginning){
    USB.write((byte)0x00);             //beginning of file
    USB.write((byte)0x00);
    USB.write((byte)0x00);
    USB.write((byte)0x00);
  } else {
    USB.write((byte)0xFF);             //end of file
    USB.write((byte)0xFF);
    USB.write((byte)0xFF);
    USB.write((byte)0xFF);
  }
  if(waitForResponse()){       //wait for a response from the CH376S. 
     /*if(getResponseFromUSB()==0x14){                 //CH376S will send 0x14 if this command was successful
     }*/
     if(USB.available()){
       USB.read();
     }
  }
}

//closes the file
void fileClose(){/*byte closeCmd*/
  //Serial.println("Closing file:");
  writeBytes();
  USB.write(0x36);
  USB.write(0x01);  /*(byte)closeCmd*/        // closeCmd = 0x00 = close without updating file Size, 0x01 = close and update file Size

  if(waitForResponse()){                      // wait for a response from the CH376S. 
    /*if(getResponseFromUSB()==0x14){                 //CH376S will send 0x14 if this command was successful
     }*/
      if(USB.available()){
         USB.read();
      }   
  }
}

//is used to wait for a response from USB. Returns true when bytes become available, false if it times out.
boolean waitForResponse(){  //commment out String errorMsg
  boolean bytesAvailable = true;
  int counter=0;
  while(!USB.available()){     //wait for CH376S to verify command
    delay(1);
    counter++;
    if(counter>timeOut){
      bytesAvailable = false;
      break;
    }
  }
  delay(1);
  return(bytesAvailable);
} 

//is used to get any error codes or messages from the CH376S module (in response to certain commands)
byte getResponseFromUSB(){
  byte response = byte(0x00);
  if (USB.available()){
    response = USB.read();
  }
  return(response);
}


