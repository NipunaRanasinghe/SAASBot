#include <NewPing.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"

#include <Servo.h>

Servo tools;

#define seeder 178
#define weeder 90
#define soilsensor 8


//See limitations of Arduino SoftwareSerial
SoftwareSerial serial(10, 11);
RoboClaw roboclaw(&serial, 10000);



#define address1 0x80
#define address2 0x81

#define Kp 50.0
#define Ki 5.0
#define Kd 0.0
#define qpps 165000
#define Acceleration 30000


#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


#define PIN_SERVO 5

#define PIN_MOTOR_WTR 52    // Normal - Pullup,Working   -> should be pulled down at the beginning
#define PIN_MOTOR_FERT 53   // Normal - Pullup,Working 
#define PIN_MOTOR_SEED 38   // Normal - Pullup,Not Working  -> should be pulled up ( optional ) at the beginning

#define PIN_STEP_Y 22
#define PIN_DIR_Y 23   //0-forward 1-backward
#define PIN_EN_Y 24

#define PIN_STEP_X 25
#define PIN_DIR_X 26   //0-left  1-right
#define PIN_EN_X 27

#define PIN_SW_X 30
#define PIN_SW_VER 31
#define PIN_SW2_Y 32
#define PIN_SW1_Y 33

#define PIN_VER_EN 48
#define PIN_VER_DIRA 49
#define PIN_VER_DIRB 50 
#define PIN_VER_PWM 8


#define spacing_x 1300
#define spacing_y 1300

#define serverSerial Serial1

uint32_t Speed = 0; //Implements speed variable
uint32_t maxSpeed = 10000;



////////////////////ABSOLUTE_POSITION//////////////////////////
int currentX=2600;
int currentY=0;
int currentCell = 1;
///////////////////////////////////////////////////////////////

int camera_offset[2] = {-400,1850};



int plantmap[13][2] = {{0,0},
  
  {200,2600},
  {1100,2600},
  {2000,2600},
  {200,3900},
  {1100,3900},
  {2000,3900},
  {200,5200},
  {1100,5200},
  {2000,5200},
  {200,6500},
  {1100,6500},
  {2000,6500}
};


