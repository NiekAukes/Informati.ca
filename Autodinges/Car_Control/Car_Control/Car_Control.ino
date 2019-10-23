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

/*Distance meter werkt eindelijk! Nu nog de app checken of die werkt. De WhichDirection code was ook een beetje herhalend dus heb een for loopje erbij geplakt.
    Heb ook de void WhichDirection gemaakt. Eerst had ik moeite met het handig invoeren van of een afstand onder een threshold was, dus ik heb nog wat arrays gemaakt.
    Had een heleboel errors met chars en strings, heb uiteindelijk maar gekozen voor een fixt length char van 11, om verdere errors te voorkomen. Jammer van de spaties
    in de woorden maarja.
    Ook tyfus veel rotzooi gehad met arrays en chars, heb in de WhichDirection code maar de brute manier gekozen van elke karakter van elke lijst vergelijken met
    die ene WallList. Het zij zo.       OEH ik heb een nieuwe functie gemaakt waarmee je in één lijntje twee arrays kan vergelijken! Dit scheelt 7 lijntjes code
    ten opzichte van de brute force for(int i;etc.) manier bij elke lijst. Totale gescheelde lijntjes code: 75
    De batterijen van de arduino waren leeg dus we konden een dag bijna niks doen, behalve proberen het via de usb serial te regelen.
    We hebben nu een lader en twee extra batterijen voor de zekerheid. Ik probeer nu (23-10) ook de arduino te laten multitasken, zodat we niet de hele tijd stilstaan
    als we willen meten.*/

//voor multitasking, zie https://www.youtube.com/watch?v=zhWV_D_9OCY
#include <SoftwareSerial.h>
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

int CheckAnglesInterval = 350, CheckDistanceInterval = 350;
unsigned long previous_Time_CheckAngles,previous_Time_CheckDistance;
unsigned long currentTime;

long duration,distance;
int distance20,distance60,distance90,distance120,distance160;
int Distances[5] = {};
int BelowThreshold[5] = {false, false, false, false, false};
int WallList[5] = {};
int ServoWrites[5] = {20,60,90,120,160};
int MaxDistance = 300; //IN CENTIMETRES
char NextStep;
Servo UltraServo;
int servoValue = 90;
int DriveAcceleration = 0; //-100 t/m 100
int SteerAcceleration = 0; //-100 t/m 100

