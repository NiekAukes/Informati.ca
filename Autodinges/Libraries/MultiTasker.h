class MultiTasker {
  int RegisteredTimers[10];
  void (*RegisteredTasks[10])();
  void (*RegisteredCallbacks[10])(int);

  public: int RegisterTask(void (*Task)(), int AfterMilliSeconds, void (*Callback)(int) = 0) {
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

  public: void Distribute() {
    for (int i = 0; i < 10; i++) {
      if (millis() > RegisteredTimers[i] && RegisteredTimers[i] != 0) {
        RegisteredTasks[i]();
        RegisteredCallbacks[i](0);
      }
    }
  }
};
