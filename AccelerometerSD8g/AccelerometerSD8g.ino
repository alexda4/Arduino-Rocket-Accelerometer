//pins:
//accelerometer: SCL to A5, SDA to A4
//SD Logger: CS to 10, MOSI to 11, MISO to 12, SCK to 13

#include<Wire.h>
#include <SD.h>
#include <SPI.h>
File myFile;
int pinCS = 10; // Pin 10 on Arduino Uno/Nano (Slave Select)
int i = 0; //counter
String fileName = "DATA.CSV"; //max of 8 characters, upper-case letters
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp; 

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(bit(4));  //0:2g, 3:4g, 4:8g
  Wire.endTransmission(true);
 
  Serial.begin(9600);

  pinMode(pinCS, OUTPUT); //needed so communication works
  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  } else{
    Serial.println("SD card initialization failed");
  }
  if (SD.remove(fileName)){
   Serial.println("File deleted successfully");
  }
   myFile = SD.open(fileName, FILE_WRITE); //FILE_WRITE: can both read and write; if file exists, will just open
    if (myFile) {
    // Write to file
    myFile.println("Collecting data");    //row1, column1 
    myFile.print("Time (ms)");            //row2, column1 
    myFile.print(","); 
    myFile.print("measurement1");         //row2, column2
    myFile.print(","); 
    myFile.print("measurment2");          //row2, column3
    myFile.print(","); 
    //Last column of a row must use println:
    myFile.println("measurment3");        //row2, column4
    
    myFile.close(); // close the file - VERY IMPORTANT
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening file - setup");
  }
}
void loop(){
   
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);  
    AcX=Wire.read()<<8|Wire.read();    
    AcY=Wire.read()<<8|Wire.read();  
    AcZ=Wire.read()<<8|Wire.read(); 

    Serial.print("Accelerometer: ");
    Serial.print(" "); Serial.print(AcX);
    Serial.print(" "); Serial.print(AcY);
    Serial.print(" "); Serial.println(AcZ); 
    myFile = SD.open(fileName, FILE_WRITE);
    unsigned long currentTime = millis(); //for comparing data vs. time
    
    if (myFile) { 
      Serial.println("Writing to file"); //displays on the serial monitor 

      //replace "variableN" by desired variables:
      myFile.print(currentTime);       //rowN, column1 
      myFile.print(",");
      myFile.print(AcX);       //rowN, column2 
      myFile.print(",");
      myFile.print(AcY);       //rowN, column3
      myFile.print(",");  
      myFile.println(AcZ);     //rowN, column4
      
      myFile.close(); // close the file - VERY IMPORTANT!
    }
    
    //delay(10);
   
   
}
