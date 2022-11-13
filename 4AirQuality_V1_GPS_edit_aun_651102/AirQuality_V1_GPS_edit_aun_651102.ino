#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <stdio.h>

int SPEED = 0;
int LAT = 0;
int LNG = 0;

TinyGPSPlus gps;
HardwareSerial ss(2);


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial.println("GPS Start");
}

void loop() {

  while(Serial2.available())//While there are characters to come from the GPS
  {
    gps.encode(Serial2.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(),6);
    LAT = gps.location.lat();
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(),6);
    LNG = gps.location.lng();
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph(),6);
    SPEED = gps.speed.mph();
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet(),6);
    Serial.println("");
    Serial.println("");
  }
  
  //int LAT = 13;
  //int LNG = 100;
  //int SPEED = 10;
  Serial.println("");
  Serial.println("");
  //#################################################
  delay(2000);
}
