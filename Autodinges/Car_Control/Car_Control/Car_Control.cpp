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
    als we willen meten.
    
    Met pijn in mijn hart heb ik 50 regels checkArrays weg moeten halen. We gebruiken nu een timer library van Niek. Dit maakt de code veel compacter en we kunnen hierdoor gewoon
    door blijven rijden terwijl we de servo meten. We hebben bijna een werkende selfDrive functie! 
    (P.S. Niek en ik hebben in dit autootje en de app al ongeveer beide 30 uur in gestoken)*/

//voor multitasking, zie https://www.youtube.com/watch?v=zhWV_D_9OCY

/*MultiTasker is een door Niek gecreëerde lokale library die zorgt voor het timen van acties, zoals het setten van een servo en het meten van een afstand.
  Commands:
    Multitasker [tasker];
      Initialiseer een instance van class MultiTasker (een klein plannertje). Hiermee kun je de functies activeren die hieronder beschreven zijn.
      DENK EROM! De tasker kan maximaal 10 tasks aan. Je kunt de limiet handmatig veranderen in de library. (bij for loop aan het begin)
      
    [tasker].RegisterTask(&function, interval);
      Registreerd een getimede functie bij de bepaalde planner.
        function = de functie die moet worden geactiveerd. 
        DENK EROM! Voor de functie moet een '&' (die wijst naar de functie zodat de planner een pointer terugkrijgt.)

        interval = interval in UL (unsigned long). Dit is het interval waarmee de actie moet gebeuren.
        DENK EROM! Na de interval (in milliseconden) moet *altijd* 'UL'. Dit staat namelijk voor unsigned long, een extra grote integer
        Anders kom je in de problemen met de returns, errors, en al die rotzooi.
        
    [tasker].Distribute();
      Geeft een opdracht aan de planner om de tijd te checken.
      Als er de hoeveelheid tijd is verstreken die ingesteld is in de RegisterTask functie, wordt de aangewezen functie geactiveerd.
*/

#include <MultiTasker.h>

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


MultiTasker tasker; //maakt instance van een class voor multitasken

long duration,distance;
int distance20,distance60,distance90,distance120,distance160;
int Distances[3] = {};
int BelowThreshold[5] = {false, false, false, false, false};
int ServoWrites[5] = {20,60,90,120,160};
int MaxDistance = 300; //IN CENTIMETRES
char NextStep;
char WhereToGo = 0x00;
Servo UltraServo;
int servoValue = 90;
int DriveAcceleration = 0; //-100 t/m 100
int SteerAcceleration = 0; //-100 t/m 100

int LatestDistanceMeasured;

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
bool UsingAutoProfiles = true;
AutoProfile profile = 0;

