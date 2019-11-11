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
const int feltThreshold = 20;

double magnetMax = 0;

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

bool magnetStatus() {

  err = ak09918.getData(&x, &y, &z);
  double magnetVerdi = abs(z + 70);
  // Serial.print(magnetVerdi);
  // Serial.print(", ");

  if (magnetVerdi > magnetMax) {
    magnetMax = magnetVerdi;
  } else if (magnetMax - 100 > magnetVerdi) {
    magnetMax = 0;
    return true;
  }
  return false;  
}

bool touch() {
  long sensorValue = cap.capacitiveSensor(30);
  // Serial.println(sensorValue);
  if (sensorValue > touchThreshold) {
    digitalWrite(touchPin, HIGH);
    return true;
  }else {
    digitalWrite(touchPin, LOW);
    return false;
  }
  
}
