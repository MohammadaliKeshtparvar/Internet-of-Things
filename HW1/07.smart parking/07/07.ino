#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

#define USER_SIZE 10
#define SERVO_PIN 40
#define COST_PER_MIN 100

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns

const char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {3, 2, 14}; //connect to the column pinouts of the keypad

Servo myServo;
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String username_password[2][USER_SIZE] = {
  { "0000", "1111", "2222", "3333", "4444", "5555", "6666", "7777", "8888"},
  { "1111", "2222", "3333", "4444", "5555", "6666", "7777", "8888", "9999"}
};

String insideCars[3][USER_SIZE];
int parkingStatus[3][3];

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String input = "";
String username = "";
String password = "";
int totalCapacity = 9, aCapacity = 3, bCapacity = 3, cCapacity = 3;

#define PIR_PIN 22
#define LED_PIN 23

const int PIR_PIN_A1 = 22;
const int PIR_PIN_A2 = 24;
const int PIR_PIN_A3 = 26;
const int PIR_PIN_B1 = 28;
const int PIR_PIN_B2 = 30;
const int PIR_PIN_B3 = 32;
const int PIR_PIN_C1 = 34;
const int PIR_PIN_C2 = 36;
const int PIR_PIN_C3 = 38;

const int LED_PIN_A1 = 23;
const int LED_PIN_A2 = 25;
const int LED_PIN_A3 = 27;
const int LED_PIN_B1 = 29;
const int LED_PIN_B2 = 31;
const int LED_PIN_B3 = 33;
const int LED_PIN_C1 = 35;
const int LED_PIN_C2 = 37;
const int LED_PIN_C3 = 39;

void setup() {
  pinMode(PIR_PIN_A1, INPUT);
  pinMode(PIR_PIN_A2, INPUT);
  pinMode(PIR_PIN_A3, INPUT);
  pinMode(PIR_PIN_B1, INPUT);
  pinMode(PIR_PIN_B2, INPUT);
  pinMode(PIR_PIN_B3, INPUT);
  pinMode(PIR_PIN_C1, INPUT);
  pinMode(PIR_PIN_C2, INPUT);
  pinMode(PIR_PIN_C3, INPUT);

  pinMode(LED_PIN_A1, OUTPUT);
  pinMode(LED_PIN_A2, OUTPUT);
  pinMode(LED_PIN_A3, OUTPUT);
  pinMode(LED_PIN_B1, OUTPUT);
  pinMode(LED_PIN_B2, OUTPUT);
  pinMode(LED_PIN_B3, OUTPUT);
  pinMode(LED_PIN_C1, OUTPUT);
  pinMode(LED_PIN_C2, OUTPUT);
  pinMode(LED_PIN_C3, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 4);
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  lcdDefaultMessage(true);
}

void loop() {
  checkSensors();
  char customKey = keypad.getKey();
  if (customKey) {
    if (customKey != '*' && customKey != '#') {
      input += customKey;
      if (input.length() < 5) {
        username += customKey;
        lcdMessage(username, 3, 2);
      } else if (input.length() < 9) {
        password += customKey;
        lcdMessage(password, 9, 3);
      } else {
        input = "";
      }
      Serial.println(input);
    } else if (customKey == '*') {
      if (checkIntrance()) {
        insideCars[0][9 - totalCapacity] = username;
        insideCars[1][9 - totalCapacity] = password;
        insideCars[2][9 - totalCapacity] = String(( millis() / 1000 ));
        //        insideCars[2][9 - totalCapacity] = String(( millis() / 1000 ) / 60);
        totalCapacity--;
        lcd.clear();
        lcd.print("Welcome");
        myServo.write(90);
        delay(400);
        myServo.write(0);
      }
      input = "";
      username = "";
      password = "";
    } else {
      checkExitance();
      input = "";
      username = "";
      password = "";
    }
  }
}

void checkSensors() {
  for (int i = 1; i < 10; i++) {
    if (digitalRead((i * 2) + 20) == HIGH) {
      digitalWrite((i * 2) + 21, HIGH);
      parkingStatus[(i - 1) / 3][(i - 1) % 3] = 1;
    } else {
      digitalWrite((i * 2) + 21, LOW);
      parkingStatus[(i - 1) / 3][(i - 1) % 3] = 0;
    }
  }
  lcdDefaultMessage(false);
}

bool checkIntrance() {
  for (int i = 0; i < USER_SIZE; i++) {
    if (username_password[0][i].equals(username)) {
      if (username_password[1][i].equals(password)) {
        if (totalCapacity <= 0) {
          lcdMessage("Full capacity");
          return false;
        }
        return true;
      } else {
        lcdMessage("Wrong password");
        return false;
      }
    }
  }
  lcdMessage("Wrong username");
  return false;
}

void checkExitance() {
  for (int i = 0; i < USER_SIZE; i++) {
    if (insideCars[0][i].equals(username)) {
      if (insideCars[1][i].equals(password)) {
        int cost = ((( millis() / 1000 )) - insideCars[2][i].toInt()) * COST_PER_MIN;
        //        int cost = ((( millis() / 1000 ) / 60) - insideCars[2][i].toInt()) * COST_PER_MIN;
        totalCapacity++;
        lcd.clear();
        lcd.print("cost:" + String(cost));
        myServo.write(90);
        delay(400);
        myServo.write(0);
        lcdDefaultMessage(true);
        insideCars[0][i] = "";
        insideCars[1][i] = "";
        insideCars[2][i] = "";
        return;
      } else {
        lcdMessage("Wrong password");
        return;
      }
    }
  }
  lcdMessage("Wrong username");
}

void lcdMessage(String message, int locX, int locY) {
  lcd.setCursor(locX, locY);
  lcd.print(message);
}

void lcdMessage(String message) {
  lcd.clear();
  lcd.print(message);
  delay(200);
  lcdDefaultMessage(true);
}

void lcdDefaultMessage(bool isTotalCapacity) {
  int a = 0, b = 0, c = 0;
  capacityStatus(&a, &b, &c);
  if (a != aCapacity || b != bCapacity || c != cCapacity || isTotalCapacity) {
    aCapacity = a, bCapacity = b, cCapacity = c;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A:" + String(a));
    lcd.setCursor(8, 0);
    lcd.print("B:" + String(b));
    lcd.setCursor(0, 1);
    lcd.print("C:" + String(c));
    lcd.setCursor(8, 1);
    lcd.print("T:" + String(totalCapacity));
    lcd.setCursor(0, 2);
    lcd.print("ID:");
    lcd.setCursor(0, 3);
    lcd.print("PASSWORD:");
  }
}

void capacityStatus(int *a, int *b, int * c) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (parkingStatus[i][j] == 0) {
        switch (i) {
          case 0:
            *a += 1;
            break;
          case 1:
            *b += 1;
            break;
          case 2:
            *c += 1;
            break;
        }
      }
    }
  }
}
