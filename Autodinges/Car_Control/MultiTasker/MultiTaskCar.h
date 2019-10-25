#ifndef MultiTaskCar_h
#define MultiTaksCar_h

#include <inttypes.h>

class MultiTasker {
private:
	unsigned long RegisteredTimers[10];
	void (*RegisteredTasks[10])();
	void (*RegisteredCallbacks[10])();

public: 
	int RegisterTask(void (*Task)(), unsigned long AfterMilliSeconds, void (*Callback)() = nullptr) {
		if (AfterMilliSeconds < 1) {
			return -1;
		}
		for (int i = 0; i < 10; i++) {
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
		for (int i = 0; i < 10; i++) {
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

#endif