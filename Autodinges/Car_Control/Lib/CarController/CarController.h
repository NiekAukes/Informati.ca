#ifndef CarController_h
#define CarController_h
#include <MultiTasker.h>
#include <Arduino.h>
#include <DistanceMeter.h>
namespace CarControl {
	

	enum class States : short {
		Null,//00 hex
		Faulty,//01 hex
		Manual,//02 hex
		Auto,//03 hex
		Stop,//04 hex
		Forward,//05 hex
		Backward,//06 hex
		Left,//07 hex
		Right,//08 hex
		ForwardLeft, //09hex
		ForwardRight = 11, //0B hex
		CheckAllAngles = 15, //hex 0F
		BeginAccelerationRange = 20,
		EndAccelerationRange = 120,
		BeginSteeringRange = 130,
		EndSteeringRange = 230,
		CheckDistance = 249,//for distance meter, hex F9
		Servo20deg = 250,//hex FA
		Servo60deg = 251,//hex FB
		Servo90deg = 252,//hex FC
		Servo120deg = 253,//hex FD
		Servo160deg = 254//hex FE
	};

	class IController {
	public:
		static char DriveAcceleration; //-100 t/m 100
		static char SteerAcceleration; //-100 t/m 100
	};

	class Controller : public IController {
	private:
		Controller();
	public:
		static States inBit;
		static DistanceMeter* meter;
		static void StopCar();
		static void carAccelerate(short carSpeed, short steerSpeed);
		static char CompareData();
		static void AssignCharArray(char copy[], char original[]);
		static bool arraysMatch(int array1[], int array2[]);
		static void callbackdefault(MeasureResult result, IDistanceMeter* meter);
	};
}
#endif