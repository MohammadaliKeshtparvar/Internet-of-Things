#include <SoftwareSerial.h>
#include <BlynkSimpleStream.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPL-my_Xkhc"
#define BLYNK_DEVICE_NAME "SecuritySystem"
#define BLYNK_AUTH_TOKEN "huphUsFtETl_gd9hn36Qd1NlWom78n35"

#define BLYNK_PRINT Serial1

const int PING_PIN = 7; // Trigger Pin of Ultrasonic Sensor
const int ECHO_PIN = 6; // Echo Pin of Ultrasonic Sensor
const char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;
SoftwareSerial terminal(19, 18); //RX, TX
Servo myServo;

int distance = 50;
const int LED_PIN = 9;
const int BUZZER_PIN = 10;
const int SERVO_PIN = 11;
bool alertIsOn = false;
int servoDegree = 0;


void myTimerEvent() {
  int lastDis = distance;
  calcDistance();
  checkSecurity(lastDis);
  terminal.println("distance:" + String(distance));
  Blynk.virtualWrite(V2, distance);
  Blynk.virtualWrite(V1, alertIsOn ? 1 : 0);
}

BLYNK_WRITE(V1) {
  int val = param.asInt(); // assigning incoming value from pin V1 to a variable
  alertIsOn = (val == 1);
  terminal.println("alert: " + String(alertIsOn));
}

BLYNK_WRITE(V0) {
  servoDegree = param.asInt(); // assigning incoming value from pin V1 to a variable
  terminal.println("alert: " + String(servoDegree));
}


void setup() {
  pinMode(PING_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  terminal.begin(9600);
  Serial.begin(9600);
//  Serial1.begin(115200);
  Blynk.begin(Serial, auth);
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  pinMode(LED_PIN, OUTPUT);
  myServo.attach(SERVO_PIN);
  myServo.write(servoDegree);
}

void loop() {
  Blynk.run();
  timer.run();
  myServo.write(servoDegree);
  alert();
}


int calcDistance() {
  long duration;
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(PING_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  distance += 2;
}

void checkSecurity(int lastDis) {
  if (lastDis < 30 && (distance - lastDis > 4 || lastDis - distance > 4) && lastDis != distance) {
    alertIsOn  = true;
  }
}

void alert() {
  if (alertIsOn) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
}
