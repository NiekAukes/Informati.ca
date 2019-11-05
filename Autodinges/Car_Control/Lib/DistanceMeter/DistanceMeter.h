#ifndef DistanceMeter_h
#define DistanceMeter_h

#define HIGH 1
#define LOW 1

#include <Servo.h>
#include <Arduino.h>
#include <MultiTasker.h>
namespace Car_Control {
	struct MeasureResult
	{
		unsigned short Duration; //in 1/100 a second
		unsigned short Distance; //in cm
		short Angle; // degrees
	};
	class IDistanceMeter {
	public:
		short TriggerPin = 0;
		short EchoPin = 0;
	private:
		virtual void GetDistance() = 0;
		virtual void RegisterMeasurement(short angle, void (*Callback)(MeasureResult)) = 0;
	};



	class DistanceMeter : public IDistanceMeter {

	public:
		static IDistanceMeter* activeMeter;
		static ClassMultiTasker<DistanceMeter>* tasker;
		Servo* servo = new Servo();
		void (*RegisteredCallbacks[20])(MeasureResult);
		short RegisteredAngles[20];
		unsigned short Distance = 0;
		unsigned short ServoPin = 0;

		DistanceMeter(int servoPin, int triggerPin, int echoPin);

		int ReadDistanceVar(int deg);
		int ReadDistance90() {
			return 0;
		}
		void GetDistancesOfAngles() {}
		void SetServo(short deg);
		
		void RegisterMeasurement(short angle, void (*Callback)(MeasureResult));

		void CheckNextDistance();
	private:
		void GetDistance();
		void GetDistanceCallBack();
	};
}
#endif