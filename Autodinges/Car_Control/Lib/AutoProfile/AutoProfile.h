#ifndef AutoProfile_h
#define AutoProfile_h

#include <Arduino.h>
#include <Servo.h>
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <DistanceMeter.h>
namespace Car_Control {
	class AutoProfile { // AutoProfile voor support met verschillende profielen
	protected:
		static AutoProfile* currentProfile;
	public:
		static AutoProfile* GetProfile();
		static bool SelfDriveActive;

		DistanceMeter* meter;
		ClassMultiTasker<AutoProfile> tasker;

		virtual void PrintDistances() = 0;
		virtual void SelfDrive() = 0;

		static void EvalSelfDrive();
	};

	class FirstProfile : public AutoProfile
	{
	public:
		static AutoProfile* SetProfile();
		FirstProfile();
		short AutoModeDriveAcc = 0;
		short AutoModeSteerAcc = 0;

		char WhereToGo = 0x00;
		unsigned short Distances[3] = {};
		void SelfDrive();
		char WhichDirection();
		void PrintDistances();
	};
}
#endif