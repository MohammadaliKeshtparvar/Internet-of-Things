#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL2yExyn2M"
#define BLYNK_DEVICE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "2OVg4-pSQ-ESrCwZglQWEwPstawASLUp"

const char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
SoftwareSerial terminal(19, 18); //RX, TX
Servo myServo;
int analogValue;
int currnetTemperature = 50;
int expectedTemperature = 50;
const int LED_PIN = 10;
const int SERVO_PIN = 11;
bool flag = false;

void myTimerEvent() {
  analogValue = analogRead(A3);
  float mv = ( analogValue * 500.0) / 1024;
  currnetTemperature = mv;
  terminal.println("currnetTemperature: " + String(currnetTemperature));
  Blynk.virtualWrite(V1, currnetTemperature);
}

BLYNK_WRITE(V0) {
  expectedTemperature = param.asInt(); // assigning incoming value from pin V1 to a variable
  terminal.println("expectedTemperature: " + String(expectedTemperature));
}


void setup() {
  terminal.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  myServo.write(0);
}

void loop() {
  int servoDegree = map(expectedTemperature, 0, 150, 0, 180);
  myServo.write(servoDegree);
  Blynk.run();
  timer.run();
  checkStatus();
}

void checkStatus() {
  if (expectedTemperature - currnetTemperature >= 3) {
    digitalWrite(LED_PIN, HIGH);
  } else if (expectedTemperature - currnetTemperature <= -3) {
    digitalWrite(LED_PIN, LOW);
  }
}
