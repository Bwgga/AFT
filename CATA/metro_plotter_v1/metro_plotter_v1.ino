// Basic demo for accelerometer readings from Adafruit MPU6050

/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/
/*
 ** Pinout for each module
  Vibration
 ** S - 
  Button
 ** S - 
  LED
 ** R - 
 ** G - 
 ** B -
*/

//SD card libraries
#include <SPI.h>
#include <SD.h>
//MPU-6050 libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const int chipSelect = 10; //SD CS pin on Arduino

Adafruit_MPU6050 mpu;

String fileNumber = "";

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  //Try to initialize SD card
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  //MPU 6050 setup
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");

  //FOR TESTING, ONLY ONCE. In operation, create in loop whenever button pushed
  fileNumber = String(createNextFile());

  delay(100); //wait just a little I guess

}

int createNextFile() {
  int i = 0;
  String fileName = "";
  do {
    i += 1;
    String number = String(i);
    fileName = "metro" + number + ".txt";
  } while (SD.exists(fileName));  //checks for numbered files and increments i when it does. breaks when the file does not already exist.
  Serial.println("The file will be named: " + fileName);

  return i;
}

void loop() {
  //when button pushed
  //createNewFile();
  String fileName = "metro" + fileNumber + ".txt";

  // make a string for assembling the data to log:
  String dataString = "";

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values FOR TESTING */
  Serial.print("AccelX:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("AccelY:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("AccelZ:");
  Serial.print(a.acceleration.z);
  Serial.print(", ");
  Serial.println("");

  //Write data to SD
  dataString += String(a.acceleration.x);
  dataString += ",";
  dataString += String(a.acceleration.y);
  dataString += ",";
  dataString += String(a.acceleration.z);
  dataString += ",";

  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fileName);
  }

  delay(100);
}