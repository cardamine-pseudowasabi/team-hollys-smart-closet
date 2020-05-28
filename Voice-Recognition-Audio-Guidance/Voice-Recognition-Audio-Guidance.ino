#include <Servo.h>

const int audioGuidancePin = 50;
const int tempOpenPin = 52;
const int tempClosePin = 53;
const int doorPin = 9;

int doorAngle = 180;
boolean isOpen;
Servo doorServo;

void setup() {
    Serial.begin(9600);

    pinMode(audioGuidancePin, OUTPUT);
    pinMode(tempOpenPin, INPUT_PULLUP);
    pinMode(tempClosePin, INPUT_PULLUP);

    isOpen = false;

    doorServo.attach(doorPin);
    doorServo.write(doorAngle);
}
 
void loop() {
    int tempOpenInput = digitalRead(tempOpenPin);
    int tempCloseInput = digitalRead(tempClosePin);
    if(tempOpenInput == LOW && tempCloseInput == LOW){
        Serial.println("Both pressed - Do nothing");
    }
    else {
        if(tempOpenInput == LOW && !isOpen){
            isOpen = true;
            
            digitalWrite(audioGuidancePin, HIGH);
            delay(10);
            digitalWrite(audioGuidancePin, LOW);

            doorAngle = 60;
            doorServo.write(doorAngle);
            Serial.println("Door open!");
        }

        if(tempCloseInput == LOW && isOpen){
            isOpen = false;
            
            doorAngle = 180;
            doorServo.write(doorAngle);
            Serial.println("Door close!");
        }
    }
}