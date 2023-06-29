#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define LIGHT_PIN A0
Adafruit_BME280 bme; // I2C

const double k = 5.0/1024;
const double luxFactor = 500000;
const double R2 = 10000;
const double B = 1.3*pow(10.0,7);
const double m = -1.4;


void getSensorData(char call_type)
{
  float temp = bme.readTemperature();// read temperature
  float pressure = bme.readPressure() / 100.0F; // read pressure
  float rel_hum = bme.readHumidity();// read humidity

  int light = analogRead(LIGHT_PIN);
  light = map(light, 1024, 0, 0, 100);
  //float alt =bme.readAltitude(SEALEVELPRESSURE_HPA);// read altitude
  StaticJsonDocument<256> doc;
  doc["temperature"]= temp;
  doc["humidity"]= rel_hum;
  doc["pressure"]= pressure/10; //We want pressure in kPa, but sensor returns it in hPa
  doc["light"]= light;
  if(call_type == 's')
    doc["call"]=0;
  else if(call_type == 'f')
    doc["call"]=1;
  else
    doc["call"]=-1;
  serializeJson(doc, Serial);
  //return doc;
}//getBME


void setup() {
  pinMode(LIGHT_PIN, INPUT);
  Serial.begin(9600);
  Serial.println(F("BDA_Analytics. Sensor board"));
  
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  
  Serial.println("-- Ready --");
  delay(2000);

  Serial.println();
}



void loop() { 
  char command;
  if(Serial.available())
    command = Serial.read();
  else 
    command = 'n';
  if(command=='f'){ // Fast call
    getSensorData('f');
    Serial.println();
  }
  else if(command=='s'){ // Slow call
    getSensorData('s');
    Serial.println();
  }
  delay(500);
}// loop end
