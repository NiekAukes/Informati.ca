#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
#include <DistanceMeter.h>

namespace CarControl {
	IDistanceMeter* DistanceMeter::activeMeter = NULL;
	DistanceMeter::DistanceMeter(int servoPin, int triggerPin, int echoPin) 
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
		servo = new Servo();
		servo->attach(servoPin);

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

		RegisteredCallbacks[0](result);
	}

	int DistanceMeter::ReadDistanceVar(int deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return -1;
		}
		servo->write(deg + 90);
		
		return 0;
	}
	void DistanceMeter::SetServo(short deg) {
		if (deg < -90 || deg > 90) { //if deg is invalid
			return;
		}
		servo->write(deg + 90);
	}

	void DistanceMeter::RegisterMeasurement(char angle, void (*Callback)(MeasureResult)) {
		if (angle < -90 || angle > 90) { //if deg is invalid
			return;
		}
		for (int i = 0; i < 10; i++) {
			if (RegisteredAngles[i] >= 127) {
				RegisteredAngles[i] = angle;
				RegisteredCallbacks[i] = Callback;
			}
		}
	}
	void DistanceMeter::CheckNextDistance() {
		if (RegisteredAngles[0] < 90 && RegisteredAngles[0] > -90) {
			GetDistanceCallBack();
			for (short i = 1; i < 10; i++) { //replace values as a queue
				if (RegisteredAngles[i] < 127)
					RegisteredAngles[i - 1] = RegisteredAngles[i];

				if (RegisteredCallbacks != nullptr)
					RegisteredCallbacks[i - 1] = RegisteredCallbacks[i];
			}
			RegisteredAngles[9] = 127;
			RegisteredCallbacks[9] = nullptr;

			servo->write(RegisteredAngles[0] + 90);
		}
	}

	void DistanceMeter::Distribute() {
		if (counter > 200) {
			CheckNextDistance();
			counter = 0;
		}
		else {
			counter += millis() - prevCount;
			prevCount = millis();
		}
		Serial.println(counter);
	}

	void DistanceMeter::GetDistancesnow(MeasureResult result) {
		Serial.println(result.Distance);
	}
}