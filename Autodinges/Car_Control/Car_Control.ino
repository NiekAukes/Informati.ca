//    Direction of movement with motors
//  ENA   ENB   IN1   IN2   IN3   IN4   Description
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped

//Hex values: https://www.ascii-code.com/

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
            //=====================================================\\
           //       60deg           90deg          120deg           \\
          //           60deg        90deg        120deg              \\
         //               60deg     90deg      120deg                 \\
        //                   60deg           120deg                    \\
       //    20deg       20deg     ▄__▄     160deg          160deg     \\
      //                    /=====/SENSOR\====\                          \\
     //                    /''''''\______/'''''\                          \\
    //                    |'''''''''CAR''''''''|                           \\
   //                     |'''''''''CAR''''''''|                            \\
  //                     /''''''''''CAR'''''''''\                            \\
 //                     |'''''''''''CAR''''''''''|                            \\
//=============================================================================\\
#include <SoftwareSerial.h>
//#include <IRremote.h>
#include <Servo.h>
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Voor de afstandssensor:
#define triggerPin A5
#define echoPin A4
long duration,distance;
int distance20,distance60,distance90,distance120,distance160;
int Distances[5] = {distance20,distance60,distance90,distance120,distance160};
int BelowThreshold[5] = {false, false, false, false, false};
int ServoWrites[5] = {20,60,90,120,160};
int MaxDistance = 300; //IN CENTIMETRES
int distanceThreshold = 10; //in centimetres
char NextAction[] = "Null";
Servo UltraServo;
char tst = 0x00000000;
int servoValue = 90;
int DriveAcceleration = 0; //-100 t/m 100
int SteerAcceleration = 0; //-100 t/m 100


enum States {
    Null,//00 hex
    Faulty,//01 hex
    Manual,//02 hex
    Auto,//03 hex
    Stop,//04 hex
    Forward,//05 hex
    Backward,//06 hex
    Left,//07 hex
    Right,//08 hex
    CheckDistance, //for distance meter, hex 09
    Servo20deg = 57,//9 symbol 
    Servo60deg = 58,//: symbol
    Servo90deg = 59,//; symbol
    Servo120deg = 60,//< symbol
    Servo160deg = 61,//= symbol
    BeginAccelerationRange = 20,
    EndAccelerationRange = 120,
    BeginSteeringRange = 130,
    EndSteeringRange = 230
  };
States inBit = Null;

void carAccelerate(int carSpeed, int steerSpeed){ 
  if(carSpeed == 0 && steerSpeed == 0){ //stop the car
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }//als de ingekomen bits tussen de gegeven parameters zijn:
  else if(carSpeed >= -100 && carSpeed <= 100 && steerSpeed >= -100 && steerSpeed <= 100){  //drive the car according to the driveacceleration and steeracceleration
      if(carSpeed < 0){ //if the software wants the car to go backward:
        analogWrite(ENA, (int)(-carSpeed * 2.55));//motoren aan op de snelheid van 0 tot 100 (wordt geconvert naar 0 tot 255 voor de motoren)
        analogWrite(ENB, (int)(-carSpeed * 2.55));//idem
        digitalWrite(IN1, LOW); //left motors forward = false
        digitalWrite(IN2, HIGH); //left motors backward = true
        digitalWrite(IN3, HIGH); //rightmotors backward = true
        digitalWrite(IN4, LOW); //rightmotors forward = false
      }
      else if (carSpeed > 0){ //if the car needs to go to forward:
        analogWrite(ENA, (int)(carSpeed*2.55));
        analogWrite(ENB, (int)(carSpeed*2.55));
        digitalWrite(IN1, HIGH);//left motors forward = true
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); //right motors forward = true
      }
      else if(steerSpeed < 0){ //if the car needs to go left:
        Serial.write("Got to motors");
        analogWrite(ENA, (int)(-steerSpeed*2.55));
        analogWrite(ENB, (int)(-steerSpeed*2.55));
        Serial.write("motors on");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);//left motors backward = true
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);//right motors forward = true
      }
      else if(steerSpeed > 0){ //if the car needs to go right:
        analogWrite(ENA, (int)(steerSpeed*2.55));
        analogWrite(ENB, (int)(steerSpeed*2.55));
        digitalWrite(IN1,HIGH);//left motors forward = true
        digitalWrite(IN2,LOW);
        digitalWrite(IN3,HIGH);//right motors backward = true
        digitalWrite(IN4,LOW);
      }
  else{
    //do nothing
    }
  }
}
bool GetDistance(int *returnvalue){ //sensor is triggered by HIGH pulse more or equal than 10 microseconds
  digitalWrite(triggerPin, LOW); //to ensure clean high pulse
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343/*=speed of sound in cm/microsecond*/; //in centimetres
  returnvalue = distance;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(".");
  return true;
}
  //                Wall       //                 //                 //                     //       Wall                   ETC.
  //          Wall        Wall //    Wall Wall    //  Wall   Wall    //  Wall        Wall   //            Wall              ETC.
  //          Wall  |--|  Wall //    |--| Wall    //  Wall   |--|    //  Wall  |--|  Wall   //     |--|   Wall              ETC.
  // |--|=car    go backward   //     go left     //       go right  //     go forward      //go fwd with 30deg left turn   ETC.
