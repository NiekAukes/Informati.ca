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
	unsigned long RegisteredTimers[20];
	void (*RegisteredTasks[20])();
	void (*RegisteredCallbacks[20])(); 
public: 
	static MultiTasker* Tasker;
	IMultiTasker* ClassMultiTaskers[10];

	MultiTasker() {
		if (Tasker != NULL && Tasker != nullptr && !Tasker) {
			//there is no instance of MultiTasker, so create one
			Tasker = this;
			return;
		}
		else {
			delete this;
		}
	}

	int RegisterTask(void (*Task)(), unsigned long AfterMilliSeconds, void (*Callback)() = nullptr) {
		if (AfterMilliSeconds < 1) {
			return -1;
		}
		for (int i = 0; i < 20; i++) {
			if (RegisteredTimers[i] == 0) { //Found a null Place
				RegisteredTimers[i] = millis() + AfterMilliSeconds;
				RegisteredTasks[i] = Task;
				RegisteredCallbacks[i] = Callback;
				return i;
			}
		}
		return -1;
	}


	void Distribute() {
		for (int i = 0; i < 20; i++) {
			if (millis() > RegisteredTimers[i] && RegisteredTimers[i] != 0) {
				RegisteredTasks[i]();
				if (RegisteredCallbacks[i] != nullptr) 
				{
					RegisteredCallbacks[i]();
				}
				RegisteredTimers[i] = 0;
				RegisteredTasks[i] = nullptr;
				RegisteredCallbacks[i] = nullptr;
			}
		}

		for (int i = 0; i < 10; i++) {
			if (!ClassMultiTaskers[i]) {
				(ClassMultiTaskers[i])->Distribute();
			}
		}
	}
};

template <class T>
class ClassMultiTasker : public IMultiTasker {
private:
	unsigned long RegisteredTimers[10];
	void (T::*RegisteredTasks[10])();
	void (T::*RegisteredCallbacks[10])();
	T* Callers[10];

public:
	ClassMultiTasker() {
		for (int i = 0; i < 10; i++) {
			if (!MultiTasker::Tasker->ClassMultiTaskers[i] || MultiTasker::Tasker->ClassMultiTaskers[i] == nullptr) {
				MultiTasker::Tasker->ClassMultiTaskers[i] = this;
			}
		}
		
	}
	int RegisterTask(T* CallOn, void (T::*Task)(), unsigned long AfterMilliSeconds, void (T::*Callback)() = nullptr) {
		if (AfterMilliSeconds < 1) {
			return -1;
		}
		for (int i = 0; i < 10; i++) {
			if (RegisteredTimers[i] == 0) { //Found a null Place
				RegisteredTimers[i] = millis() + AfterMilliSeconds;
				RegisteredTasks[i] = Task;
				RegisteredCallbacks[i] = Callback;
				Callers[i] = CallOn;
				return i;
			}
		}
		return -1;
	}

	void Distribute() {
		for (int i = 0; i < 10; i++) {
			if (millis() > RegisteredTimers[i] && RegisteredTimers[i] != 0) {
				(Callers[i]->*RegisteredTasks[i])();
				if (RegisteredCallbacks[i] != nullptr)
				{
					(Callers[i]->*RegisteredCallbacks[i])();
				}
				RegisteredTimers[i] = 0;
				RegisteredTasks[i] = nullptr;
				RegisteredCallbacks[i] = nullptr;
				Callers[i] = nullptr;
			}
		}
	}
	
};


//MultiTasker* MultiTasker::multiTasker = new MultiTasker();

#endif