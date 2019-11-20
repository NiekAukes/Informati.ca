#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
#include <DistanceMeter.h>

namespace CarControl {
	IDistanceMeter* DistanceMeter::activeMeter = NULL;
	short distanceMeter::adjustRight = 15;
	short distanceMeter::adjustCentre = 15;
	short distanceMeter::adjustLeft = 30;
	
	void DistanceMeter::InitDistanceMeter(int servoPin, int triggerPin, int echoPin) 
	{
		counter = 0;
		prevCount = millis();
		for (int i = 0; i < 10; i++) {
			RegisteredAngles[i] = (char)127;
		}
		ServoPin = servoPin;
		TriggerPin = triggerPin;
		EchoPin = echoPin;
		DistanceMeter::activeMeter = this;
		servo.attach(servoPin);

		//MultiTasker::Tasker->AddSubTasker(this);
		
		Serial.println("set servo");
	}
	void DistanceMeter::GetDistance() { //sensor is triggered by HIGH pulse more or equal than 10 microseconds
		digitalWrite(TriggerPin, LOW); //to ensure clean high pulse
		delayMicroseconds(5);
		digitalWrite(TriggerPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(TriggerPin, LOW);
		int duration = pulseIn(EchoPin, HIGH);
		Distance = (duration / 2) * 0.0343;//0.0343=speed of sound in cm/microsecond
	}

	void DistanceMeter::GetDistanceCallBack() { //sensor is triggered by HIGH pulse more or equal than 10 microseconds
		digitalWrite(TriggerPin, LOW); //to ensure clean high pulse
		delayMicroseconds(5);
		digitalWrite(TriggerPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(TriggerPin, LOW);
		int duration = pulseIn(EchoPin, HIGH);
		Distance = (duration / 2) * 0.0343;//0.0343=speed of sound in cm/microsecond

		MeasureResult result;
		result.Angle = RegisteredAngles[0];
		result.Distance = Distance;
		result.Duration = duration;

		RegisteredCallbacks[0](result, this);
	}

	int DistanceMeter::ReadDistanceVar(int deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo.write(deg + 90);
		
		return 0;
	}
	void DistanceMeter::SetServo(short deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return;
		}
		short realVal = deg + 90
		if(realVal < 0 -10){ //if right (-10 is for a threshold, so that is isn't immediately applying an adjust value) => adjust value so that the servo is set correctly
			servo.write(realVal + adjustRight);
		}
		else if(realVal == 0){ //exactly in the centre
			servo.write(realVal + adjustCentre);
		}
		else if(realVal > 0 + 10){ //if servo is to be set left, adjust left (+ 10 is for threshold) => Adjust value to set servo correctly (the servo has severe aids)
			servo.write(realVal + adjustLeft)
		}
	}

	void DistanceMeter::RegisterMeasurement(char angle, void (*Callback)(MeasureResult, IDistanceMeter*)) {
		//takes an angle and callback pointer for a function with 2 args
		if (angle < -90 || angle > 90) { //if deg is invalid
			return; //do nothing
		}
		for (int i = 0; i < 10; i++) {//checks for free spots for angles to put in
			if (RegisteredAngles[i] >= 127) { //127 = number out of range, as replacement for Null (you can read it as "if place == Null, then:")
				RegisteredAngles[i] = angle; //put angle in free spot
				RegisteredCallbacks[i] = Callback; //put callback in free spot
				return;
			}
		}
	}
	void DistanceMeter::CheckNextDistance() {
		if (RegisteredAngles[0] < 90 && RegisteredAngles[0] > -90) { //if the value is valid
			Serial.print("going to angle: ");
			Serial.println((short)RegisteredAngles[0]);
			SetServo((short)RegisteredAngles[0]); //sets servo to the registered angle
			GetDistanceCallBack(); //Measures distance and calls the callback registered
			for (short i = 1; i < 10; i++) { //replace values as a queue

				RegisteredAngles[i - 1] = RegisteredAngles[i]; //schuift alles een plekkie op
				RegisteredCallbacks[i - 1] = RegisteredCallbacks[i];
			}
			RegisteredAngles[9] = 127; //the equivalent of "Set spot in array to Null"
			RegisteredCallbacks[9] = nullptr; //make the callback point to nothing

			
		}
	}

	void DistanceMeter::Distribute() { //simply waits 500ms, checks next distance, and begins again
		if (counter > 500) {
			CheckNextDistance();
			counter = 0;
		}
		else {
			counter += millis() - prevCount;
			prevCount = millis();
		}
	}

	void DistanceMeter::GetDistancesnow(MeasureResult result, IDistanceMeter* meter) {
		Serial.println(result.Distance); //prints distance measured
	}
}