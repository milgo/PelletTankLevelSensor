#define ENABLE_PIN 2
#define ONE_WIRE_PIN 3

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(ONE_WIRE_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Ready...");
}

// Function to read a bit
bool readBit() {
    if (pulseIn(ONE_WIRE_PIN, LOW, 1000) <= 20)
      return 1;
    return 0;
}

void loop() {
  int readByte = 0;
  digitalWrite(ENABLE_PIN, HIGH);
  Serial.print("listening...");
  for (int i=0; i<16; i++) if(readBit()==1) readByte = bitSet(readByte, i);
  digitalWrite(ENABLE_PIN, LOW);
  Serial.println(readByte);
  delay(1000);

}