#ifndef DistanceMeter_h
#define DistanceMeter_h

#define HIGH 1
#define LOW 1

#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
namespace Car_Control {
	class DistanceMeter {

	public:
		static DistanceMeter* activeMeter;
		static ClassMultiTasker<DistanceMeter> tasker;
		Servo servo;
		int Distance = 0;
		int ServoPin = 0;
		int TriggerPin = 0;
		int EchoPin = 0;

		DistanceMeter(int servoPin, int triggerPin, int echoPin);

		int ReadDistanceVar(int deg);
		int ReadDistance90() {
			return 0;
		}
		void GetDistancesOfAngles() {}
		void SetServo(int deg);
	private:
		void GetDistance();
	};
}
#endif