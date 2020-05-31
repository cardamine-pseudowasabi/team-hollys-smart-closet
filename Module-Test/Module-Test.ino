#include <Servo.h>

const int doorPin = 9;
Servo doorServo;
int doorAngle = 180;

void setup(){
    doorServo.attach(doorPin);
    doorServo.write(doorAngle);
}

void loop(){
}