void carAccelerate(int carSpeed, int steerSpeed){ 
  Serial.print(carSpeed);
  Serial.print(" + ");
  Serial.println(steerSpeed);
  if(carSpeed == 0 && steerSpeed == 0){ //stop the car
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      return;
    }//als de ingekomen bits tussen de gegeven parameters zijn:
  else if(carSpeed >= -100 && carSpeed <= 100 && steerSpeed >= -100 && steerSpeed <= 100){  //drive the car according to the driveacceleration and steeracceleration
      if(steerSpeed > 0 && carSpeed > 0){
        Serial.println("Going RF");
        analogWrite(ENA, (int)(carSpeed * 0.2 * 2.55));
        analogWrite(ENB, (int)(carSpeed * 2.55));
        digitalWrite(IN1, LOW); //left motors forward = true
        digitalWrite(IN2, HIGH); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true
      }
      else if(steerSpeed < 0 && carSpeed > 0){
        
        Serial.println("Going LF");
        analogWrite(ENA, (int)(carSpeed)* 2.55);
        analogWrite(ENB, (int)(0.2*carSpeed*2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, HIGH); //rightmotors backward = false
        digitalWrite(IN4, LOW); //rightmotors forward = true
      }
      else if(steerSpeed > 0 && carSpeed < 0){
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
void GetDistance(){ //sensor is triggered by HIGH pulse more or equal than 10 microseconds
  digitalWrite(triggerPin, LOW); //to ensure clean high pulse
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343/*=speed of sound in cm/microsecond*/; //in centimetres
  LatestDistanceMeasured = distance;
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


//allemaal aparte functies omdat de scheduler geen argumenten aankan. fuck.
void SetServo60() {UltraServo.write(45); }
void SetServo90() {UltraServo.write(90); }
void SetServo120(){UltraServo.write(135);}

void SaveDistanceInPlace0(){Distances[0] = LatestDistanceMeasured;}
void SaveDistanceInPlace1(){Distances[1] = LatestDistanceMeasured;}
void SaveDistanceInPlace2(){Distances[2] = LatestDistanceMeasured;}

void PrintDistances(){
  for(int i;i<3;i++){
    Serial.println("Distance " + (String)(i+1) + " is: " + (String)(Distances[i]));
  }
}

void GetDistancesOfAngles(){
  //Registratie van de automatische besturing (zie SelfDrive();)
  //in volgorde: Servo naar 60 graden, afstand meten, afstand storen, servo naar 90 graden, afstand meten, afstand storen, servo naar 120 graden, afstand meten, afstand storen, done.
  // servo naar 60deg = 0UL, servo 90deg = 250UL, servo 120deg = 500UL. Dan nog servo90deg aan het einde met 750UL.
  tasker.RegisterTask(&SaveDistanceInPlace0, 249UL); //store distance int at GetDistance's 1st time + about 20ms
  tasker.RegisterTask(&SaveDistanceInPlace1, 499UL);//store distance int at GetDistance's 2nd time of measuring + about 20ms (again)
  tasker.RegisterTask(&SaveDistanceInPlace2, 749UL);//store distance int at GetDistance's 3rd time of measuring + about 20ms (again)
  
  tasker.RegisterTask(&SetServo60, 1UL);
  tasker.RegisterTask(&SetServo90, 250UL);
  tasker.RegisterTask(&SetServo120,500UL);
  tasker.RegisterTask(&SetServo90, 750UL);
  for (int i = 1;i<4;i++){ //dus drie keer:
    tasker.RegisterTask(&GetDistance, i * 250UL - 20); //moet callen net voordat de servo angles switcht, dus 230, 480, 730 UL
  }


  //tasker.RegisterTask(&PrintDistances,1100UL);

  if((States)Serial.read() == Auto){
    GetDistancesOfAngles(); 
  }
}

char WhichDirection(){  //Zie SelfDrive(). Zet de servo naar vijf vooringestelde  standen (20,60,90,120,160 graden) en meet afstand.
  PrintDistances();
  bool CanRight = true;
  bool CanForward = true;
  bool CanForwardFast = true;
  bool CanLeft = true;
  if(Distances[0] < 40){
    CanRight = false;
  }
  if(Distances[1] < 40){
    CanForward = false;
  }
  if(Distances[2] < 40){
    CanLeft = false;
  }
  if (Distances[1] < 120) {
    CanForwardFast = false;
  }
  
  if (!CanRight && !CanForward && !CanLeft) {
    return 'B';
  }
  if (CanForwardFast) {
    return 'U';
  }
  if (!CanForward && !CanRight) {
    return 'L';
  }
  if (!CanForward && !CanLeft) {
    return 'R';
  } 
  if (CanForward && CanRight) {
    return 'X';
  }
  if (CanForward && CanLeft) {
    return 'Y';
  }
  if (CanForward) {
    return 'F';
  }
  if (!CanForward) {
    return 'L';
  }
  //Zegmaar recursion maar dan getimed
  if ((States)Serial.read() == Auto){
    tasker.RegisterTask(&GetDistancesOfAngles, 1750UL);
  }
}
void StopCar() {
  carAccelerate(0,0);
}
bool SelfDriveActive = true;
void SelfDrive(){
  
    WhereToGo = WhichDirection();
    Serial.print("Next step is to go: "); //print naar de telefoon welke kant hij op zal gaan
    Serial.println(WhereToGo);
    
    /*Een heleboel if statements, voor elk gegeven scenario (naar voren, achteren, links, rechts, en ook naar voren terwijl de auto links/rechts draait.*/
    if(WhereToGo == 'B'){
      /*  analogWrite(ENA, (int)(100*2.55));
        analogWrite(ENB, (int)(70*2.55));
        digitalWrite(IN1, HIGH); //left motors forward = true
        digitalWrite(IN2, LOW); //left motors backward = false
        digitalWrite(IN3, LOW); //rightmotors backward = false
        digitalWrite(IN4, HIGH); //rightmotors forward = true*/
      AutoModeDriveAcc = 0; 
      AutoModeSteerAcc = -80;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
      tasker.RegisterTask(&StopCar, 500UL);
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
      tasker.RegisterTask(&StopCar, 500UL);
    }
    else if(WhereToGo == 'R'){
      AutoModeDriveAcc = 0;
      AutoModeSteerAcc = 75;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
      tasker.RegisterTask(&StopCar, 700UL);
    }
    else if(WhereToGo == 'U'){
      AutoModeDriveAcc = 80;
      AutoModeSteerAcc = 0;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'X'){
      AutoModeDriveAcc = 100;
      AutoModeSteerAcc = -50;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else if(WhereToGo == 'Y'){
      AutoModeDriveAcc = 100;
      AutoModeSteerAcc = 50;
      carAccelerate(AutoModeDriveAcc,AutoModeSteerAcc);
    }
    else{
      Serial.print("Character not valid in SelfDrive function."); //als er per ongelijk een verkeerde karakter wordt ingevoerd, laat de code dat aan de telefoon weten.
      }
    //om ervoor te zorgen dat de auto ook een tijdje die richting op gaat en niet direct stopt.
    //carAccelerate(0,0); //stopt de auto, voor de zekerheid.
    if((States)Serial.read() == Manual || (States)Serial.read() == Stop){
      carAccelerate(0,0);
      Serial.print("inBit is not <Auto> anymore");
      SelfDriveActive = false;
    }
    else if((States)Serial.read() == Auto){
      Serial.println("Self drive succes!");
      WhereToGo = '∅';
      SelfDriveActive = true;
    }
    if (SelfDriveActive) {
      Serial.println(tasker.RegisterTask(&SelfDrive, (WhereToGo == 'R' || WhereToGo == 'L') ? 1500UL : 1000UL));
      if (!(WhereToGo == 'R' || WhereToGo == 'L')) {
        GetDistancesOfAngles();
      }
      else {
      tasker.RegisterTask(&GetDistancesOfAngles, 500UL, &WhichDirection);
      }
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
      SelfDriveActive = false;
    }
    else if(inBit == Auto){
      Serial.println("Going into automatic mode...");
	  
      SelfDriveActive = true;
	  if (UsingAutoProfiles) {
		  profile.SelfDrive();
	  }
	  else {
		  SelfDrive();
	  }
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
      DriveAcceleration = ((inBit - BeginAccelerationRange)-((EndAccelerationRange-BeginAccelerationRange)/2)*2); //lastly, times two to make it exactly -100 to 100
    }
    else if(inBit == CheckDistance){
      GetDistance();
      Serial.print(LatestDistanceMeasured);
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
  UltraServo.write(90);
  Serial.println("Setup is done.");
  GetDistancesOfAngles();
  
}

void loop() {
  getBTdata();
  tasker.Distribute(); //check timers if there are any pending tasks, if so, activate those functions.
}
