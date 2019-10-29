#ifndef AutoProfileStates_h
#define AutoProfileStates_h

#include <arduino.h>
#include <Servo.h>
#include <MultiTasker.h>
class AutoProfile { // AutoProfile voor support met verschillende profiles
	public:

		void GetDistance() { //sensor is triggered by HIGH pulse more or equal than 10 microseconds
			digitalWrite(triggerPin, LOW); //to ensure clean high pulse
			delayMicroseconds(5);
			digitalWrite(triggerPin, HIGH);
			delayMicroseconds(10);
			digitalWrite(triggerPin, LOW);
			duration = pulseIn(echoPin, HIGH);
			distance = (duration / 2) * 0.0343/*=speed of sound in cm/microsecond*/; //in centimetres
			LatestDistanceMeasured = distance;
			inBit = Null;
		}

		virtual void GetDistancesOfAngles(unsigned long interval) {
			//Registratie van de automatische besturing (zie SelfDrive();)
			//in volgorde: Servo naar 60 graden, afstand meten, afstand storen, servo naar 90 graden, afstand meten, afstand storen, servo naar 120 graden, afstand meten, afstand storen, done.
			// servo naar 60deg = 0UL, servo 90deg = 250UL, servo 120deg = 500UL. Dan nog servo90deg aan het einde met 750UL.
			tasker.RegisterTask(&SaveDistanceInPlace0, interval - 1UL); //store distance int at GetDistance's 1st time + about 20ms
			tasker.RegisterTask(&SaveDistanceInPlace1, 2 * interval - 1UL);//store distance int at GetDistance's 2nd time of measuring + about 20ms (again)
			tasker.RegisterTask(&SaveDistanceInPlace2, 3 * interval - 1UL);//store distance int at GetDistance's 3rd time of measuring + about 20ms (again)

			tasker.RegisterTask(&SetServo60, 1UL);
			tasker.RegisterTask(&SetServo90, interval);
			tasker.RegisterTask(&SetServo120, interval * 2);
			tasker.RegisterTask(&SetServo90, interval * 3);
			for (int i = 1; i < 4; i++) { //dus drie keer:
				tasker.RegisterTask(&GetDistance, i * interval - 20); //moet callen net voordat de servo angles switcht, dus 230, 480, 730 UL
			}
		}

		virtual void PrintDistances() {
			for (int i; i < 3; i++) {
				Serial.println("Distance " + (String)(i + 1) + " is: " + (String)(Distances[i]));
			}
		}
		virtual void SelfDrive() = 0;
		virtual char WhichDirection() = 0;
	};

class FirstProfile : public AutoProfile
{
public:
	void SelfDrive()
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
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			tasker.RegisterTask(&StopCar, 500UL);
		}
		else if (WhereToGo == 'F') {
			AutoModeDriveAcc = 40;
			AutoModeSteerAcc = 0;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'L') {
			AutoModeDriveAcc = 0;
			AutoModeSteerAcc = -75;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			tasker.RegisterTask(&StopCar, 500UL);
		}
		else if (WhereToGo == 'R') {
			AutoModeDriveAcc = 0;
			AutoModeSteerAcc = 75;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
			tasker.RegisterTask(&StopCar, 700UL);
		}
		else if (WhereToGo == 'U') {
			AutoModeDriveAcc = 80;
			AutoModeSteerAcc = 0;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'X') {
			AutoModeDriveAcc = 100;
			AutoModeSteerAcc = -50;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else if (WhereToGo == 'Y') {
			AutoModeDriveAcc = 100;
			AutoModeSteerAcc = 50;
			carAccelerate(AutoModeDriveAcc, AutoModeSteerAcc);
		}
		else {
			Serial.print("Character not valid in SelfDrive function."); //als er per ongelijk een verkeerde karakter wordt ingevoerd, laat de code dat aan de telefoon weten.
		}
		//om ervoor te zorgen dat de auto ook een tijdje die richting op gaat en niet direct stopt.
		//carAccelerate(0,0); //stopt de auto, voor de zekerheid.
		if ((States)Serial.read() == Manual || (States)Serial.read() == Stop) {
			carAccelerate(0, 0);
			Serial.print("inBit is not <Auto> anymore");
			SelfDriveActive = false;
		}
		else if ((States)Serial.read() == Auto) {
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
	char WhichDirection()
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
		if ((States)Serial.read() == Auto) {
			tasker.RegisterTask(&GetDistancesOfAngles, 1750UL);
		}
	}
};

#endif