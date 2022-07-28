#include <SoftwareSerial.h>
#include <Keypad.h>

SoftwareSerial terminal(19, 18); //RX, TX
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
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String message = "";

void setup() {
  Serial.begin(9600);
  terminal.begin(9600);
}

void loop() {

  char inputKey = keypad.getKey();
  if (inputKey) {
    if (inputKey == '#') {
      terminal.println("send message : " + message);
      Serial.print(message);
      message = "";
    } else {
      message += inputKey;
      terminal.println(message);
      checkCommand();
    }
  }
}

void checkCommand() {
  if (message.equals("1*")) {
    terminal.println("send message : " + message);
    Serial.print(message);
    message = "";
  } else if (message.equals("2*")) {
    terminal.println("send message : " + message);
    Serial.print(message);
    message = "";
  }
}