int AutoModeDriveAcc = 0;
int AutoModeSteerAcc = 0;

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
    ForwardLeft, //09hex
    ForwardRight = 11, //0B hex
    CheckDistance = 249,//for distance meter, hex F9
    Servo20deg = 250,//hex FA
    Servo60deg = 251,//hex FB
    Servo90deg = 252,//hex FC
    Servo120deg = 253,//hex FD
    Servo160deg = 254,//hex FE
    CheckAllAngles = 15, //hex 0F
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
      if(steerSpeed > 0 && carSpeed > 0){
        analogWrite(ENA, (int)((carSpeed+steerSpeed) * 2.55));
        analogWrite(ENB, (int)((0.5 * carSpeed) * 2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      }
      else if(steerSpeed < 0 && carSpeed > 0){
        analogWrite(ENA, (int)((0.5*carSpeed)));
        analogWrite(ENB, (int)((carSpeed-steerSpeed) * 2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      }
      if(steerSpeed > 0 && carSpeed < 0){
        analogWrite(ENA, (int)((-0.70*(carSpeed-steerSpeed)) * 2.55));
        analogWrite(ENB, (int)((-0.5* carSpeed) * 2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      }
      else if(steerSpeed < 0 && carSpeed < 0){
        analogWrite(ENA, (int)((0.5*carSpeed)));
        analogWrite(ENB, (int)((-(carSpeed-steerSpeed)) * 2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      }
      else if(carSpeed < 0){ //if the software wants the car to go backward:
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
      else if (steerSpeed < 0){ //if the car needs to go to left:
        analogWrite(ENA, (int)(steerSpeed*-2.55));
        analogWrite(ENB, (int)(steerSpeed*-2.55));
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);//left motors backward
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH); //right motors forward = true
      }
      else if (steerSpeed > 0){ //if the car needs to go to right:
        analogWrite(ENA, (int)(steerSpeed*2.55));
        analogWrite(ENB, (int)(steerSpeed*2.55));
        digitalWrite(IN1, HIGH);//left motors fwd = true
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);//right motors backward = true
        digitalWrite(IN4, LOW); 
      }
      
  else{
    Serial.print("CarAccelerate() function parameters not met.");
    }
  }
}

int GetDistance(){ //sensor is triggered by HIGH pulse more or equal than 10 microseconds
  digitalWrite(triggerPin, LOW); //to ensure clean high pulse
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343/*=speed of sound in cm/microsecond*/; //in centimetres
  int returnvalue = distance;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(".");
  return returnvalue;
  inBit = Null;
}

  //                Wall       //                 //                 //                     //       Wall                   ETC.
  //          Wall        Wall //    Wall Wall    //  Wall   Wall    //  Wall        Wall   //            Wall              ETC.
  //          Wall  |--|  Wall //    |--| Wall    //  Wall   |--|    //  Wall  |--|  Wall   //     |--|   Wall              ETC.
  // |--|=car    go backward   //     go left     //       go right  //     go forward      //go fwd with 30deg left turn   ETC.

void AssignCharArray(char copy[], char original[]){ //weet niet of nog gebruikt wordt. Werd gebruikt om chars over te kopiëren.
  int lengthofarray = sizeof(original);
  for(int i; i<lengthofarray;i++){
    copy[i] = original[i];
  }
}
bool arraysMatch(int array1[],int array2[]){ //handig stukje om arrays in één regel te vergelijken. Bespaart verderop ong. 75 lijntjes code :D
  if (sizeof(array1) != sizeof(array2)){    //DENK EROM: ALLEEN ARRAYS MET SIZE 5. SIZEOF() functie doet niet wat ik will
    return false;
  }
  bool doTheyMatch = true;
  for(int i;i<5;i++){
    if(array1[i] != array2[i]){
      return false;
      break;
    } 
  }
  return doTheyMatch;
}


int BackWardsThresholdList1[5] =          {1,1,1,1,1}; //Deze lijsten zijn voor de whichdirection om in te vullen. Hij neemt de lijst van Distances
int BackWardsThresholdList2[5] =          {0,1,1,1,0}; //(zie GetDistance), en voert ze in een BelowThreshold lijst in. 
int BackWardsThresholdList3[5] =          {1,1,0,1,1}; //Vervolgens matcht de rest van de WhichDirection welke kant hij op moet
int BackWardsThresholdList4[5] =          {0,0,1,0,0};
int BackWardsThresholdList5[5] =          {0,1,1,1,0};
int ForwardsThresholdList1[5] =           {1,0,0,0,1}; //op basis van deze basislijsten (links, rechts, etc.)
int ForwardsThresholdList2[5] =           {0,0,0,0,0};
int LeftThresholdList1[5] =               {0,1,1,0,0};
int LeftThresholdList2[5] =               {0,0,1,1,1}; //er staat een nul als er geen muur moet zijn, een een als een muur binnen de threshold moet zijn.
int RightThresholdList1[5] =              {1,1,1,0,0}; //zo werkt de code voor het bepalen van muren ook.
int RightThresholdList2[5] =              {1,1,1,1,0};
int RightThresholdList3[5] =              {1,1,1,1,0};
int FWD_LeftThresholdList[5] =            {0,0,1,1,1};
int FWD_RightThresholdList[5]=            {1,1,0,0,0};

//CheckAnglesInterval, CheckDistanceInterval zijn beide 350 milliseconden
//previous_Time_CheckAngles,previous_Time_CheckDistance houden beide de tijd vast wanneer de vorige keer de code was gerund (elke zoveel milliseconden, de intervals)

char WhichDirection(){  //Zie SelfDrive(). Zet de servo naar vijf vooringestelde  standen (20,60,90,120,160 graden) en meet afstand.
  previous_Time_CheckAngles = currentTime;
  previous_Time_CheckDistance = currentTime + 350; //zodat de checkdistance altijd 350 milliseconden na de servo.write komt
  int x = 0;
  for(;x < 5;){
    if(currentTime - previous_Time_CheckAngles >= CheckAnglesInterval){ //als de tijd CheckAnglesInterval milliseconden vooruit is gegaan...
      UltraServo.write(ServoWrites[x]);
      
      previous_Time_CheckAngles = currentTime;    
    }    
    if(currentTime - previous_Time_CheckDistance >= CheckDistanceInterval){
      int DistanceScanned = GetDistance();
      Distances[x] = DistanceScanned;
      Serial.println(Distances[x]);
      
      if(Distances[x] < 22){
        BelowThreshold[x] = 1; //als de afstand van een bepaald aantal graden onder de threshold zit komt er een één te staand ("Hier staat een muur!")
      }
      else{
        BelowThreshold[x] = 0; //anders komt er een nul in de array ("Geen muur dichtbij!")
      }
      
      previous_Time_CheckDistance = currentTime + 350; //zodat de checkdistance altijd 350 milliseconden na de servo.write komt
      x++;
    }
    if(x == 4){ //int WallList[5] = {}; staat bovenaan
      for(int i;i<5;i++){
        WallList[i] = BelowThreshold[i];
        Serial.print("Wall list is: ");
        Serial.println(WallList[i]);
      }
    }
  }
  UltraServo.write(90); //zet de servo weer naar 90graden om het mogelijk te maken snel weer dezelfde functie te activeren (en het ziet er stom uit als dat ding scheef staat)
  /*Dit zijn alle true/false parameters van de SelfDrive() code. Voor elk scenario wordt een boolean aangemaakt. De lijsten boven de WhichDirection() functie
  zijn hiermee geïntegreerd. Als de lijst van een scenario (bijv. BakcwardsThreshold1) NIET overeenkomt met de gemeten afstanden, wordt de boolean voor
  de lijst false. Als de lijst wél overeenkomt met het gemeten scenario, dan is de boolean true, en wordt een gegeven karakter gereturned.*/
  bool Matches_BackwardsList1 = true,Matches_BackwardsList2=true,Matches_BackwardsList3=true,Matches_BackwardsList4=true,Matches_ForwardsList1=true,
       Matches_ForwardsList2=true,Matches_LeftList1=true,Matches_LeftList2=true,Matches_RightList1=true,Matches_RightList2=true,Matches_RightList3=true,
       Matches_FWD_LeftList=true,Matches_FWD_RightList=true;
  Matches_BackwardsList1 =  arraysMatch(WallList,BackWardsThresholdList1);//checken of alle lijsten misschien de gemeten afstanden matchen...
  Matches_BackwardsList2 =  arraysMatch(WallList,BackWardsThresholdList2);
  Matches_BackwardsList3 =  arraysMatch(WallList,BackWardsThresholdList3);
  Matches_BackwardsList4 =  arraysMatch(WallList,BackWardsThresholdList4);
  Matches_ForwardsList1  =  arraysMatch(WallList,ForwardsThresholdList1);
  Matches_ForwardsList2  =  arraysMatch(WallList,ForwardsThresholdList2);
  Matches_LeftList1      =  arraysMatch(WallList,LeftThresholdList1);
  Matches_LeftList2      =  arraysMatch(WallList,LeftThresholdList2);
  Matches_RightList1     =  arraysMatch(WallList,RightThresholdList1);
  Matches_RightList2     =  arraysMatch(WallList,RightThresholdList2);
  Matches_RightList3     =  arraysMatch(WallList,RightThresholdList3);
  Matches_FWD_LeftList   =  arraysMatch(WallList,FWD_LeftThresholdList);
  Matches_FWD_RightList  =  arraysMatch(WallList,FWD_RightThresholdList);//als een scenario voorkomt, bijvoorbeeld backward, dan output de functie een 'B'.
  Serial.println(Matches_BackwardsList1);
  Serial.println(Matches_BackwardsList2);
  Serial.println(Matches_BackwardsList3);
  Serial.println(Matches_BackwardsList4);
  Serial.println(Matches_ForwardsList1);
  Serial.println(Matches_ForwardsList2);
  Serial.println(Matches_LeftList1);
  Serial.println(Matches_LeftList2);
  Serial.println(Matches_RightList1);
  Serial.println(Matches_RightList2);
  Serial.println(Matches_RightList3);
  Serial.println(Matches_FWD_LeftList);
  Serial.println(Matches_FWD_RightList);
  
  if(Matches_BackwardsList1||Matches_BackwardsList2||Matches_BackwardsList3||Matches_BackwardsList4)
                                                                                {Serial.println("Matches bwd list");return 'B';}
  if(Matches_ForwardsList1 || Matches_ForwardsList2)                            {Serial.println("Matches fwd list");return 'F';}
  if(Matches_LeftList1 || Matches_LeftList2)                                    {Serial.println("Matches left list");return 'L';}
  if(Matches_RightList1 || Matches_RightList2 || Matches_RightList3)            {Serial.println("Matches right list");return 'R';}
  if(Matches_FWD_LeftList)                                                      {Serial.println("Matches fwd left list");return 'X';}
  if(Matches_FWD_RightList)                                                     {Serial.println("Matches fwd right list");return 'Y';}
}
void SelfDrive(){
    char WhereToGo = '∅'; //zet de WhereToGo char naar het karakter-equivalent van Null ('∅'), dit is om te voorkomen dat de auto één richting 
                          //uit blijft gaan na de eerste keer te hebben gemeten.
    UltraServo.write(90);
    WhereToGo = WhichDirection(); //stored het karakter dat WhichDirection output (zie de uitleg van WhichDirection voor verdere clarificatie)
    Serial.print("Next step is to go: "); //print naar de telefoon welke kant hij op zal gaan
    Serial.println(WhereToGo);
    
    /*Een heleboel if statements, voor elk gegeven scenario (naar voren, achteren, links, rechts, en ook naar voren terwijl de auto links/rechts draait.*/
    if(WhereToGo == 'B'){
        analogWrite(ENA, (int)(100*2.55));
        analogWrite(ENB, (int)(70*2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      /*AutoModeDriveAcc = -80; 
      AutoModeSteerAcc = 60;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);*/
    }
    else if(WhereToGo == 'F'){
      AutoModeDriveAcc = 40;
      AutoModeSteerAcc = 0;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'L'){
      AutoModeDriveAcc = 0;
      AutoModeSteerAcc = -75;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'R'){
      AutoModeDriveAcc = 0;
      AutoModeSteerAcc = 75;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'X'){
      AutoModeDriveAcc = 60;
      AutoModeSteerAcc = -75;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'Y'){
      AutoModeDriveAcc = 60;
      AutoModeSteerAcc = 75;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else{
      Serial.print("Character not valid in SelfDrive function."); //als er per ongelijk een verkeerde karakter wordt ingevoerd, laat de code dat aan de telefoon weten.
      }
    //om ervoor te zorgen dat de auto ook een tijdje die richting op gaat en niet direct stopt.
    //carAccelerate(0,0); //stopt de auto, voor de zekerheid.
    char BTData = getBTdata();
    if(BTData == Manual){
      carAccelerate(0,0);
      Serial.print("inBit is not <Auto> anymore");
      inBit = Null;
      return true;
    }
    else{
      Serial.println("Car Self Drive loop succesfull! Starting again...");
      WhereToGo = '∅';
      SelfDrive();
    }
}
char getBTdata(){   //0 = Null 1 = Faulty (fault in app or bluetooth) 2/3 = Manual/auto switch
  if (Serial.available()){ // als er bits beschikbaar zijn
    inBit = (States)Serial.read();
    if (inBit == Null){
      //do nothing
    }
    else if(inBit == Faulty){
      Serial.println("Error code 1: Fault with app");
      carAccelerate(0,0);//driveAcceleration = 0 en steerAcceleration = 0 dus stop car
    }
    else if(inBit == Manual){
      Serial.println("Switching to manual...");
      carAccelerate(0,0);
      UltraServo.write(90);
    }
    else if(inBit == Auto){
      Serial.println("Going into automatic mode...");
      SelfDrive();
    }
    else if(inBit == Stop){
      Serial.println("stopping");
      carAccelerate(0, 0); //0 acceleratie + 0 is de turnSpeed, dus niks
    }
    else if(inBit == Forward){
      Serial.println("Going forward");
      DriveAcceleration = 100;
      carAccelerate(DriveAcceleration, 0); //carSpeed is in het begin gedefined + 0 is de turnSpeed, dus niks
      }
    else if(inBit == Backward){
      Serial.println("Going backward");
      DriveAcceleration = 100;
      carAccelerate(-DriveAcceleration, 0);
      inBit == Null;
    }
    else if(inBit == Left){
      Serial.println("Going left");
      carAccelerate(0,-80); 
    }
    else if(inBit == Right){
      Serial.println("Going right");
      SteerAcceleration = 100;
      carAccelerate(0, SteerAcceleration);
    }
    else if(inBit == ForwardLeft){
      Serial.println("Going forward with left turn");
      carAccelerate(75,-50);
    }
    else if(inBit == ForwardRight){
      Serial.println("Going forward with right turn");
      carAccelerate(75,50);
    }
    else if(inBit == Servo20deg){
      Serial.println("Servo going to 20deg");
      UltraServo.write(20);
      inBit = Null;
    }
    else if(inBit == Servo60deg){
      Serial.println("Servo going to 60deg");
      UltraServo.write(60);
      inBit = Null;
    }
    else if(inBit == Servo90deg){
      Serial.println("Servo going to 90deg");
      UltraServo.write(90);
      inBit = Null;
    }
    else if(inBit == Servo120deg){
      Serial.println("Servo going to 120deg");
      UltraServo.write(120);
      inBit = Null;
    }
    else if(inBit == Servo160deg){
      Serial.println("Servo going to 160deg");
      UltraServo.write(160);
      inBit = Null;
    }
    else if(inBit == CheckAllAngles){
      Serial.println("Checking All Angles");
      WhichDirection();
      inBit = Null;
    }
    
    else if(inBit >= BeginAccelerationRange && inBit <= EndAccelerationRange){  //so the total acceleration range is 100 bits
      //Subtract inBit by begin of array, so the range is exactly an int ranging from 0 to 100. Then subtract half of the width of array to make it range from -50 to 50 for example
      DriveAcceleration = (inBit - BeginAccelerationRange)-((EndAccelerationRange-BeginAccelerationRange)/2)*2; //lastly, times two to make it exactly -100 to 100
    }
    else if(inBit == CheckDistance){
      int ScannedDistance = GetDistance();
      Serial.print(ScannedDistance);
      Serial.print(" cm.");
    }
    else if(inBit == 10 || inBit == 13){
      //vreemde bits die we niet kunnen gebruiken, maar die bij elke send bit aankomen en zorgen voor rotzooi.
    }
    else{
      Serial.print("Bit not valid: ");
      Serial.println(inBit);
      inBit = Null; 
    }
  }
  delay(50); //Insert delay so that the code won't run too fast, which isn't very useful
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
  //SelfDrive();
  UltraServo.write(90);
  delay(4000);
  Serial.println("Setup is done. Ready to recieve information.");
}

void loop() {
  currentTime = millis();
  getBTdata();
}
