#include <CapacitiveSensor.h>
#include "AK09918.h"
#include <Wire.h>
CapacitiveSensor cap = CapacitiveSensor(7,9);

AK09918_err_type_t err;
int32_t x, y, z;
AK09918 ak09918;

const int trampPin = 5;
const int touchPin = 4;

const int touchThreshold = 100;
const int magnetfeltThreshold = 35;

double magnetMax = 0;
double kalibrasjon = 0;

void setup() {

    // Setter LED-pinnene som output
  pinMode(trampPin, OUTPUT);
  pinMode(touchPin, OUTPUT);

    // Tester LEDene
  digitalWrite(trampPin, HIGH);
  digitalWrite(touchPin, HIGH);
  delay(1000);
  digitalWrite(trampPin, LOW);
  digitalWrite(touchPin, LOW);

  Wire.begin();

  err = ak09918.initialize();
  ak09918.switchMode(AK09918_POWER_DOWN);
  ak09918.switchMode(AK09918_CONTINUOUS_100HZ);
  Serial.begin(9600);
  while (err != AK09918_ERR_OK){
      Serial.print("Waiting Sensor");
      delay(100);
      err = ak09918.isDataReady();
  }  

  err = ak09918.getData(&x, &y, &z);
  kalibrasjon = z;
}

void loop() {
  bool tarPa = touch();
  bool forbi = magnetStatus();

  touch();
  magnetStatus();
  if (tarPa && forbi) {
    digitalWrite(trampPin, HIGH);
    delay(1000);
    digitalWrite(trampPin, LOW);
  }
  
  delay(10);
}

  // Sjekker om steinen har passert en stein, gir true/false som output.
  // Fungerer ved å lagre den høyeste verdien den har målt. Når denne verdien passerer et punkt
bool magnetStatus() {

  err = ak09918.getData(&x, &y, &z);    // Leser av Magnetsensor
  double magnetVerdi = abs(z - kalibrasjon);     // Konverterer inputen. Omtrent 0 i nøytral tilstand
  
   Serial.println(magnetVerdi);         // Kan brukes til feilsøking
  // Serial.print(", ");                
  
  if (magnetVerdi > magnetMax) {
    magnetMax = magnetVerdi;
  } else if (magnetMax - magnetfeltThreshold > magnetVerdi) {
    magnetMax = 0;
    return true;
  }
  return false;  
}

  // Sjekker om steinen blir holdt på, gir true/false som output.
bool touch() {  
  long sensorValue = cap.capacitiveSensor(30);
  
  // Serial.println(sensorValue);     // Kan brukes til feilsøking
  
  if (sensorValue > touchThreshold) {
    digitalWrite(touchPin, HIGH);
    return true;
  }else {
    digitalWrite(touchPin, LOW);
    return false;
  }
  
}
