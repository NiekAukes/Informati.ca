#include "SecondProfile.h"
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <AutoProfile.h>
#include <CarController.h>
#include <DistanceMeter.h>
#include <stdlib.h> // for malloc and free
void* operator new(size_t size) { return malloc(size); } 
void operator delete(void* ptr) { free(ptr); }
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Voor de afstandssensor:
#define triggerPin A5
#define echoPin A4
namespace CarControl {


  ActionData ActionData::actions[50];
   MeasureResult SecondProfile::lastResult;
  int ActionData::totalDriven = millis();
  bool SecondProfile::RetrievedMeasureResult = false;
 SecondProfile SecondProfile::newProfile;

 //SecondProfile::SecondProfile
  void SecondProfile::SelfDrive() {
    //functie
    
      Serial.println("no");
    if (meter != nullptr){
      Serial.println("yes");
      meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
    }

  }
  unsigned char SpecifiedNext = 0xFF;
  ActionData* actiontaken;
  bool newAction = true;
  unsigned char remainingresults;
  unsigned int OnLastBackTrack;
  void SecondProfile::OnUpdate() {
    if (OnBackTrack) {
      if (actiontaken->DistanceDriven + StartingBackTrack < millis() && newAction) {
        //reverse the action
        Controller::StopCar();
        newAction = false;
        if (actiontaken->optionChosen == (char)States::Left) {
          //don't go right if there is a choice
          meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
          meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
        }
        if (actiontaken->optionChosen == (char)States::Right) {
          //don't go left if there is a choice
          meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);
          meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
        }
        if (actiontaken->optionChosen == (char)States::Forward) {
          //don't go forward if there is a choice
          meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
          meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);
        }

        actiontaken = &stack->ReverseDirection();
      }
      else if ((actiontaken->DistanceDriven + StartingBackTrack) < millis() && RetrievedMeasureResult) {
        if (lastResult.Angle == -85 && lastResult.Distance > 80) {
          //go Left
          Controller::carAccelerate(0, -100);
          MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
        }
        if (lastResult.Angle == 0 && lastResult.Distance > 80) {
          //go Forward
          Controller::carAccelerate(80, 0);
        }
        if (lastResult.Angle == 85 && lastResult.Distance > 80) {
          //go Right
          Controller::carAccelerate(0, 100);
          MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
        }
        if (--remainingresults == 0) {
          //go previous way
          if (actiontaken->optionChosen == (char)States::Left) {
            //go right
            Controller::carAccelerate(0, 100);
            MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
          }
          if (actiontaken->optionChosen == (char)States::Right) {
            //go left
            Controller::carAccelerate(0, -100);
            MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
          }
          if (actiontaken->optionChosen == (char)States::Forward) {
            Controller::carAccelerate(80, 0);
          }
          newAction = true;
        }
      }
    }
    if (RetrievedMeasureResult && !OnBackTrack) {
      //Serial.println((short)lastResult.Angle);
      char spc = 0xFF;
      switch (lastResult.Angle) {
      case -85: {
        Serial.println(spc);
        //if the measurement is initiated by forward stop
        if (SpecifiedNext == 0) {
          if (lastResult.Distance < 60) {
            //no option to go left, so look right
            meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);
            spc = 2;
          }
          else {
            //left is clear so go that way
            Controller::carAccelerate(0, 100);
            MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
            meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
            meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
            stack->SetNewAction((char)States::Left, 100);
          }
          
        }
        //if the measurement is initiated by left look
        if (SpecifiedNext = 1) {
          if (lastResult.Distance > 80) {
            //could go this way, but forward is also possible
            stack->SetNewAction((char)States::Forward, Controller::DriveAcceleration);
          }
        }
        LastMeasures[0] = lastResult.Distance & 0x00FF;
        SpecifiedNext = spc;
        break;
      }
      case -45: {
        short dis = (short)lastResult.Distance;
        short lastdis = (short)LastMeasures[1];
        if (dis - lastdis > 20 && LastMeasures[1] != 0) {
          //if the distance varied by more than 20, look there
          
          Controller::StopCar();
          spc = 1;
          meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);

        }
        else {
        meter->RegisterMeasurement(45, &SecondProfile::ReceiveMeasureData);
        }
        Serial.print("deltadistance: ");
          Serial.print(dis);
          Serial.print("  //  ");
          Serial.println(lastdis);
        LastMeasures[1] = lastResult.Distance & 0x00FF;
        break;
      }
      case 0:{
        //Serial.println((short)lastResult.Distance);
        if (lastResult.Distance > 120) {
          //look elsewhere
          Controller::carAccelerate(80, 0);
          stack->SetNewAction((char)States::Forward, 80);
          meter->RegisterMeasurement(-45, &SecondProfile::ReceiveMeasureData);
        }
        else if (lastResult.Distance > 40) {
          //look elsewhere
          
          Controller::carAccelerate(40, 0);
          stack->SetNewAction((char)States::Forward, 40);
          meter->RegisterMeasurement(-45, &SecondProfile::ReceiveMeasureData);
        }
        else {
          //look elsewhere
          Controller::carAccelerate(0, 0);
          meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
          SpecifiedNext = 0;
        }
        LastMeasures[2] = lastResult.Distance & 0x00FF;
        break;
      }
      case 45: {
        if (lastResult.Distance - LastMeasures[3] > 20 && LastMeasures[3] != 0) {
          //if the distance varied by more than 20, look there
          Controller::StopCar();
          meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);

        }
        else {
          meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
        }
        LastMeasures[3] = lastResult.Distance & 0x00FF;
        break;
      }
      case 85: {
        spc = 0xFF;
        //if the measurement is initiated by forward stop
        if (SpecifiedNext == 0) {
          if (lastResult.Distance < 60) {
            //no option to go right, turn around
            Controller::carAccelerate(0, -100);
            MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 700);
            MultiTasker::Tasker->RegisterTask(&AutoProfile::EvalSelfDrive, 700);
            OnBackTrack = true;
            StartingBackTrack = millis() + 700;
            actiontaken = &stack->ReverseDirection();
          }
          else {
            //Right is clear so go that way
            Controller::carAccelerate(0, -100);
            MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
            meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
            meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
            stack->SetNewAction((char)States::Right, 100);
          }
        }
        //if the measurement is initiated by left look
        if (SpecifiedNext = 1) {
          if (lastResult.Distance > 80) {
            //could go this way, but forward is also possible
            stack->SetNewAction((char)States::Forward, Controller::DriveAcceleration);
          }
        }
        LastMeasures[4] = lastResult.Distance & 0x00FF;
        break;
      }
        default: {
          break;
        }
      }
    RetrievedMeasureResult = false;
    }
  }

  void SecondProfile::ReceiveMeasureData(MeasureResult result, IDistanceMeter* meter) {
    RetrievedMeasureResult = true;
    lastResult.Distance = result.Distance;
    lastResult.Angle = result.Angle;
    lastResult.Duration = result.Duration;
    
    /*Serial.print(RetrievedMeasureResult);
    Serial.print("  //  ");
    Serial.print(lastResult.Distance);
    Serial.print("  //  ");
    Serial.println((short)result.Angle);*/
  }
  void SecondProfile::PrintDistances() {

  }
  AutoProfile* SecondProfile::SetProfile(DistanceMeter* dismeter) {
    
    AutoProfile::currentProfile = &newProfile;
    newProfile.meter = dismeter;
    return &newProfile;
  }
}
