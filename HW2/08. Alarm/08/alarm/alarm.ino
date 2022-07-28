#include <SoftwareSerial.h>

SoftwareSerial terminal(19, 18); //RX, TX

const int BUZZER_PIN = 11;
long startTime = 0;
int alertTime = 0;

void setup() {
  Serial.begin(9600);
  terminal.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String received = Serial.readString();
    received.trim();
    terminal.println("message : " + received);
    checkMessage(received);
  }
  alertStatus();
}

void checkMessage(String message) {
  if (message.equals("1*")) {
    alertTime = 5;
    startTime = millis() / 1000;
  } else if (message.equals("2*")) {
    alertTime = 0;
    startTime = 0;
  } else {
    startTime = millis() / 1000;
    alertTime = message.toInt();
  }
}

void alertStatus() {
  if (alertTime == 0) {
    noTone(BUZZER_PIN);
    return;
  }
  if ((millis() / 1000) - startTime > alertTime) {
    tone(BUZZER_PIN, 100);
  } else {
    noTone(BUZZER_PIN);
  }
}