void setup() {

  tools.attach(PIN_SERVO);
  tools.write(90);
  
  pinMode(PIN_MOTOR_WTR,OUTPUT);
  digitalWrite(PIN_MOTOR_WTR,LOW);
  pinMode(PIN_MOTOR_FERT,OUTPUT);
  digitalWrite(PIN_MOTOR_FERT,LOW);
  pinMode(PIN_MOTOR_SEED,OUTPUT);
  digitalWrite(PIN_MOTOR_SEED,LOW);
  
  pinMode(PIN_SW1_Y,INPUT_PULLUP);
  pinMode(PIN_SW2_Y,INPUT_PULLUP);
  pinMode(PIN_SW_X,INPUT_PULLUP);
  pinMode(PIN_SW_VER,INPUT_PULLUP);

    
  pinMode(PIN_EN_Y,OUTPUT);
  digitalWrite(PIN_EN_Y,LOW);
  pinMode(PIN_STEP_Y,OUTPUT);
  digitalWrite(PIN_STEP_Y,LOW);
  pinMode(PIN_DIR_Y,OUTPUT);
  digitalWrite(PIN_DIR_Y,LOW);


  pinMode(PIN_EN_X,OUTPUT);
  digitalWrite(PIN_EN_X,LOW);
  pinMode(PIN_STEP_X,OUTPUT);
  digitalWrite(PIN_STEP_X,LOW);
  pinMode(PIN_DIR_X,OUTPUT);
  digitalWrite(PIN_DIR_X,LOW);

  pinMode(PIN_VER_EN,OUTPUT);
  digitalWrite(PIN_VER_EN,LOW);
  pinMode(PIN_VER_DIRA,OUTPUT);
  digitalWrite(PIN_VER_DIRA,HIGH);
  pinMode(PIN_VER_DIRB,OUTPUT);
  digitalWrite(PIN_VER_DIRB,HIGH);
  analogWrite(PIN_VER_PWM,250);

  
  //initial_calib();
  
  
  Serial.begin(115200);
  serverSerial.begin(115200);
  roboclaw.begin(115200);
  delay(1000);
  roboclaw.SetM1VelocityPID(address1, Kp, Ki, Kd, qpps);
  roboclaw.SetM2VelocityPID(address1, Kp, Ki, Kd, qpps);
  roboclaw.SetM1VelocityPID(address2, Kp, Ki, Kd, qpps);
  roboclaw.SetM2VelocityPID(address2, Kp, Ki, Kd, qpps);

  roboclaw.SpeedAccelM1M2(address1, Acceleration, 0, 0);
  roboclaw.SpeedAccelM1M2(address2, Acceleration, 0, 0);
//  
//  delay(1000);
//
//  //Left
//  roboclaw.SpeedAccelM1M2(address1, Acceleration,-maxSpeed, maxSpeed);
//  roboclaw.SpeedAccelM1M2(address2, Acceleration, maxSpeed, -maxSpeed);
//  delay(5000);
//
//  roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
//  roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);
//
//  delay(1000);
//
//  roboclaw.SpeedAccelM1M2(address1, Acceleration,maxSpeed, -maxSpeed);
//  roboclaw.SpeedAccelM1M2(address2, Acceleration,-maxSpeed, maxSpeed);
//  delay(5000);
//  
//
//  roboclaw.SpeedAccelM1M2(address1, Acceleration, 0,0);
//  roboclaw.SpeedAccelM1M2(address2, Acceleration, 0,0);

  




  





  
 
 initial_calib();
 delay(1000);
 //check_tools();

 
 
// move_x_y(1000,3900);
// delay(1000);
// move_down(9000);
// //vertical_adjust(15);
// 
// delay(3000);
// move_down(1000);
// //vertical_adjust(7);
// delay(1000);
// //vertical_adjust(15);
// move_up(1000);
// delay(2000);
// move_x_y(1000+250,3900-1500);
// 
// auto_planting(1,8,3);
 delay(2000);
 //auto_planting(1,12,3);
 
 
  
  while(1){
    check_data();
  }
  delay(100);
  sendPacket_EMG();
  while(1){
    //readPacket();
    delay(100);
  }
  

  
  //vertical_adjust(14);
  //delay(500);

 
//   while(1){}
//   
//   step_x_y_smooth(1400,400,1,1400,400,0);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,1,1400,400,0);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,1,2800,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,1,0,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,1,0,400,1);
//   delay(2000);
//
//
//   step_x_y_smooth(1400,400,0,0,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,0,0,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,0,0,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,0,0,400,1);
//   delay(2000);
//
//   step_x_y_smooth(1400,400,0,0,400,1);
//   delay(2000);
//   

  //step_x_y_smooth(1000,400,1,2700,400,1);

  

  while(1){}
  
  step_x_calib(500);
  

  
  delay(500);
  step_y_smooth(5000,400,1);
  delay(1000);
  step_y_calib(400);

  while(1){}

//
    step_y_smooth(1000,400,0);
//
    delay(2000);
//
  step_y_smooth(1000,400,1);
//  

  delay(20000);
  
/*
  Step_X(300,1000,0);
 // delay(500);

  
  for(int x=0;x<1;x++){
  Step_Y(10000,30,0);
  //delay(500);
  }

  Step_X(300,1000,0);
 // delay(500);

  
  for(int x=0;x<1;x++){
  Step_Y(10000,30,0);
  //delay(500);
  }

  Step_X(300,1000,1);
  //delay(500);


  for(int x=0;x<1;x++){
  Step_Y(10000,30,1);
 // delay(500);
  }

  Step_X(300,1000,1);
  //delay(500);


  for(int x=0;x<1;x++){
  Step_Y(10000,20,1);
  delay(500);
  }
//  Step(1000,500,1);
//  delay(500);
//  Step(1000,500,1);
//  delay(500);
//  Step(1000,500,1);
//  delay(500);
//  Step(1000,500,1);
//  delay(500);
//  

  */

}

void loop() {
  
}


void Step_Y(int count,int Delay,bool Dir){            //count, pulse_delay(millis), dir
  digitalWrite(PIN_EN_Y,HIGH);
  digitalWrite(PIN_DIR_Y,Dir);
  
  for(int x=0; x<count; x++){
    digitalWrite(PIN_STEP_Y,HIGH);
    delayMicroseconds(Delay);
    digitalWrite(PIN_STEP_Y,LOW);
    delayMicroseconds(Delay);
  }

  digitalWrite(PIN_EN_Y,LOW);
}


void Step_X(int count,int Delay,bool Dir){            //count, pulse_delay(millis), dir
  digitalWrite(PIN_EN_X,HIGH);
  digitalWrite(PIN_DIR_X,Dir);
  
  for(int x=0; x<count; x++){
    digitalWrite(PIN_STEP_X,HIGH);
    delayMicroseconds(Delay);
    digitalWrite(PIN_STEP_X,LOW);
    delayMicroseconds(Delay);
  }

  digitalWrite(PIN_EN_X,LOW);
}
