#include <Servo.h>

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor

int distance_list[20] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
long index = 0;
long timePeriod = 0;

Servo myServo;
const int SERVO_PORT = 8;
long openTime = 0;

void setup() {
  Serial.begin(9600); // Starting Serial Terminal
  myServo.attach(SERVO_PORT);
  myServo.write(0);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  if (millis() - timePeriod > 150) {
    duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    distance_list[index % 20] = distance;
    index++;
    if (averageDistance() < 30) {
      openTime = millis() / 1000;
      myServo.write(90);
    }
    Serial.println("distance=" + String(distance));
    timePeriod = millis();
  }
  if ((millis() / 1000) - openTime >= 1) {
    openTime = 0;
    myServo.write(0);
  }
}

int averageDistance() {
  int sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += distance_list[i];
  }
  return sum / 20;
}
