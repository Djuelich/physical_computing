#include <Servo.h>
#include <IRremote.h>

const int buttonPin = 2;
int buttonState = 0;
Servo servoA;
int buttonMode = 0;

void setup() {

servoA.attach(9);
servoA.write(0);

pinMode(buttonPin, INPUT);
}

void loop() {

buttonState = digitalRead(buttonPin);


if(buttonState == HIGH){
  servoA.write(180);
  delay(1000);
  servoA.write(0);
  delay(1000);

delay(15);
}

else{
  servoA.write(0);
  delay(1000);
}
}