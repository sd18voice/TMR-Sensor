unsigned long previousMicros = 0;   // Stores the last sample time
const unsigned long sampleInterval = 1000; // 1 ms in microseconds

void setup() {
  Serial.begin(38400);
  pinMode(7, OUTPUT); // Set LED pin mode
}

void loop() {
  // Check if 1 ms has passed
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= sampleInterval) {
    previousMicros = currentMicros;

    // Read sensor values
    float bmag = analogRead(A1);
    float pow1 = analogRead(A2);
    float temp = analogRead(A3);

    // Toggle LED as indicator
    digitalWrite(7, HIGH);

    // Print values separated by "x"
    Serial.print(bmag);
    Serial.print("x");
    Serial.print(pow1);
    Serial.print("x");
    Serial.println(temp);

    digitalWrite(7, LOW);
  }
}