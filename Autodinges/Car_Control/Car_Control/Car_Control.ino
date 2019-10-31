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
#include <SoftwareSerial.h>
#include <AutoProfile.h>
#include <CarController.h>
#include <DistanceMeter.h>
using namespace Car_Control;
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

DistanceMeter disMeter(3, triggerPin, echoPin);
AutoProfile* profile = new FirstProfile();
Controller CarController(profile);

  


  //                Wall       //                 //                 //                     //       Wall                   ETC.
  //          Wall        Wall //    Wall Wall    //  Wall   Wall    //  Wall        Wall   //            Wall              ETC.
  //          Wall  |--|  Wall //    |--| Wall    //  Wall   |--|    //  Wall  |--|  Wall   //     |--|   Wall              ETC.
  // |--|=car    go backward   //     go left     //       go right  //     go forward      //go fwd with 30deg left turn   ETC.


//allemaal aparte functies omdat de scheduler geen argumenten aankan. fuck.



void setup(){
  Serial.begin(9600); //serial bit rate of 9600 baud
  
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Setup is done.");
  //DistanceMeter::GetDistancesOfAngles();
  
}

void loop() {
  CarController.CompareData();

  tasker.Distribute(); //check timers if there are any pending tasks, if so, activate those functions.
}
