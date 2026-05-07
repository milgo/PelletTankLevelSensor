#define ONE_WIRE_PIN 3

void setup() {
  pinMode(ONE_WIRE_PIN, INPUT); // Default state
  Serial.begin(9600);
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
bool resetBus() {
  pinMode(ONE_WIRE_PIN, OUTPUT);
  digitalWrite(ONE_WIRE_PIN, LOW);
  delayMicroseconds(480);
  pinMode(ONE_WIRE_PIN, INPUT);
  delayMicroseconds(70);
  long start = micros();
  long duration = 0;
  while (digitalRead(ONE_WIRE_PIN) == LOW){
    duration = micros() - start;
    if (duration > 1000) return false;
  }
  return true;
}

void loop() {
  if (resetBus()) {
    Serial.println("Slave Present");
    //delayMicroseconds(60);//wait for slave to listen
    pinMode(ONE_WIRE_PIN, OUTPUT);
    digitalWrite(ONE_WIRE_PIN, HIGH); //set bus high
    Serial.print("bus high...");
    //delayMicroseconds(100);
    // Send 1 byte of data (e.g., 0x55)
    for (int i=0; i<8; i++) sendBit(bitRead(0xca, i));
    //while(1){}
  }else{
    Serial.println("Slave not Present");
  }
  delay(1000);
}