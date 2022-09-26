/*
Performing Robots - Personality
Assignments Due Monday September 26
2022/09/23

Created by Yoki Xue and Yupu Chen
*/


const int Pin1 = 5;
const int Pin2 = 6;
const int Pin3 = 10;
const int Pin4 = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode (Pin1, OUTPUT);
  pinMode (Pin2, OUTPUT);
  pinMode (Pin3, OUTPUT);
  pinMode (Pin4, OUTPUT);
}

void loop() {
  digitalWrite(Pin1, LOW);
  analogWrite(Pin2, 70);
  digitalWrite (Pin3, LOW);
  analogWrite (Pin4, 70);
  delay(3000);
  digitalWrite(Pin1, LOW);
  analogWrite(Pin2, 250);
  digitalWrite (Pin3, LOW);
  analogWrite (Pin4, 250);
  delay(5000);
  digitalWrite(Pin2, LOW);
  analogWrite(Pin1, LOW);
  digitalWrite (Pin3, LOW);
  analogWrite (Pin4, 250);
  delay(5000);
}
