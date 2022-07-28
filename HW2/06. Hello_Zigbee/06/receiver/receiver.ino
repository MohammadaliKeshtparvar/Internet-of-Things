#include <SoftwareSerial.h>

SoftwareSerial terminal(19, 18); //RX, TX

const String PASSWORD = "1234";
const int GREEN_LED_PIN = 13;
const int RED_LED_PIN = 12;
const int BUZZER_PIN = 11;


void setup() {
  Serial.begin(9600);
  terminal.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String received = Serial.readString();
    received.trim();
    terminal.println("message : " + received);
    checkMessage(received);
  }
}

void checkMessage(String message) {
  message.trim();
  if (PASSWORD.equals(message)) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
    delay(3000);
    noTone(BUZZER_PIN);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
}
