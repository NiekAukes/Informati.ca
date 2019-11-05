#include <CarController.h>
#include <Arduino.h>
#include <MultiTasker.h>
#include <AutoProfile.h>
#include <DistanceMeter.h>

#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11


namespace Car_Control {

	AutoProfile* Controller::profile = 0;
	ClassMultiTasker<Controller> Controller::tasker;
	States Controller::inBit = States::Null;
	short IController::DriveAcceleration = 100;
	short IController::SteerAcceleration = 0;

	void Controller::carAccelerate(short carSpeed, short steerSpeed) {
		Serial.print(carSpeed);
		Serial.print(" + ");
		Serial.println(steerSpeed);
		if (carSpeed == 0 && steerSpeed == 0) { //stop the car
			digitalWrite(ENA, HIGH);
			digitalWrite(ENB, HIGH);
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
			digitalWrite(IN3, LOW);
			digitalWrite(IN4, LOW);
			return;
		}//als de ingekomen bits tussen de gegeven parameters zijn:
		else if (carSpeed >= -100 && carSpeed <= 100 && steerSpeed >= -100 && steerSpeed <= 100) {  //drive the car according to the driveacceleration and steeracceleration
			if (steerSpeed > 0 && carSpeed > 0) {
				Serial.println("Going RF");
				analogWrite(ENA, (short)(carSpeed * 0.2 * 2.55));
				analogWrite(ENB, (short)(carSpeed * 2.55));
				digitalWrite(IN1, LOW); //left motors forward = true
				digitalWrite(IN2, HIGH); //left motors backward = false
				digitalWrite(IN3, LOW); //rightmotors backward = false
				digitalWrite(IN4, HIGH); //rightmotors forward = true
			}
			else if (steerSpeed < 0 && carSpeed > 0) {

				Serial.println("Going LF");
				analogWrite(ENA, (short)(carSpeed) * 2.55);
				analogWrite(ENB, (short)(0.2 * carSpeed * 2.55));
				digitalWrite(IN1, HIGH); //left motors forward = true
				digitalWrite(IN2, LOW); //left motors backward = false
				digitalWrite(IN3, HIGH); //rightmotors backward = false
				digitalWrite(IN4, LOW); //rightmotors forward = true
			}
			else if (steerSpeed > 0 && carSpeed < 0) {
				analogWrite(ENA, (short)((-0.70 * (carSpeed - steerSpeed)) * 2.55));
				analogWrite(ENB, (short)((-0.5 * carSpeed) * 2.55));
				digitalWrite(IN1, HIGH); //left motors forward = true
				digitalWrite(IN2, LOW); //left motors backward = false
				digitalWrite(IN3, LOW); //rightmotors backward = false
				digitalWrite(IN4, HIGH); //rightmotors forward = true
			}
			else if (steerSpeed < 0 && carSpeed < 0) {
				analogWrite(ENA, (short)((0.5 * carSpeed)));
				analogWrite(ENB, (short)((-(carSpeed - steerSpeed)) * 2.55));
				digitalWrite(IN1, HIGH); //left motors forward = true
				digitalWrite(IN2, LOW); //left motors backward = false
				digitalWrite(IN3, LOW); //rightmotors backward = false
				digitalWrite(IN4, HIGH); //rightmotors forward = true
			}
			else if (carSpeed < 0) { //if the software wants the car to go backward:
				analogWrite(ENA, (short)(-carSpeed * 2.55));//motoren aan op de snelheid van 0 tot 100 (wordt geconvert naar 0 tot 255 voor de motoren)
				analogWrite(ENB, (short)(-carSpeed * 2.55));//idem
				digitalWrite(IN1, LOW); //left motors forward = false
				digitalWrite(IN2, HIGH); //left motors backward = true
				digitalWrite(IN3, HIGH); //rightmotors backward = true
				digitalWrite(IN4, LOW); //rightmotors forward = false
			}
			else if (carSpeed > 0) { //if the car needs to go to forward:
				analogWrite(ENA, (short)(carSpeed * 2.55));
				analogWrite(ENB, (short)(carSpeed * 2.55));
				digitalWrite(IN1, HIGH);//left motors forward = true
				digitalWrite(IN2, LOW);
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, HIGH); //right motors forward = true
			}
			else if (steerSpeed < 0) { //if the car needs to go to left:
				analogWrite(ENA, (short)(steerSpeed * -2.55));
				analogWrite(ENB, (short)(steerSpeed * -2.55));
				digitalWrite(IN1, LOW);
				digitalWrite(IN2, HIGH);//left motors backward
				digitalWrite(IN3, LOW);
				digitalWrite(IN4, HIGH); //right motors forward = true
			}
			else if (steerSpeed > 0) { //if the car needs to go to right:
				analogWrite(ENA, (short)(steerSpeed * 2.55));
				analogWrite(ENB, (short)(steerSpeed * 2.55));
				digitalWrite(IN1, HIGH);//left motors fwd = true
				digitalWrite(IN2, LOW);
				digitalWrite(IN3, HIGH);//right motors backward = true
				digitalWrite(IN4, LOW);
			}

			else {
				Serial.print("CarAccelerate() function parameters not met.");
			}
		}
	}

	//                Wall       //                 //                 //                     //       Wall                   ETC.
	//          Wall        Wall //    Wall Wall    //  Wall   Wall    //  Wall        Wall   //            Wall              ETC.
	//          Wall  |--|  Wall //    |--| Wall    //  Wall   |--|    //  Wall  |--|  Wall   //     |--|   Wall              ETC.
	// |--|=car    go backward   //     go left     //       go right  //     go forward      //go fwd with 30deg left turn   ETC.

	void Controller::StopCar() {
		carAccelerate(0, 0);
	}

	void Controller::AssignCharArray(char copy[], char original[]) { //weet niet of nog gebruikt wordt. Werd gebruikt om chars over te kopi�ren.
		unsigned short lengthofarray = sizeof(original);
		for (int i; i < lengthofarray; i++) {
			copy[i] = original[i];
		}
	}
	bool Controller::arraysMatch(int array1[], int array2[]) { //handig stukje om arrays in ��n regel te vergelijken. Bespaart verderop ong. 75 lijntjes code :D
		if (sizeof(array1) != sizeof(array2)) {    //DENK EROM: ALLEEN ARRAYS MET SIZE 5. SIZEOF() functie doet niet wat ik will
			return false;
		}
		bool doTheyMatch = true;
		for (int i; i < 5; i++) {
			if (array1[i] != array2[i]) {
				return false;
				break;
			}
		}
		return doTheyMatch;
	}
	char Controller::CompareData() {
		if (Serial.available()) { // als er bits beschikbaar zijn
			inBit = (States)Serial.read();
			if (inBit == States::Null) {
				//do nothing
			}
			else if (inBit == States::Faulty) {
				Serial.println("Error code 1: Fault with app");
				carAccelerate(0, 0);//driveAcceleration = 0 en steerAcceleration = 0 dus stop car
			}
			else if (inBit == States::Manual) {
				Serial.println("Switching to manual...");
				carAccelerate(0, 0);
				profile->meter->ReadDistanceVar(0);
				AutoProfile::SelfDriveActive = false;
			}
			else if (inBit == States::Auto) {
				Serial.println("Going into automatic mode...");

				AutoProfile::SelfDriveActive = true;
				AutoProfile::EvalSelfDrive();
			}
			else if (inBit == States::Stop) {
				Serial.println("stopping");
				carAccelerate(0, 0); //0 acceleratie + 0 is de turnSpeed, dus niks
			}
			else if (inBit == States::Forward) {
				Serial.println("Going forward");

				carAccelerate(Controller::DriveAcceleration, 0); //carSpeed is in het begin gedefined + 0 is de turnSpeed, dus niks
			}
			else if (inBit == States::Backward) {
				Serial.println("Going backward");

				carAccelerate(-Controller::DriveAcceleration, 0);
				inBit = States::Null;
			}
			else if (inBit == States::Left) {
				Serial.println("Going left");
				carAccelerate(0, -100);
			}
			else if (inBit == States::Right) {
				Serial.println("Going right");

				carAccelerate(0, 100);
			}
			else if (inBit == States::ForwardLeft) {
				Serial.println("Going forward with left turn");
				carAccelerate(75, -50);
			}
			else if (inBit == States::ForwardRight) {
				Serial.println("Going forward with right turn");
				carAccelerate(75, 50);
			}
			else if (inBit == States::Servo20deg) {
				Serial.println("Servo going to 20deg");
				profile->meter->SetServo(-70);
				inBit = States::Null;
			}
			else if (inBit == States::Servo60deg) {
				Serial.println("Servo going to 60deg");
				profile->meter->SetServo(-30);
				inBit = States::Null;
			}
			else if (inBit == States::Servo90deg) {
				Serial.println("Servo going to 90deg");
				profile->meter->SetServo(0);
				inBit = States::Null;
			}
			else if (inBit == States::Servo120deg) {
				Serial.println("Servo going to 120deg");
				profile->meter->SetServo(30);
				inBit = States::Null;
			}
			else if (inBit == States::Servo160deg) {
				Serial.println("Servo going to 160deg");
				profile->meter->SetServo(70);
				inBit = States::Null;
			}

			else if (inBit >= States::BeginAccelerationRange && inBit <= States::EndAccelerationRange) {  //so the total acceleration range is 100 bits
			  //Subtract inBit by begin of array, so the range is exactly an int ranging from 0 to 100. Then subtract half of the width of array to make it range from -50 to 50 for example
				Controller::DriveAcceleration = (((short)inBit - (short)States::BeginAccelerationRange) - (((short)States::EndAccelerationRange - (short)States::BeginAccelerationRange) / 2) * 2); //lastly, times two to make it exactly -100 to 100
			}
			else if (inBit == (States)10 || inBit == (States)13) {
				//vreemde bits die we niet kunnen gebruiken, maar die bij elke send bit aankomen en zorgen voor rotzooi.
			}
			else {
				Serial.print("Bit not valid: ");
				Serial.println((short)inBit);
				inBit = States::Null;
			}
		}
	}
}
