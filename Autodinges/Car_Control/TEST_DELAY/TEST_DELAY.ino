#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11


#include <MultiTaskCar.h>
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long interval = 1000;           // interval at which to blink (milliseconds)
MultiTasker tasker;

bool goingBW = false;
void bw(){
  analogWrite (ENA, 200);
  analogWrite (ENB, 200);
  digitalWrite(IN1, HIGH); //left motors forward = true
  digitalWrite(IN2, LOW); //left motors backward = false
  digitalWrite(IN3, LOW); //rightmotors backward = false
  digitalWrite(IN4, HIGH); //rightmotors forward = true
}
void stopcar(){
analogWrite (ENA, 0);
  analogWrite (ENB, 0);
  digitalWrite(IN1, LOW); //left motors forward
  digitalWrite(IN2, LOW); //left motors backward
  digitalWrite(IN3, LOW); //rightmotors backward
  digitalWrite(IN4, LOW); //rightmotors forward
}
void setup() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  delay(1000);
  bw();
  delay(1000);
  stopcar();

  tasker.RegisterTask(&bw, 500UL);
  tasker.RegisterTask(&stopcar, 1000UL);
}

void loop() {
  tasker.Distribute();
}
