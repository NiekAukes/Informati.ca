#define vinger A0
#define vinger1 A1
#define vinger2 A2
#define led_pin 4
#define rood A3
#define groen A4
#define blauw A5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(vinger, INPUT);
  pinMode(vinger1, INPUT);
  pinMode(vinger2, INPUT);
  
}

void loop() {
    int h = analogRead(vinger);
    int h1 = analogRead(vinger1);
    int h2 = analogRead(vinger2);
    Serial.println((String)(h2) + "  " + (String)(h) + "  " + (String)(h1));

    analogWrite(rood, h2/10);
    analogWrite(groen, h/10);
    analogWrite(blauw, h1/10);
    
    delay(100);
}
