#ifndef AutoProfile_h
#define AutoProfile_h

#include <Arduino.h>
#include <Servo.h>
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <DistanceMeter.h>
namespace Car_Control {
	class AutoProfile { // AutoProfile voor support met verschillende profielen
	public:
		static AutoProfile *currentProfile;
		static bool SelfDriveActive;
		char WhereToGo = 0x00;
		DistanceMeter* meter;
		ClassMultiTasker<AutoProfile> tasker;
		//AutoProfile(DistanceMeter* distanceMeter);

		virtual void PrintDistances() = 0;
		virtual void SelfDrive() = 0;

		static void EvalSelfDrive();
	};

	class FirstProfile : public AutoProfile
	{
	public:
		FirstProfile(DistanceMeter* distanceMeter);
		FirstProfile();
		int AutoModeDriveAcc = 0;
		int AutoModeSteerAcc = 0;
		int Distances[3] = {};
		void SelfDrive();
		char WhichDirection();
		void PrintDistances();
		
	};
}
#endif