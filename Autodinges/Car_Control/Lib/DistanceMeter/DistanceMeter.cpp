#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
#include <DistanceMeter.h>

namespace Car_Control {
	IDistanceMeter* DistanceMeter::activeMeter = NULL;
	ClassMultiTasker<DistanceMeter>* DistanceMeter::tasker = new ClassMultiTasker<DistanceMeter>();
	DistanceMeter::DistanceMeter(int servoPin, int triggerPin, int echoPin) 
	{
		for (int i = 0; i < 20; i++) {
			RegisteredAngles[i] = 127;
		}
		ServoPin = servoPin;
		TriggerPin = triggerPin;
		EchoPin = echoPin;
		DistanceMeter::activeMeter = this;
		servo->attach(servoPin);
		servo->write(90);
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

		RegisteredCallbacks[0](result);
	}

	int DistanceMeter::ReadDistanceVar(int deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo->write(deg + 90);
		tasker->RegisterTask(this, &GetDistance, 200UL);
		return 0;
	}
	void DistanceMeter::SetServo(short deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo->write(deg + 90);
	}

	void DistanceMeter::RegisterMeasurement(short angle, void (*Callback)(MeasureResult)) {
		if (angle < -90 || angle > 90) { //if deg is invalid
			return -1;
		}
		for (int i = 0; i < 20; i++) {
			if (RegisteredAngles[i] == 127) {
				RegisteredAngles[i] = angle;
				RegisteredCallbacks[i] = Callback;
			}
		}

		
	}
	void DistanceMeter::CheckNextDistance() {
		GetDistanceCallBack();

		for (short i = 1; i < 20; i++) {
			RegisteredAngles[i - 1] = RegisteredAngles[i];
			RegisteredCallbacks[i - 1] = RegisteredCallbacks[i];
		}

		tasker->RegisterTask(this, &CheckNextDistance, 200UL);
		servo->write(RegisteredAngles[0] + 90);
	}
}