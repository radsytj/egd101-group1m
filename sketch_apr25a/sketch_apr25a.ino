int led = 9;
int brightness = 0;
Serial.begin(9600);
int fadeamount = 5;


void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(led, brightness);
  brightness += fadeamount;
  if (brightness <= 0) || (brightness >= 255) {
    fadeamount = -fadeamount;
  }
  delay(30);
}
