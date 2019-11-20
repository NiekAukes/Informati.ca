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
#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
//Voor de afstandssensor:
#define triggerPin A5
#define echoPin A4
namespace Car_Control {

	class ActionData
	{
	private:
		ActionData() {

		}
	public:
		static int totalDriven;
		unsigned char ID = 0;
		unsigned char speed;
		unsigned char Size = 1;
		unsigned char DistanceDriven = 0;
		char optionChosen = 0x00;
		ActionData* nextAction;

		static ActionData* SetNewStack() {
			return new ActionData();
		}

		ActionData* SetNewAction(char Direction, unsigned char Speed) {
			Size++;
			if (nextAction == nullptr) {
				nextAction = new ActionData();
				this->DistanceDriven = (millis() - totalDriven) * speed / 100;
				nextAction->totalDriven = millis();
				nextAction->optionChosen = Direction;
				nextAction->ID = ID + 1;
				nextAction->speed = Speed;
				return nextAction;
			}
			else {
				return nextAction->SetNewAction(Direction, Speed);
			}
		}

		ActionData ReverseDirection() {
			if (!nextAction) {
				ActionData returnval = *this;
				delete this;
				return returnval;
			}
			else {
				nextAction->ReverseDirection();
			}
		}
		ActionData* FindByID(unsigned char FindID) {
			if (ID == FindID) {
				return this;
			}
			else {
				nextAction->FindByID(FindID);
			}
		}
	};
  
class SecondProfile : public AutoProfile {
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

	static void ReceiveMeasureData(MeasureResult result);
};
}
#endif
