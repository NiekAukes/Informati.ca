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
	char SpecifiedNext = 0xFF;
	void SecondProfile::OnUpdate() {
		if (RetrievedMeasureResult) {


			switch (lastResult.Angle) {
			case 5:
				if (SpecifiedNext == 1) {
					if (lastResult.Distance < 80) {
						meter->RegisterMeasurement(-85, &SecondProfile::ReceiveMeasureData);
					}
					else {
						Controller::carAccelerate(0, 100);
						MultiTasker::Tasker->RegisterTask(&Controller::StopCar, 400);
						meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
						meter->RegisterMeasurement(0, &SecondProfile::ReceiveMeasureData);
					}
					LastMeasures[0] = lastResult.Distance & 0x00FF;
				}
				if (SpecifiedNext = 1) {

				}
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
					meter->RegisterMeasurement(-45, &SecondProfile::ReceiveMeasureData);
				}
				else if (lastResult.Distance > 60) {
					//look elsewhere
					Controller::carAccelerate(40, 0);
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
				LastMeasures[4] = lastResult.Distance & 0x00FF;
				break;
			}
		}
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
