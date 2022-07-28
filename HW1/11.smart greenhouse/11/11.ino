#include <Servo.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {8, 7, 6};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Servo myServo[3];
const int FLAME_SENSOR_PIN = 12;
const int BUZZER_PIN = 13;
bool isManual = false;
String input = "";
String degree = "", servoIndex = "";

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  myServo[0].attach(9);
  myServo[1].attach(10);
  myServo[2].attach(11);
  Serial.begin(9600);
}

void loop() {
  checkFlameSensor();
  char customKey = keypad.getKey();
  if (customKey) {
    if (customKey == '#' && !isManual) {
      Serial.println("**Manual Mode**");
      isManual = true;
    } else if (customKey == '#' && isManual) {
      Serial.println("**Auto Mode**");
      isManual = false;
    } else if (customKey != '#' && customKey != '*') {
      input += customKey;
      if (input.length() > 1 && input.length() < 4) {
        degree += customKey;
      } else if (input.length() == 1) {
        servoIndex += customKey;
      }
    } else if (customKey == '*') {
      int servoDegree = map((degree.toInt() % 101), 0, 100, 0, 180);
      Serial.println("servo number:" + servoIndex);
      Serial.println("degree:" + degree);
      myServo[(servoIndex.toInt() % 4) - 1].write(servoDegree);
      input = "", degree = "", servoIndex = "";
    }
  }

  if (!isManual) {
    int deg1 = map(analogRead(A0), 0, 1023, 0, 180);
    int deg2 = map(analogRead(A1), 0, 1023, 0, 180);
    int deg3 = map(analogRead(A2), 0, 1023, 0, 180);
    myServo[0].write(deg1);
    myServo[1].write(deg2);
    myServo[2].write(deg3);
  }

}

void checkFlameSensor() {
  if (digitalRead(FLAME_SENSOR_PIN) == HIGH) {
    flameWarnning();
  }
}

void flameWarnning() {
  Serial.println("Flame Warnning");
  tone(BUZZER_PIN, 1000);
  for (int i = 0; i < 3; i++)
    myServo[i].write(180);
  delay(1000);
  noTone(BUZZER_PIN);
  for (int i = 0; i < 3; i++)
    myServo[i].write(0);
}
