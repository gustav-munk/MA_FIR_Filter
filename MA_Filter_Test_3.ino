const int triggerPin = 12;
const int echoPin = 13;

double avgDist;

// FIR filter order
const int M = 4;

void setup() {
  Serial.begin(115200);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  xTaskCreate(uniTask, "UltrasonicTask", 3000, NULL, 3, NULL);
}

void loop() {}


void uniTask(void *pvParameters) {
  (void)pvParameters;

  // Arrays for storing the distance measurements for each sensor
  double meas[M + 1];
  // Variables for the sum of the measurements
  double sum = 0;
  int index = 0;

  double filteredDist;

  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (1) {
    //Serial.println(millis());
    filteredDist = readAndFilterSensor(triggerPin, echoPin, meas, sum, index);  // Measured and filtered distance from sensorL

    Serial.print(",");
    Serial.println(filteredDist, 4);

    index = (index + 1) % (M + 1);  // increase index and wrap around if necessary

    vTaskDelayUntil(&xLastWakeTime, 25);  // period [ms] 1 tick = 1 ms.
  }
}


double readAndFilterSensor(int triggerPin, int echoPin, double *measurements, double &sum, int index) {
  unsigned long startTime;
  long duration;
  double distance;
  double timeOut = 7000;  // Timeout in microseconds - corresponding to 1.2 meter (7000 µs)

  digitalWrite(triggerPin, LOW);

  digitalWrite(triggerPin, HIGH);
  startTime = micros();
  while (micros() - startTime < 10);
  digitalWrite(triggerPin, LOW);

  
  duration = pulseIn(echoPin, HIGH, timeOut);

  if (duration == 0) {
    distance = 1.2;  // We assign max distance to prevent listningen too long time
  } else {
    distance = (duration / 2.0) / 2911.0;  // Calculate distance
  }

  Serial.print(distance,4);


  sum -= measurements[index];
  measurements[index] = distance;
  sum += measurements[index];

  return sum / (M + 1);
}


