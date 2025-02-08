#include <Servo.h>

Servo dist_servo;

const int trigPin = 45;
const int echoPin = 47;
float duration;
float distance;

unsigned long previousMillis = 0;
unsigned long previousServoMillis = 0;
unsigned long previousDistMillis = 0;
const unsigned long interval = 1000; // interval at which to measure distance and move servo

enum State {
  MEASURE_DIST,
  SERVO_0_TO_90,
  SERVO_90_TO_180,
  SERVO_180_TO_90,
  SERVO_90_TO_0
};

State currentState = MEASURE_DIST;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  dist_servo.attach(36);
  dist_servo.write(0);  // Initialize the servo position to 0
}

void loop() {
  unsigned long currentMillis = millis();

  // State machine for servo movement and distance measurement
  switch (currentState) {
    case MEASURE_DIST:
      if (currentMillis - previousDistMillis >= interval) {
        previousDistMillis = currentMillis;
        measureDistance();
        currentState = SERVO_0_TO_90;
        previousServoMillis = currentMillis; // Reset servo timing
      }
      break;

    case SERVO_0_TO_90:
      if (currentMillis - previousServoMillis >= interval) {
        previousServoMillis = currentMillis;
        dist_servo.write(90);
        Serial.println(dist_servo.read());
        measureDistance();
        currentState = SERVO_90_TO_180;
      }
      break;

    case SERVO_90_TO_180:
      if (currentMillis - previousServoMillis >= interval) {
        previousServoMillis = currentMillis;
        dist_servo.write(180);
        Serial.println(dist_servo.read());
        measureDistance();
        currentState = SERVO_180_TO_90;
      }
      break;

    case SERVO_180_TO_90:
      if (currentMillis - previousServoMillis >= interval) {
        previousServoMillis = currentMillis;
        dist_servo.write(90);
        Serial.println(dist_servo.read());
        measureDistance();
        currentState = SERVO_90_TO_0;
      }
      break;

    case SERVO_90_TO_0:
      if (currentMillis - previousServoMillis >= interval) {
        previousServoMillis = currentMillis;
        dist_servo.write(0);
        Serial.println(dist_servo.read());
        measureDistance();
        currentState = MEASURE_DIST;
      }
      break;
  }
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance in cm: ");
  Serial.println(distance);
}