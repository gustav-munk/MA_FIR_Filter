const int triggerPin = 7;  // Ultrasonic sensor trigger pin
const int echoPin = 6;     // Ultrasonic sensor echo pin
const double fs = 100; // Hz
const double T = 1.0 / fs; // Time period in seconds

const int M = 10; // Order of the FIR filter
float measurements[M + 1] = {0}; // Array to store the last M+1 measurements
float sum = 0; // Sum of the measurements
int index = 0; // Current index for the measurements array

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  unsigned long startTime = micros();  // Get the start time in microseconds

  long duration;
  float distance;

  // Trigger the ultrasonic sensor to send a pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Measure the duration of the pulse
  duration = pulseIn(echoPin, HIGH); // returns time in microseconds

  // Calculate the distance in meters
  distance = (duration / 2.0) / 2911.0; 

  // FIR Filter Implementation
  sum -= measurements[index]; // Subtract the oldest measurement from the sum
  measurements[index] = distance; // Add the new measurement to the array
  sum += measurements[index]; // Add the new measurement to the sum
  index = (index + 1) % (M + 1); // Update the index, and wrap around if necessary

  float filteredDistance = sum / (M + 1);

  Serial.print(distance,3); // unfiltered distance
  Serial.print(",");
  Serial.println(filteredDistance,3); //  filtered distance



  unsigned long endTime = micros();  // Get the end time in microseconds

  // Calculate the time taken for one loop iteration
  long loopTime = endTime - startTime;

  // Calculate the required delay to ensure a 100 Hz sampling rate
  long delayTime = (long)(T * 1000000 - loopTime); // Convert T to microseconds and subtract loopTime

  // Ensure delayTime is not negative and delay
  if (delayTime > 0) {
    delayMicroseconds(delayTime);
  }
}
