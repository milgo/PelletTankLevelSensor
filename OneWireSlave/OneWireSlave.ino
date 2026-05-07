#define SLAVE_PIN 3

void setup() {
  pinMode(SLAVE_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Ready...");
}

// Function to read a bit
bool readBit() {
    if (pulseIn(SLAVE_PIN, LOW, 1000) <= 20)
      return 1;
    return 0;
}

void loop() {
  // Wait for reset pulse (480us - 960us LOW)
  byte readByte = 0;
  if (digitalRead(SLAVE_PIN) == LOW) {
    //Serial.println("is high");
    long start = micros();
    long duration = 0;
    while (digitalRead(SLAVE_PIN) == LOW){
      duration = micros() - start;
      if (duration > 400) break;
    }
    //Serial.print("is low after ");
    Serial.println(duration);
    if (duration > 400) {
      // Valid reset, send presence pulse
      delayMicroseconds(30); // Wait after reset
      pinMode(SLAVE_PIN, OUTPUT);
      digitalWrite(SLAVE_PIN, HIGH);
      delayMicroseconds(60);//pulse high 60us
      pinMode(SLAVE_PIN, INPUT);//listen
      Serial.print("listening...");
      //while (digitalRead(SLAVE_PIN) == LOW){}//wait for bus to go high
      //Serial.print("go...");
      //Serial.println("pulse");


      for (int i=0; i<8; i++) if(readBit()==1) readByte = bitSet(readByte, i);
      //for (int i=0; i<8; i+=2) readByte |=  i << 1;

      Serial.println(readByte, HEX);
      //while(1){}
    }
  }
}