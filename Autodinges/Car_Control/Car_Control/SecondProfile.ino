#include "SecondProfile.h"
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <AutoProfile.h>
#include <CarController.h>
#include <DistanceMeter.h>
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Voor de afstandssensor:
#define triggerPin A5
#define echoPin A4
namespace Car_Control {
  void SecondProfile::SelfDrive() {
    //functie
  }
  void SecondProfile::PrintDistances() {
    
  }
  SecondProfile::AutoProfile* SetProfile() {
    return new SecondProfile();
  }
}
