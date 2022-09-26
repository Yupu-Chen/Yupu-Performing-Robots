/*
Performing Robots - Movements
Assignments Due Monday September 26
2022/09/23

Created by Yupu Chen and Yoki Xue
*/

unsigned long previousMillis = 0;

const long interval = 5000;

int state5 = HIGH;
int state6 = 128;
int state10 = HIGH;
int state11 = 128;

bool direction(){
  if (!(state5 == HIGH && state10 == HIGH)){
    return false;
  } else if (!(state5 == LOW && state10 == LOW)) {
    return true;
  }
}

bool turningAround(){
  if (state5 == state10){
     return true;
  }else{
    return false;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (state5 != state10){
      state5 = HIGH;
      state6 = 128;
      state10 = HIGH;
      state11 = 128;
    }else if (turningAround() && !direction()){ //???
      state5 = HIGH;
      state10 = LOW;
      state6 = 128;
      state11 = 128;
    }else if (direction()){
      state5 = LOW;
      state10 = LOW;
      state6 = 128;
      state11 = 128;
       //digitalWrite(5, state5);
       //digitalWrite(6, state6);
       //digitalWrite(10, state10);
       //digitalWrite(11, state11);
    }else if (!direction()){
      state5 = HIGH;
      state10 = HIGH;
      state6 = 128;
      state11 = 128;
    }
    //if (turningAround() && direction()){ //???
      //state5 = HIGH;
      //state10 = LOW;
      //state6 = 128;
      //state11 = 128;
    //}
  }
  digitalWrite(5, state5);
  analogWrite(6, state6);
  digitalWrite(10, state10);
  analogWrite(11, state11);
}
