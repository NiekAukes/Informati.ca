#define ENA 6
#define ENB 5
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long interval = 1000;           // interval at which to blink (milliseconds)

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
  bw();
  delay(1000);
  stopcar();
}

void loop() {
  if (millis() - previousMillis > interval){
    // save the last time you blinked the LED
    previousMillis += interval; 

    // if the LED is off turn it on and vice-versa:
    goingBW != goingBW;
    if(goingBW){
        bw();
      }
    else{
       stopcar();
       }
  }
}
