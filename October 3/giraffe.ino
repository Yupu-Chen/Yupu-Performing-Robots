/*
Performing Robots - Hobby RC system
Assignments Due Monday September 26
2022/09/30

Created by Yupu Chen and Yoki Xue

Reference:
   Minimal decoding of multiple RC channels using pin change interrupts
   Does not check for reasonable values or timeouts
   Based on https://ryanboland.com/blog/reading-rc-receiver-values
*/



#include <EnableInterrupt.h>
#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2
#define RC_CH4  3

#define RC_CH1_PIN  8
#define RC_CH2_PIN  7
#define RC_CH3_PIN  4
#define RC_CH4_PIN  2

const int Pin1 = 5;
const int Pin2 = 6;
const int Pin3 = 10;
const int Pin4 = 11;

int state1 = LOW;
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() {
  calc_input(RC_CH1, RC_CH1_PIN);
}
void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_PIN);
}
void calc_ch3() {
  calc_input(RC_CH3, RC_CH3_PIN);
}
void calc_ch4() {
  calc_input(RC_CH4, RC_CH4_PIN);
}

int speed;

//turn right for going forward
//slow down the right motor
void turnRight(){
  state1 = LOW;
  state2= speed;
  state3 = LOW;
  state4 = (speed - 40)/2;
}


//turn left for going forward
//slow down the left motor
void turnLeft(){
  state1 = LOW;
  state2= speed/2;
  state3 = LOW;
  state4 = (speed - 40);
}


//turn right for going backward
//speed up the right motor
void turnRightBack(){
  state1 = HIGH;
  state2 = speed;
  state3 = HIGH;
  state4 = (state2 + 45) - 40;
}

//turn left for going backward
//speed up the left motor and slow down the right motor
void turnLeftBack(){
  state1 = HIGH;
  state2 = speed/3;
  state3 = HIGH;
  state4 = state2 + 45 + 80;
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);
  pinMode(13, OUTPUT);

  enableInterrupt(RC_CH1_PIN, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_PIN, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_PIN, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_PIN, calc_ch4, CHANGE);
}

void loop() {
  rc_read_values();

  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
  Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]);

 //When Chanel 1 value is bigger than 1550 (Turn the knob forward)
 //Move forward
  if(rc_values[RC_CH1]>1550) {
    
    //map the read value from 900-2050 range to 90-255 range)
    
    speed=map(rc_values[RC_CH2],  900, 2050, 90,255);

    //if the Chanel 3 value is smaller than 1100 (Turn the knob backward)
    if(rc_values[RC_CH3] < 1100)
    {
      turnRight();
    //if the Chanel 3 value is bigger than 2000 (Turn the knob forward)
    }else if(rc_values[RC_CH3] > 2000){
      turnLeft();
    }
    else{
      state1 = LOW;
      state2= speed;
      state3 = LOW;
      state4 = (speed - 40);}}
      
 //When Chanel 1 value is smaller than 1500 (Turn the knob backward)
 //Move backward    
   else if (rc_values[RC_CH1] < 1500){
    
    //map the read value from 900-2050 range to 165-0 range)
    speed=map(rc_values[RC_CH2],  900, 2050, 165,0);

    //if the Chanel 3 value is smaller than 1100 (Turn the knob backward)
    if(rc_values[RC_CH3] < 1100){
      turnRightBack();
      //if the Chanel 3 value is bigger than 2000 (Turn the knob forward)
    }else if(rc_values[RC_CH3] > 2000){
      turnLeftBack();
    }else{
      state1 = HIGH;
      state2 = speed;
      state3 = HIGH;
      state4 = state2 + 45;
      }
  }

  //stay still
  else{
    state1 = LOW;
    state2 = LOW;
    state3 = LOW;
    state4 = LOW;
  }
  
  delay(200);
  digitalWrite(Pin1, state1);
  analogWrite(Pin2, state2);
  digitalWrite (Pin3, state3);
  analogWrite (Pin4, state4);
}
