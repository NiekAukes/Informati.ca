#include <SoftwareSerial.h>
#define duim D1
#define wijsvinger D2
#define middelvinger D3
#define ringvinger D4
#define pink D5

SoftwareSerial s(30,31);
void setup() {
  Serial.begin(9600);
  s.begin(9600);
}

void loop() {
  Serial.println("blyat");
  s.println("f");
  delay(100);
}
