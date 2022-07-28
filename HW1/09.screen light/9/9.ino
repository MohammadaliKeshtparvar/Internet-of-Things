
const int LED_PORT = 9;

const int LDR_PORT = A0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PORT, OUTPUT);
  pinMode(LDR_PORT, INPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PORT);
  int volt = map(ldrValue, 11, 990, 255, 0);
  Serial.println("degree:" + String(ldrValue) + ",volt:" + String(volt) );
  analogWrite(LED_PORT, volt);
}
