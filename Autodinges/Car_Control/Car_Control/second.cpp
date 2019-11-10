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
	int ActionData::totalDriven = millis();
	bool SecondProfile::RetrievedMeasureResult = false;

	void SecondProfile::SelfDrive() {
		//functie
		meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);

	}
	unsigned char SpecifiedNext = 0xFF;
	ActionData actiontaken;
	bool newAction = true;
	unsigned char remainingresults;
	unsigned int OnLastBackTrack;
	void SecondProfile::OnUpdate() {
		if (OnBackTrack) {
			if (actiontaken.DistanceDriven + StartingBackTrack < millis() && newAction) {
				//reverse the action
				Controller::StopCar();
				newAction = false;
				if (actiontaken.optionChosen == (char)States::Left) {
					//don't go right if there is a choice
					meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
					meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
				}
				if (actiontaken.optionChosen == (char)States::Right) {
					//don't go left if there is a choice
					meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);
					meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
				}
				if (actiontaken.optionChosen == (char)States::Forward) {
					//don't go forward if there is a choice
					meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
					meter->RegisterMeasurement(85, &SecondProfile::ReceiveMeasureData);
				}

				actiontaken = stack->ReverseDirection();
			}
			else if ((actiontaken.DistanceDriven + StartingBackTrack) < millis() && RetrievedMeasureResult) {
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
					if (actiontaken.optionChosen == (char)States::Left) {
						//go right
						Controller::carAccelerate(0, 100);
						MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
					}
					if (actiontaken.optionChosen == (char)States::Right) {
						//go left
						Controller::carAccelerate(0, -100);
						MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
					}
					if (actiontaken.optionChosen == (char)States::Forward) {
						Controller::carAccelerate(80, 0);
					}
					newAction = true;
				}
			}
		}
		if (RetrievedMeasureResult && !OnBackTrack) {


			switch (lastResult.Angle) {
			case 5:
				char spc = 0xFF;
				//if the measurement is initiated by forward stop
				if (SpecifiedNext == 0) {
					if (lastResult.Distance < 80) {
						//no option to go left, so look right
						meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
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
			case 45:
				if (lastResult.Distance - LastMeasures[1] > 20) {
					//if the distance varied by more than 20, look there
					Controller::StopCar();
					meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);

				}
				meter->RegisterMeasurement(45, &SecondProfile::ReceiveMeasureData);
				LastMeasures[1] = lastResult.Distance & 0x00FF;
				break;
			case 90:
				if (lastResult.Distance > 120) {
					//look elsewhere
					Controller::carAccelerate(80, 0);
					stack->SetNewAction((char)States::Forward, 80);
					meter->RegisterMeasurement(-45, &SecondProfile::ReceiveMeasureData);
				}
				else if (lastResult.Distance > 60) {
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
			case 135:
				LastMeasures[3] = lastResult.Distance & 0x00FF;
				break;
			case 175:
				char spc = 0xFF;
				//if the measurement is initiated by forward stop
				if (SpecifiedNext == 0) {
					if (lastResult.Distance < 80) {
						//no option to go right, turn around
						Controller::carAccelerate(0, -100);
						MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 700);
						MultiTasker::Tasker->RegisterTask(&AutoProfile::EvalSelfDrive, 700);
						OnBackTrack = true;
						StartingBackTrack = millis() + 700;
						actiontaken = stack->ReverseDirection();
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
		}
		RetrievedMeasureResult = false;
	}

	void SecondProfile::ReceiveMeasureData(MeasureResult result) {
		RetrievedMeasureResult = true;
		lastResult = result;
	}
	void SecondProfile::PrintDistances() {

	}
	AutoProfile* SecondProfile::SetProfile() {
		SecondProfile* newProfile = new SecondProfile();
		currentProfile = newProfile;
		return newProfile;
	}
}
