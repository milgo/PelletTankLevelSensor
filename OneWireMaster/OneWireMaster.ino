#include "Adafruit_VL53L0X.h"
#define ENABLE_PIN  2
#define ONE_WIRE_PIN 3

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  pinMode(ONE_WIRE_PIN, OUTPUT); // Default state
  pinMode(ENABLE_PIN, INPUT); // Default state
  Serial.begin(9600);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

// Function to send a bit
void sendBit(bool bit) {
  digitalWrite(ONE_WIRE_PIN, LOW);
  if (bit == 1) {
    delayMicroseconds(10); // Short low for 1
    digitalWrite(ONE_WIRE_PIN, HIGH);
    delayMicroseconds(50);
  } else {
    delayMicroseconds(50); // Long low for 0
    digitalWrite(ONE_WIRE_PIN, HIGH);
    delayMicroseconds(10);
  }
}

// Function to reset bus
bool waitForEnable() {
  while (digitalRead(ENABLE_PIN) == LOW){}
  return true;
}

void loop() {

    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    } else {
      //Serial.println(" out of range ");
    }

  if (waitForEnable()) {
    Serial.println("Slave Present");
    delayMicroseconds(60);//wait for slave to listen
    digitalWrite(ONE_WIRE_PIN, HIGH); //set bus high
    Serial.print("bus high...");
    for (int i=0; i<16; i++) sendBit(bitRead(measure.RangeMilliMeter, i));
  }else{
    Serial.println("Slave not Present");
  }
  delay(100);
}