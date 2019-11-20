#ifndef SecondProfile_h
#define SecondProfile_h

#include <Arduino.h>
#include <MultiTasker.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <AutoProfile.h>
#include <CarController.h>
#include <DistanceMeter.h>
#include <stdlib.h> // for malloc and free
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Voor de afstandssensor:
#define triggerPin A5
#define echoPin A4
namespace CarControl {

	class ActionData
	{
  private:
    ActionData() {

    }
	public:
    
		static int totalDriven;
    static ActionData actions[50];
		unsigned char ID = 0;
		unsigned char speed;
		unsigned char Size = 1;
		unsigned char DistanceDriven = 0;
		char optionChosen = 0x00;

		static ActionData* SetNewStack() {
			return &actions[0];
		}

		ActionData* SetNewAction(char Direction, unsigned char Speed) {
			Size++;
			if (actions[ID+1].optionChosen == 0x00) {
				this->DistanceDriven = (millis() - totalDriven) * speed / 100;
				actions[ID+1].totalDriven = millis();
				actions[ID+1].optionChosen = Direction;
				actions[ID+1].ID = ID + 1;
				actions[ID+1].speed = Speed;
				return &actions[ID+1];
			}
			else {
				return actions[ID+1].SetNewAction(Direction, Speed);
			}
		}

		ActionData ReverseDirection() {
			if (actions[ID+1].optionChosen == 0x00) {
				ActionData returnval = *this;
				return returnval;
			}
			else {
				actions[ID+1].ReverseDirection();
			}
		}
		ActionData* FindByID(unsigned char FindID) {
			if (ID == FindID) {
				return this;
			}
			else {
				actions[ID+1].FindByID(FindID);
			}
		}
	};
  
class SecondProfile : public AutoProfile {
private:
    SecondProfile(){}  
public:
    void PrintDistances();
    void SelfDrive();

	void OnUpdate();
	static bool RetrievedMeasureResult;
 
    static SecondProfile newProfile;
    static AutoProfile* SetProfile(DistanceMeter* dismeter);
	ActionData* stack = ActionData::SetNewStack();
	static MeasureResult lastResult;
	static void GoForward() {
		Controller::carAccelerate(80, 0);
	}
	char LastMeasures[5];
	bool OnBackTrack = false;
	int StartingBackTrack;

	static void ReceiveMeasureData(MeasureResult result, IDistanceMeter* meter);
};
}
#endif
