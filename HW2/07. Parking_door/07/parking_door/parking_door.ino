#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial terminal(19, 18); //RX, TX
const int PING_PIN = 7; // Trigger Pin of Ultrasonic Sensor
const int ECHO_PIN = 6; // Echo Pin of Ultrasonic Sensor
const int SERVO_PIN = 8;
const int RED_LED_PIN = 12;
Servo myServo;
bool isOpen = false;
int openTime = 0;


void setup() {
  Serial.begin(9600);
  terminal.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(PING_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  if (isOpen) {
    if (((millis() / 1000) - openTime) >= 10) {
      closeDoor();
    }
  }
  if (Serial.available() > 0) {
    String received = Serial.readString();
    received.trim();
    terminal.println("message : " + received);
    checkMessage(received);
  }
}

void checkMessage(String message) {
  message.trim();
  if (message.equals("1")) {
    openDoor();
  } else if (message.equals("3")) {
    closeDoor();
  }
}

int calcDistance() {
  long duration;
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(PING_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void openDoor() {
  isOpen = true;
  myServo.write(90);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  openTime = millis() / 1000;
}

void closeDoor() {
  int distance = calcDistance();
  if (distance > 30) {
    myServo.write(0);
    digitalWrite(RED_LED_PIN, HIGH);
  }
  isOpen = false;
  distance = calcDistance();
  int startTime = millis() / 1000;
  while (((millis() / 1000) - startTime) < 1) {
    distance = calcDistance();
    terminal.println("distance=" + String(distance));
    if (distance < 30) {
      isOpen = true;
      myServo.write(90);
      openTime = millis() / 1000;
      delay(300);
    }
  }
  digitalWrite(RED_LED_PIN, LOW);
}
