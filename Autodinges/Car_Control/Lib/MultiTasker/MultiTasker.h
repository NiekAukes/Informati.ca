#ifndef MultiTasker_h
#define MultiTasker_h

#include <inttypes.h>
#include <Arduino.h>


class IMultiTasker {
public:
	virtual void Distribute() = 0;
};


class MultiTasker : public IMultiTasker {
private:
	unsigned int RegisteredTimers[20];
	void (*RegisteredTasks[20])();
public: 
	static MultiTasker* Tasker;
	IMultiTasker* ClassMultiTaskers[5];

	static MultiTasker* SetMultiTasker();

	int RegisterTask(void (*Task)(), unsigned int AfterMilliSeconds) {
		if (AfterMilliSeconds < 1) {
			return -1;
		}
		for (int i = 0; i < 20; i++) {
			if (RegisteredTimers[i] == 0) { //Found a null Place
				RegisteredTimers[i] = millis() + AfterMilliSeconds;
				RegisteredTasks[i] = Task;
				return i;
			}
		}
		return -1;
	}


	void Distribute() {
		for (int i = 0; i < 20; i++) {
			if (millis() > RegisteredTimers[i] && RegisteredTimers[i] != 0) {
				RegisteredTasks[i]();
				RegisteredTimers[i] = 0;
				RegisteredTasks[i] = nullptr;
			}
		}

		for (int i = 0; i < 5; i++) {
			if (ClassMultiTaskers[i] != nullptr) {
				(ClassMultiTaskers[i])->Distribute();
			}
		}
	}
	void AddSubTasker(IMultiTasker* newTasker) {
		Serial.println("Creating new Classmultitasker");
		for (int i = 0; i < 5; i++) {
			if (this->ClassMultiTaskers[i] == nullptr) {

				this->ClassMultiTaskers[i] = newTasker;
				break;
			}
		}
	}
};

template <class T>
class ClassMultiTasker : public IMultiTasker {
private:
	unsigned int RegisteredTimers[10];
	void (T::*RegisteredTasks[10])();
	T* Callers[10];

public:
	ClassMultiTasker(bool Create) {
		if (Create) {
			Serial.println("Creating new Classmultitasker");
			for (int i = 0; i < 5; i++) {
				if (MultiTasker::Tasker->ClassMultiTaskers[i] == nullptr) {
					
					MultiTasker::Tasker->ClassMultiTaskers[i] = this;
					break;
				}
			}
			for (int i = 0; i < 10; i++) {
				RegisteredTimers[i] = 0;
			}
		}
		
	}
	int RegisterTask(T* CallOn, void (T::*Task)(), unsigned long AfterMilliSeconds) {
		if (AfterMilliSeconds < 1) {
			return -1;
		}
		for (int i = 0; i < 10; i++) {
			if (RegisteredTimers[i] == 0) { //Found a null Place
				RegisteredTimers[i] = millis() + AfterMilliSeconds;
				RegisteredTasks[i] = Task;
				Callers[i] = CallOn;
				return i;
			}
		}
		return -1;
	}

	void Distribute() {
		for (int i = 0; i < 10; i++) {
			if (millis() > RegisteredTimers[i] && RegisteredTimers[i] != 0) {
				Serial.println("the Timers stops...");
				Serial.print("f");
				Serial.println(RegisteredTimers[i]);
				if (Callers != nullptr && RegisteredTasks != nullptr)
					(Callers[i]->*RegisteredTasks[i])();
				RegisteredTimers[i] = 0;
				RegisteredTasks[i] = nullptr;
				Callers[i] = nullptr;
			}
		}
	}
	
};


//MultiTasker* MultiTasker::multiTasker = new MultiTasker();

#endif