#include <Arduino.h>
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "Adafruit_BMP280.h"
#include "Adafruit_MPU6050.h"
#include "elapsedMillis.h"

//define I2C Pins
#define SCL_PIN 222
#define SDA_PIN 222

//define SD card SS pin
#define SD_SS 9 

Adafruit_BMP280 bmp;
//Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
//Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
//sensors_event_t bmpTempEvent, pressureEvent;
#define bmp_ADDRESS 0x77
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_MPU6050 mpu;
sensors_event_t a, w, mpuTemp;
#define mpu_ADDRESS 0x68

File myFile;

elapsedMicros elapsedTime;
#define dp 4
const char defaultDelim = ',';

void addToString(float sensorVal, String dataString, int decimalPlace = 4, char delim = defaultDelim){
  dataString += String(sensorVal, decimalPlace);
  dataString += delim;
}

void logData(){
  String dataString;
  addToString(elapsedTime, dataString, 1);
  //read mpu data
  mpu.getEvent(&a, &w, &mpuTemp);
   
  //read bmp
  bmp.takeForcedMeasurement();
  
  addToString(a.acceleration.x, dataString);
  addToString(a.acceleration.y, dataString);
  addToString(a.acceleration.z, dataString);
  addToString(w.gyro.x, dataString);
  addToString(w.gyro.y, dataString);
  addToString(w.gyro.z, dataString);

  addToString(bmp.readPressure(), dataString);
  addToString(bmp.readAltitude(), dataString);
  addToString(bmp.readTemperature(), dataString, 4, '\n');

  Serial.println(dataString);
}

void setup() {
  Wire.begin(); //TODO check which channel is used
  Wire.setSCL(SCL_PIN);
  Wire.setSDA(SDA_PIN);

  if(!mpu.begin(mpu_ADDRESS, &Wire)){
    //do something
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


  if(!bmp.begin(bmp_ADDRESS)){
    //do something
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  String header = "time(s), *C, Pa, Alt (m), ax (m/s^2), ay (m/s^2), az (m/s^2), wx (deg/s), wy (deg/s), wz (deg/s),";
  if (SD.begin(SD_SS)) {
    File dataFile = SD.open("DATA.txt", FILE_WRITE); //TODO add automatic increase file number
    dataFile.println(header);
    dataFile.close();
  }
}

void loop() {
  
  //store
  /*
  File dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.println(dataString);
  dataFile.close();
  delay(1);
  */
}