#include <AutoProfile.h>
#include <Arduino.h>
#include <Servo.h>
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <DistanceMeter.h>
#include <CarController.h>
namespace Car_Control {
	AutoProfile* AutoProfile::currentProfile = NULL;
	bool AutoProfile::SelfDriveActive = false;

	void AutoProfile::EvalSelfDrive() {
		AutoProfile::currentProfile->SelfDrive();
	}

	FirstProfile::FirstProfile(DistanceMeter* distanceMeter) {
		meter = distanceMeter;
		currentProfile = this;
	}
	FirstProfile::FirstProfile() {
		currentProfile = this;
	}

	void FirstProfile::PrintDistances() {
		for (int i; i < 3; i++) {
			Serial.println("Distance " + (String)(i + 1) + " is: " + (String)(Distances[i]));
		}
	}

	void FirstProfile::SelfDrive()
	{
		WhereToGo = WhichDirection();
		Serial.print("Next step is to go: "); //print naar de telefoon welke kant hij op zal gaan
		Serial.println(WhereToGo);

		/*Een heleboel if statements, voor elk gegeven scenario (naar voren, achteren, links, rechts, en ook naar voren terwijl de auto links/rechts draait.*/
		if (WhereToGo == 'B') {
			/*  analogWrite(ENA, (int)(100*2.55));
			  analogWrite(ENB, (int)(70*2.55));
			  digitalWrite(IN1, HIGH); //left motors forward = true
			  digitalWrite(IN2, LOW); //left motors backward = false
			  digitalWrite(IN3, LOW); //rightmotors backward = false
			  digitalWrite(IN4, HIGH); //rightmotors forward = true*/
			AutoModeDriveAcc = 0;
			AutoModeSteerAcc = -80;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			Controller::tasker.RegisterTask(&Controller::StopCar, 500UL);
		}
		else if (WhereToGo == 'F') {
			AutoModeDriveAcc = 40;
			AutoModeSteerAcc = 0;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'L') {
			AutoModeDriveAcc = 0;
			AutoModeSteerAcc = -75;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			Controller::tasker.RegisterTask(&Controller::StopCar, 500UL);
		}
		else if (WhereToGo == 'R') {
			AutoModeDriveAcc = 0;
			AutoModeSteerAcc = 75;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			Controller::tasker.RegisterTask(&Controller::StopCar, 700UL);
		}
		else if (WhereToGo == 'U') {
			AutoModeDriveAcc = 80;
			AutoModeSteerAcc = 0;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'X') {
			AutoModeDriveAcc = 100;
			AutoModeSteerAcc = -50;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'Y') {
			AutoModeDriveAcc = 100;
			AutoModeSteerAcc = 50;
			Controller::carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else {
			Serial.print("Character not valid in SelfDrive function."); //als er per ongelijk een verkeerde karakter wordt ingevoerd, laat de code dat aan de telefoon weten.
		}
		//om ervoor te zorgen dat de auto ook een tijdje die richting op gaat en niet direct stopt.
		//carAccelerate(0,0); //stopt de auto, voor de zekerheid.
		if ((States)Serial.read() == States::Manual|| (States)Serial.read() == States::Stop) {
			Controller::carAccelerate(0, 0);
			Serial.print("inBit is not <Auto> anymore");
			SelfDriveActive = false;
		}
		else if ((States)Serial.read() == States::Auto) {
			Serial.println("Self drive succes!");
			WhereToGo = '∅';
			SelfDriveActive = true;
		}
		if (SelfDriveActive) {
			
			Serial.println(MultiTasker::Tasker->RegisterTask(&EvalSelfDrive, (WhereToGo == 'R' || WhereToGo == 'L') ? 1500UL : 1000UL));
			if (!(WhereToGo == 'R' || WhereToGo == 'L')) {
				meter->GetDistancesOfAngles();
			}
			else {
				DistanceMeter::tasker.RegisterTask(&meter->GetDistancesOfAngles, 500UL);
			}
		}
		
	}

	char FirstProfile::WhichDirection()
	{
		PrintDistances();
		bool CanRight = true;
		bool CanForward = true;
		bool CanForwardFast = true;
		bool CanLeft = true;
		if (Distances[0] < 40) {
			CanRight = false;
		}
		if (Distances[1] < 40) {
			CanForward = false;
		}
		if (Distances[2] < 40) {
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
		if ((States)((short)Serial.read()) == States::Auto) {
			DistanceMeter::tasker.RegisterTask(&meter->GetDistancesOfAngles, 1750UL);
		}
	}
}