#include <Servo.h>

Servo servoA;
Servo servoB;
int pos = 50;

void setup() {

servoA.attach(9);
servoB.attach(10);
servoA.write(50);
servoB.write(50);

}
void loop() {

 for(int pos = 80;pos < 150; pos++)
   {
     servoA.write(pos);
     servoB.write(252 - pos);
     delay(25);
   }
   for(int pos = 150;pos > 80; pos--)
{
servoA.write(pos);
servoB.write(252 - pos);
delay(25);
}
}
