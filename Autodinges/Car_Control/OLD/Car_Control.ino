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

//Hex values: https://www.ascii-code.com/ 

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

/*	Douwe Osinga 21/10/2019: Distance meter werkt eindelijk! Nu nog de app checken of die werkt. De WhichDirection code was ook een beetje herhalend dus heb een for loopje erbij geplakt.
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

/* Log van Niek: 01/11/2019
	-Ik heb een aantal veranderingen doorgevoerd die het programma geheel anders in elkaar zet. dit is de log waarin deze veranderingen worden besproken:
	  --Veel functies zijn overgezet in andere classes dan de program (.ino) class. in totaal zijn er nog 3 classes aangemaakt en de States enum apart gezet.
		deze classes gaan onder de namen: Controller (voor de controle over de auto), DistanceMeter (zorgt voor servo) en de abstracte AutoProfile (voor selfdrive).
		deze classes worden geplaatst in de library folder en zijn daarom libraries. Deze Libraries zijn NIET bedoeld om aangepast te worden. 
		Als er toch een nieuwe feature moet komen, kun je een nieuwe class maken en deze afleiden van de I- versies van de classes.
		Deze I- versies zijn interfaces waarin de gebruiker zelf de gedefineerde functies moet maken.

	  --Door de Nieuwe Constructie zijn een aantal nieuwe termen beschikbaar.
			
				 Program	--->    Program  -  IController  -  ...
					|				           |			       |
				SubClasses			Functies	     Subclasses
				Functies				               Functies
		
		Het nieuwe programma gebruikt een andere structuur dan de vorige versies. hier komt ook nieuwe syntax bij.
		2 belangrijke operators van de nieuwe structuur bespreek ik bij deze log.

		2 nieuwe operators zijn nu van toepassing op de code. dit zijn de Scope-operator (::) en de Arrow-operator (->).
		-De Scope-operator wordt gebruikt om 'in te scopen' op een klasse/enum.
		om toegang te krijgen van een lid van een klasse moet je de Scope-operator toepassen op de klasse.
		VB:
				DistanceMeter::GetDistancesOfAngles();
				^Klasse		    ^Scope		^Lid

		-De Arrow-operator wordt gebruikt om (snel) toegang te krijgen tot een object van een referentie, oftewel pointers(*). 
		pointers zijn adressen die verwijzen naar een object.
		Bijvoorbeeld: 'AutoProfile* profile;' verwijst naar een AutoProfile object.
		om bij de leden van dit object te kunnen moet je de Arrow-Operator gebruiken.
		VB:
				int pin = DistanceMeter::activeMeter->ServoPin
				///////   ^klasse		 ^pointer   ^Arrow


	-Update aan de MultiTasker lib.
	  --MultiTasker.h heeft een nieuwe klasse: ClassMultiTasker<Class> 
		Deze nieuwe klasse heeft support voor lidfuncties (T::*). 
	  
	  --Er is maar 1 actieve instantie van de klasse MultiTasker. deze klasse overziet nu alle ClassMultiTaskers.
*/
#include "SecondProfile.h"
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

MultiTasker* tasker = MultiTasker::SetMultiTasker(); //maakt instance van een class voor multitasken
DistanceMeter disMeter(3, triggerPin, echoPin);
AutoProfile* profile;

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
  //profile = SecondProfile::SetProfile();
}

void loop() {
  //Controller::CompareData();
  Serial.println("fff");
  tasker->Distribute(); //check timers if there are any pending tasks, and if so, activates those functions.
}
