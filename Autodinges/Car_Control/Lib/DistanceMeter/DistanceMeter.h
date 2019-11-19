#ifndef DistanceMeter_h
#define DistanceMeter_h

#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
namespace CarControl {
	struct MeasureResult
	{
		unsigned char Duration; //in 1/100 a second
		unsigned char Distance; //in cm
		char Angle; // degrees
	};
	class IDistanceMeter {
	public:
		char TriggerPin = 0;
		char EchoPin = 0;
	protected:
		virtual void GetDistance() = 0;
		virtual void RegisterMeasurement(char angle, void (*Callback)(MeasureResult)) = 0;
	};



	class DistanceMeter : public IDistanceMeter/*, public MultiTasker*/ {
	private:
		int counter;
		unsigned long prevCount;
	public:
		static IDistanceMeter* activeMeter;
		Servo* servo;
		void (*RegisteredCallbacks[10])(MeasureResult);
		char RegisteredAngles[10];
		unsigned char Distance = 0;
		unsigned char ServoPin;

		DistanceMeter(int servoPin, int triggerPin, int echoPin);

		int ReadDistanceVar(int deg);
		void ReadDistance90() {
			servo->write(90);
		}
		static void GetDistancesnow(MeasureResult result);
		void GetDistancesOfAngles() {}
		void SetServo(short deg);
		
		void RegisterMeasurement(char angle, void (*Callback)(MeasureResult));
		void Distribute();

		void CheckNextDistance();
	private:
		void GetDistance();
		void GetDistanceCallBack();
	};
}
#endif