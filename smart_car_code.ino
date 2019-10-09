//    Direction of movement with motors
//  ENA   ENB   IN1   IN2   IN3   IN4   Description
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped

//    Left motor truth table
//  ENA         IN1               IN2         Description  
//  LOW   Not Applicable    Not Applicable    Motor is off
//  HIGH        LOW               LOW         Motor is stopped (brakes)
//  HIGH        HIGH              LOW         Motor is on and turning forwards
//  HIGH        LOW               HIGH        Motor is on and turning backwards
//  HIGH        HIGH              HIGH        Motor is stopped (brakes)

//    Right motor truth table
//  ENB         IN3             IN4         Description  
//  LOW   Not Applicable   Not Applicable   Motor is off
//  HIGH        LOW             LOW         Motor is stopped (brakes)
//  HIGH        LOW             HIGH        Motor is on and turning forwards
//  HIGH        HIGH            LOW         Motor is on and turning backwards
//  HIGH        HIGH            HIGH        Motor is stopped (brakes) 
// carAccelerate(float DriveAcceleration, float SteerAcceleration){}
#include <SoftwareSerial.h>
//#include <IRremote.h> 
#include <Servo.h>
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
char tst = 0x00000000;
int carSpeed = 150;
int turnSpeed = 250;
int state = 0;
int servoValue = 90;
int DriveAcceleration = 40; //-100 t/m 100
int SteerAcceleration = 0; //-100 t/m 100
Servo UltraServo;


enum States {
    Null,//0
    Faulty,//1
    Manual,//2
    Auto,//3
    Stop,//4
    Forward,//5
    Backward,//6
    Left,
    Right,
    Servo20deg,
    Servo90deg,
    Servo160deg,
  };

States inBit = Null;

void carAccelerate(int DriveAcceleration, int SteerAcceleration){ 
  if(DriveAcceleration == 0 && SteerAcceleration == 0){ //stop the car
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }//als de ingekomen bits tussen de gegeven parameters zijn:
  else if(DriveAcceleration >= -100 && DriveAcceleration <= 100 && SteerAcceleration >= -100 && SteerAcceleration <= 100){  //drive the car according to the driveacceleration and steeracceleration
      if(DriveAcceleration < 0){ //if the software wants the car to go backward:
        analogWrite(ENA, (int)(-DriveAcceleration * 2.55));//motoren aan op de snelheid van 0 tot 100 (wordt geconvert naar 0 tot 255 voor de motoren)
        analogWrite(ENB, (int)(-DriveAcceleration * 2.55));//idem
        digitalWrite(IN1, LOW); //left motors forward = false
        analogWrite(IN2, HIGH); //left motors backward = true
        analogWrite(IN3, HIGH); //rightmotors backward = true
        digitalWrite(IN4, LOW); //rightmotors forward = false
      }
      else if (DriveAcceleration > 0){ //if the car needs to go to forward:
        analogWrite(ENA, (int)(DriveAcceleration*2.55));
        analogWrite(ENB, (int)(DriveAcceleration*2.55));
        digitalWrite(IN1, HIGH);//left motors forward = true
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); //right motors forward = true
      }
      else if(SteerAcceleration < 0){ //if the car needs to go left:
        analogWrite(ENA, (int)(-SteerAcceleration*2.55));
        analogWrite(ENB, (int)(-SteerAcceleration*2.55));
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);//left motors backward = true
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);//right motors forward = true
      }
      else if(SteerAcceleration > 0){ //if the car needs to go right:
        //go right
      }
    }
  else{
    //do nothing
    }
  }
void fwd(int carSpeed){//movement
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  }
void bwd(int carSpeed){
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void left(int turnSpeed){
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void right(int turnSpeed){
  analogWrite(ENA, turnSpeed);
  analogWrite(ENB, turnSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void stp(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  }
void motorTest(){
  fwd(carSpeed);
  delay(1000);
  left(carSpeed);
  delay(1000);
  right(carSpeed);
  delay(1000);
  bwd(carSpeed);
  delay(1000);
  stp();
}
void getBTdata(){   //0 = Null 1 = Faulty (fault in app or bluetooth) 2/3 = Manual/auto switch
  if (Serial.available()){ // als er bits beschikbaar zijn
    inBit = (States)Serial.read();
    if (inBit == Null){
      //do nothing
    }
    else if(inBit == Faulty){
      Serial.println("Error code 1: Fault with app");
      carAccelerate(0,0);//driveAcceleration = 0 en steerAcceleration = 0 dus stop car
      inBit == Null;
    }
    else if(inBit == Forward){
      Serial.println("Going forward");
      carAccelerate(carSpeed, 0); //carSpeed is in het begin gedefined + 0 is de turnSpeed, dus niks
    }
    else if(inBit == Backward){
      Serial.println("Going backward");
      carAccelerate(-carSpeed, 0); 
    }
    else if(inBit == Left){
      Serial.println("Going left");
      carAccelerate(0,-turnSpeed); 
    }
    else if(inBit == Right){
      Serial.println("Going right");
      carAccelerate(0, turnSpeed);
    }
    else if(inBit == Servo20deg){
      Serial.println("Servo going to 20deg");
      UltraServo.write(20);
      inBit = Null;
    }
    else if(inBit == Servo90deg){
      Serial.println("Servo going to 90deg");
      UltraServo.write(90);
      inBit == Null;
    }
    else if(inBit == Servo160deg){
      Serial.println("Servo going to 160deg");
      UltraServo.write(160);
      inBit = Null;
    }
  }
  delay(20);
}
void setup() {
  Serial.begin(9600); //or 9600 baud
  UltraServo.attach(3);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  //motorTest();
  servoValue = 90;
  UltraServo.write(servoValue); 
   
}

void loop() {
  if(Serial.available() > 0){
    state = Serial.read();}
  /*if (state == "Null"){
    digitalWrite(13, LOW);
  }
  if (state == '0'){
    digitalWrite(13, HIGH);
    stp();
    //state = 0;
    }
  else if(state == '1'){
    digitalWrite(13, HIGH);
    fwd(carSpeed);
    //state = 0;
    }
  else if(state == '2'){
    digitalWrite(13, HIGH);
    Serial.println("Going left");
    left(turnSpeed);
    }
  else if(state == '3'){
    digitalWrite(13, HIGH);
    Serial.println("Going right");
    right(turnSpeed);
  }
  //test
  else if (state == '4'){
    digitalWrite(13, HIGH);
    Serial.println("Servo to 20 degrees");
    servoValue = 20;
    UltraServo.write(servoValue);
    state = "Null";
  }
  else if (state == '5'){
    digitalWrite(13, HIGH);
    Serial.println("Servo to 160 degrees");
    servoValue = 160;
    UltraServo.write(servoValue);
    state = "Null";
  }
  else if (state == '6'){
    Serial.println("Going backward");
    bwd(carSpeed);  
  }
  else if(state == '7'){
    Serial.println("Servo to 90deg");
    UltraServo.write(90);
  }*/
   Serial.println("state is " + (char)(state));
  }
    
