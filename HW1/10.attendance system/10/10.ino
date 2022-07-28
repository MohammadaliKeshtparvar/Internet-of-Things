#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

#define STUDENT_NUMBER 10

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[rows] = {4, 3, 2, 14}; //connect to the row pinouts of the keypad
byte colPins[cols] = {5, 6, 7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const String instructorId = "1111";
const String studentsId[STUDENT_NUMBER] = {"9701", "9702", "9703", "9704", "9705", "9706", "9707", "9708", "9709", "9710"};


//Global variables
unsigned long startTime = 0;
unsigned long finishTime = 0;
bool isClassTime = false;
String inputId = "";
String *attendanceList = (const String *)malloc(STUDENT_NUMBER);
int attendanceNumber = 0;
Servo myServo;

const int buzzerPin = 15;
const int ledPin = 23;
const int servoPin = 22;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  myServo.attach(servoPin);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  lcdMessage("");
}

void loop() {
  // put your main code here, to run repeatedly:
  char customKey = keypad.getKey();
  if (customKey) {
    if (customKey != '*' && customKey != '#') {
      inputId += customKey;
      lcd.print(customKey);
      Serial.println("inputId:" + inputId);
    } else if (customKey == '*') {
      checkEntrance();
      inputId = "";
    } else {
      checkExitance();
      inputId = "";
    }
  }

  if (checkFinishTime()) {
    printAttandanceList();
    printAbsentList();
    clearAttandanceList();
  }
}

void lcdMessage(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ID : ");
  if (message != "") {
    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(5, 0);
  }
}

void checkEntrance() {
  if (inputId.equals(instructorId) && !isClassTime) {
    startTime = millis();
    finishTime = 0;
    isClassTime = true;
    lcdMessage("The class started");
    openDoor();
  } else if (isClassTime && isInStudentList() && !isInAttandanceList() && isValidTime()) {
    attendanceList[attendanceNumber++] = inputId;
    inputId = "";
    lcdMessage("Welcome");
    openDoor();
  } else if (!isClassTime) {
    Serial.println("Isn't class time");
    lcdMessage("Isn't class time");
  } else if (!isInStudentList()) {
    Serial.println("Wrong ID");
    lcdMessage("Wrong ID");
    buzzerActivation();
  } else if (!isValidTime()) {
    Serial.println("Long delay");
    lcdMessage(convertMillisToMinuts(millis() - startTime));
    buzzerActivation();
  } else if (isInAttandanceList()) {
    Serial.println("ID is present");
    lcdMessage("ID is present");
    buzzerActivation();
  }
}

String convertMillisToMinuts(unsigned long milliSecond) {
  int seconds = (int) (milliSecond / 1000) % 60 ;
  int minutes = (int) ((milliSecond / (1000 * 60)) % 60);
  int hours   = (int) ((milliSecond / (1000 * 60 * 60)) % 24);
  String result = "h:" + String(hours) + ",m:" + String(minutes) + ",s:" + String(seconds);
  return result;
}

void checkExitance() {
  if (inputId.equals(instructorId) && isClassTime) {
    startTime = 0;
    finishTime = millis();
    lcdMessage("Go out please");
    openDoor();
  } else if (finishTime != 0 && isInStudentList() && isInAttandanceList()) {
    inputId = "";
    lcdMessage("Go out please");
    openDoor();
  } else if (finishTime == 0) {
    Serial.println("Is class time");
    lcdMessage("Is class time");
  } else if (!isInStudentList()) {
    Serial.println("Wrong ID");
    lcdMessage("Wrong ID");
    buzzerActivation();
  } else if (!isInAttandanceList()) {
    Serial.println("ID isn't present");
    lcdMessage("ID isn't present");
    buzzerActivation();
  }
}

void buzzerActivation() {
  Serial.println("buzzer");
  tone(buzzerPin, 2000);  // Send 1KHz sound signal...
  delay(1000);            // ...for 1 sec
  noTone(buzzerPin);      // Stop sound...
}

void openDoor() {
  myServo.write(0);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  myServo.write(90);
  digitalWrite(ledPin, LOW);
}

bool isInStudentList() {
  for (int i = 0; i < STUDENT_NUMBER; i++)
    if (studentsId[i].equals(inputId))
      return true;
  return false;
}

bool isInAttandanceList() {
  for (int i = 0; i < STUDENT_NUMBER; i++)
    if (attendanceList[i] != "" && inputId.equals(attendanceList[i]))
      return true;
  return false;
}

bool isInAttandanceList(String studentId) {
  for (int i = 0; i < STUDENT_NUMBER; i++)
    if (attendanceList[i] != "" && studentId.equals(attendanceList[i]))
      return true;
  return false;
}

bool isValidTime() {
  if (startTime == 0)
    return false;
  return (millis() - startTime < 20000);
}

bool checkFinishTime() {
  if (finishTime == 0)
    return false;
  if (millis() - finishTime > 10000 && isClassTime) {
    isClassTime = false;
    lcdMessage("The class is over");
    finishTime = 0;
    return true;
  }
  return false;
}

void printAttandanceList() {
  Serial.println("Attandance List:");
  for (int i = 0; i < attendanceNumber; i++) {
    if (attendanceList[i] != "")
      Serial.println(attendanceList[i]);
  }
  Serial.println();
}

void printAbsentList() {
  Serial.println("Absent List:");
  for (int i = 0; i < STUDENT_NUMBER; i++) {
    if (!isInAttandanceList(studentsId[i])) {
      Serial.println(studentsId[i]);
    }
  }
  Serial.println();
}

void clearAttandanceList() {
  for (int i = 0; i < STUDENT_NUMBER; i++) {
    attendanceList[i] = "";
  }
  attendanceNumber = 0;
}
