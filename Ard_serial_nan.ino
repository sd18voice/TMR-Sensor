unsigned long previousMicros = 0;   // Stores the last sample time
const unsigned long sampleInterval = 1666; // 1.666 ms (600 samples/second)

const int samplesPerCycle = 10;
int sampleCount = 0;

float bmagSumOfSquares = 0; // Use float to avoid overflow issues
int bmagMin = 1023;         // For tracking minimum
int bmagMax = 0;            // For tracking maximum
int bmax = 0;
float bmagRMS = 0;          // RMS value requires float
int bmin = 1023;
int prevbmag = 0;
const int shiftValue = 512; // Shift value remains as int
const int thres = 93;
byte sat = 0;

void setup() {
  Serial.begin(250000);
  pinMode(7, OUTPUT); // Set LED pin mode
}

void loop() {
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= sampleInterval) {
    previousMicros = currentMicros;

    // Read sensor values
    int bmag = analogRead(A1) - shiftValue; // Subtract shift value
    int pow1 = analogRead(A2);
    float tempread= analogRead(A5);
    float voltage = tempread * (5.0 / 1024.0);  // convert analog reading to voltage
    float temp = (voltage - 0.5) * 100; 

    int didt = bmag - prevbmag;

    prevbmag = bmag;
    sat=0;
    // Update min and max values
    if (bmag > thres) {
      sat = 1;
    }
    if (bmag < -thres) {
      sat = 1;
    }

    // Accumulate square of bmag
    bmagSumOfSquares += (float)bmag * bmag; // Cast bmag to float to ensure precision
    sampleCount++;

    // Toggle LED as indicator
    digitalWrite(7, HIGH);

    // Print values over Serial
    Serial.print(bmag);
    Serial.print("x");
    Serial.print(bmagRMS, 1);  // Print RMS with 1 decimal place
    Serial.print("x");
    Serial.print(sat);
    Serial.print("x");
    Serial.print(didt);
    Serial.print("x");
    Serial.print(pow1);
    Serial.print("x");
    Serial.println(temp);
    digitalWrite(7, LOW);

    // Cycle completion: calculate RMS and reset
    if (sampleCount >= samplesPerCycle) {
      bmagRMS = sqrt(bmagSumOfSquares / sampleCount); // Calculate RMS
      bmax = bmagMax;
      bmin = bmagMin;

      // Reset for next cycle
      bmagSumOfSquares = 0;
      bmagMin = 1023;
      bmagMax = 0;
      sampleCount = 0;
    }
  }
}
