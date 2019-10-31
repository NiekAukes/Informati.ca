#ifndef MultiTasker_h
#define MultiTasker_h

#include <inttypes.h>
#include <Arduino.h>

class MultiTasker {
private:
	unsigned long RegisteredTimers[20];
	void (*RegisteredTasks[20])();
	void (*RegisteredCallbacks[20])();

public: 
	static MultiTasker* Tasker;

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
	}
};

template <class T>
class ClassMultiTasker {
private:
	unsigned long RegisteredTimers[20];
	void (T::*RegisteredTasks[20])();
	void (T::*RegisteredCallbacks[20])();

public:
	int RegisterTask(void (T::*Task)(), unsigned long AfterMilliSeconds, void (T::*Callback)() = nullptr) {
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
	}
};


//MultiTasker* MultiTasker::multiTasker = new MultiTasker();

#endif