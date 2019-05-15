/*
 * Basic code for data logging with Robojax SD card module
 * 
 * SPI Nano/Uno D pins: 10 (CS), 11 (MOSI), 12 (MISO), 13 (SCK)
 * 
 * SD Library reference: https://www.arduino.cc/en/reference/SD
 */

//import libraries for interaction with data logger:
#include <SD.h>
#include <SPI.h>
//declare variables:
File myFile;
int pinCS = 10; // Pin 10 on Arduino Uno/Nano (Slave Select)
int i = 0; //counter
String fileName = "DATA.csv"; //max of 8 characters, upper-case letters

void setup() { //sets up file
    
  Serial.begin(9600); // Starts the serial communication
  pinMode(pinCS, OUTPUT); //needed so communication works
  
  // SD Card Initialization
  // SD.begin initializes the SD library and card. returns true on success, false otherwise
  if (SD.begin()){
    Serial.println("SD card is ready to use.");
  } else{
    Serial.println("SD card initialization failed");
  }

//  //Uncomment if you want to delete the previous file
//  if (SD.remove(fileName)){
//    Serial.println("File deleted successfully")
//  }
  
  // Create/Open file
  // SD.open return true when it successfully opens or creates the file, false otherwise
  myFile = SD.open(fileName, FILE_WRITE); //FILE_WRITE: can both read and write; if file exists, will just open
  
  // if the file opened okay, write to it:
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
}//end of setup

void loop() { //adds data to file
  
  if (i < 10)
  {
    myFile = SD.open(fileName, FILE_WRITE);
    unsigned long currentTime = millis(); //for comparing data vs. time
    
    if (myFile) { 
      Serial.println("Writing to file"); //displays on the serial monitor 

      //replace "variableN" by desired variables:
      myFile.print(currentTime);       //rowN, column1 
      myFile.print(",");
      myFile.print("variable1");       //rowN, column2 
      myFile.print(",");
      myFile.print("variable2");       //rowN, column3
      myFile.print(",");  
      myFile.println("variable3");     //rowN, column4
      
      myFile.close(); // close the file - VERY IMPORTANT!
    }
    // if the file didn't open, print an error:
    else {
      Serial.println("error opening file - loop");
    }

  //can also display on serial monitor what's printing to file
  //replace "variableN" by desired variables
  Serial.print("Time: "); 
  Serial.print(currentTime);
  Serial.print("  Measurement1: ");
  Serial.print("variable1"); 
  Serial.print("  Measurement2: ");
  Serial.println("variable2...");
  Serial.print("Iteration: "); Serial.println(i); //lets you know in which iteration it is
  }
  
  i ++; //increments counter
  delay(100); //a "pause" if needed for sensors, etc.
}//end of loop
