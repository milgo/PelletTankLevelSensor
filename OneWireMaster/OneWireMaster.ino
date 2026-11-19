#include <OneWire.h>
#include <Adafruit_VL53L0X.h>
#include <DallasTemperature.h>

#define ENABLE_PIN  2
#define DATALOG_ONE_WIRE_BUS 3
#define TEMP_ONE_WIRE_BUS 9

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
OneWire oneWire(TEMP_ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

union {
  long ival;
  float fval;
} temp;

uint16_t level = 0;

void setup() {
  pinMode(DATALOG_ONE_WIRE_BUS, OUTPUT); // Default state
  pinMode(ENABLE_PIN, INPUT); // Default state
  Serial.begin(9600);

  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  lox.startRangeContinuous();
  tempSensor.begin();
  tempSensor.setResolution(12);
}

// Function to send a bit
void sendBit(bool bit) {
  digitalWrite(DATALOG_ONE_WIRE_BUS, LOW);
  if (bit == 1) {
    delayMicroseconds(10); // Short low for 1
    digitalWrite(DATALOG_ONE_WIRE_BUS, HIGH);
    delayMicroseconds(50);
  } else {
    delayMicroseconds(50); // Long low for 0
    digitalWrite(DATALOG_ONE_WIRE_BUS, HIGH);
    delayMicroseconds(10);
  }
}

void loop() {

  if (lox.isRangeComplete()) {
    level = lox.readRange();
  }  

  if (digitalRead(ENABLE_PIN) != LOW) {
    temp.fval = 1.234f;
    delayMicroseconds(60);//wait for slave to listen
    digitalWrite(DATALOG_ONE_WIRE_BUS, HIGH); //set bus high
    //Serial.print("bus high...");
    for (int i=0; i<16; i++) sendBit(bitRead(level, i));
    for (int i=0; i<32; i++) sendBit(bitRead(temp.ival, i));
  }
}