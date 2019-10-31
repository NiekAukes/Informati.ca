#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
#include <DistanceMeter.h>

namespace Car_Control {
	DistanceMeter* DistanceMeter::activeMeter = NULL;
	ClassMultiTasker<DistanceMeter> DistanceMeter::tasker;
	DistanceMeter::DistanceMeter(int servoPin, int triggerPin, int echoPin) : ServoPin(servoPin), TriggerPin(triggerPin), EchoPin(echoPin)
	{
		DistanceMeter::activeMeter = this;
		servo.attach(servoPin);
		servo.write(90);
	}
	void DistanceMeter::GetDistance() { //sensor is triggered by HIGH pulse more or equal than 10 microseconds
		digitalWrite(TriggerPin, LOW); //to ensure clean high pulse
		delayMicroseconds(5);
		digitalWrite(TriggerPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(TriggerPin, LOW);
		int duration = pulseIn(EchoPin, HIGH);
		Distance = (duration / 2) * 0.0343/*=speed of sound in cm/microsecond*/; //in centimetres
		int LatestDistanceMeasured = Distance;
	}

	int DistanceMeter::ReadDistanceVar(int deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo.write(deg + 90);
		tasker.RegisterTask(&GetDistance, 200UL);
		return 0;
	}
	void DistanceMeter::SetServo(int deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo.write(deg + 90);
	}
}