int BackWardsBoolList[5] = {true, true, true, true, true};
int ForwardsBoolList1[5] = {true, false, false, false, true};
int ForwardsBoolList2[5] = {true, true, false, true, true};
int LeftBoolList[5] = {false, true, true, true, true};
int RightBoolList[5] = {true, true, true, true, false};
int ForwardWithLeftTurnBoolList[5] = {false, false, true, true, true};
int ForwardWithRightTurnBoolList[5] = {true, true, false, false, false};
bool WhichDirection(char *NextAction){  //See SelfDrive
  for(int i;i<5;i++){
    UltraServo.write(ServoWrites[i]);
    GetDistance(&Distances[i]);  
    if(Distances[i] < distanceThreshold){
      BelowThreshold[i] = true;
    }
    else{
      BelowThreshold[i] = false;
    }
  }//if it detects that all potential walls are closer than in this case 30cm.
  if (BelowThreshold == BackWardsBoolList){
    NextAction = "Go Backwards"; 
  }
  else if (BelowThreshold ==  ForwardsBoolList1 && BelowThreshold == ForwardsBoolList2){
    NextAction = "Go Forwards";
  }
  else if(BelowThreshold == LeftBoolList){
    NextAction = "Turn Left";
  }
  else if(BelowThreshold == RightBoolList){
    NextAction = "Turn Right";
  }
  else if(BelowThreshold == ForwardWithLeftTurnBoolList){
    NextAction = "Go forward with turn to left";
  }
  else if(BelowThreshold == ForwardWithRightTurnBoolList){
    NextAction = "Go forward with turn to right";
  }
}
void SelfDrive(){
    /*Distance meter werkt eindelijk! Nu nog de app checken of die werkt. De code was ook een beetje herhalend dus heb een for loopje erbij geplakt.
    Heb ook de void WhichDirection gemaakt. Eerst had ik moeite met het handig invoeren van of een afstand onder een threshold was, dus ik heb nog een array gemaakt.*/
  
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
    else if(inBit == Stop){
      Serial.println("stopping");
      carAccelerate(0, 0); //carSpeed is in het begin gedefined + 0 is de turnSpeed, dus niks
      inBit == Null;
    }
    else if(inBit == Forward){
      Serial.println("Going forward");
      DriveAcceleration = 100;
      carAccelerate(DriveAcceleration, 0); //carSpeed is in het begin gedefined + 0 is de turnSpeed, dus niks
      inBit == Null;
      }
    else if(inBit == Backward){
      Serial.println("Going backward");
      DriveAcceleration = 100;
      carAccelerate(-DriveAcceleration, 0);
      inBit == Null;
    }
    else if(inBit == Left){
      Serial.println("Going left");
      SteerAcceleration = 100;
      carAccelerate(0,-SteerAcceleration); 
      inBit == Null;
    }
    else if(inBit == Right){
      Serial.println("Going right");
      SteerAcceleration = 100;
      carAccelerate(0, SteerAcceleration);
      inBit == Null;
    }
    else if(inBit == Servo20deg){
      Serial.println("Servo going to 20deg");
      UltraServo.write(20);
      inBit = Null;
    }
    else if(inBit == Servo60deg){
      Serial.println("Servo going to 20deg");
      UltraServo.write(60);
      inBit = Null;
    }
    else if(inBit == Servo90deg){
      Serial.println("Servo going to 90deg");
      UltraServo.write(90);
      inBit = Null;
    }
    else if(inBit == Servo120deg){
      Serial.println("Servo going to 20deg");
      UltraServo.write(120);
      inBit = Null;
    }
    else if(inBit == Servo160deg){
      Serial.println("Servo going to 160deg");
      UltraServo.write(160);
      inBit = Null;
    }
    else if(inBit >= BeginAccelerationRange && inBit <= EndAccelerationRange){  //so the total acceleration range is 100 bits
      //Subtract inBit by begin of array, so the range is exactly an int ranging from 0 to 100. Then subtract half of the width of array to make it range from -50 to 50 for example
      DriveAcceleration = (inBit - BeginAccelerationRange)-((EndAccelerationRange-BeginAccelerationRange)/2)*2; //lastly, times two to make it exactly -100 to 100
    }
    else if(inBit == CheckDistance){
      int ScannedDistance;
      GetDistance(&ScannedDistance);
      Serial.print(ScannedDistance);
      Serial.print(" cm.");
    }
  }
  delay(20); //Insert delay so that the code won't run too fast, which isn't very useful
}

void setup(){
  Serial.begin(9600); //serial bit rate of 9600 baud
  UltraServo.attach(3);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoValue = 90; //set the servo value to straight ahead (90 degrees)
  UltraServo.write(servoValue);   
}

void loop() {
  getBTdata();
}
