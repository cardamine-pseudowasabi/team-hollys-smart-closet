#include <Servo.h>

Servo myservo;

const int motion_pin = 3;
int servo_pos = 0;    // variable to store the servo position

int closeopen_status;
int not_motion_count = 0;

void setup() {
    Serial.begin(9600);
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    pinMode(motion_pin, INPUT);

    closeopen_status = 0;
}

void loop() {
    int motion_read = digitalRead(motion_pin);
    Serial.print("motion info: ");
    Serial.print(motion_read);

    if(motion_read == HIGH){ // 모션 감지 될 때
        if(!closeopen_status){ // 현재 옷장이 닫혀 있는 상태라면
            for (servo_pos = 0; servo_pos <= 120; servo_pos += 1) { // 옷장 문 열기
                myservo.write(servo_pos);
                delay(20);
            }
            closeopen_status = 1; // 옷장이 열려 있는 상태
        }
        not_motion_count = 0; // 모션이 감지되어 카운트 초기화
    }
    else {
        not_motion_count ++; // 모션 감지 안 될 때 우선 카운트
    }

    delay(500);
    if(not_motion_count == 10){ // 열 번 카운트 하는 동안 모션 감지 안 되었으면
        for (servo_pos = 120; servo_pos >= 0; servo_pos -= 1) { // 옷장 문 닫기
            myservo.write(servo_pos);
            delay(20);
        }
        not_motion_count = 0; // 카운트 초기화
        closeopen_status = 0; // 옷장이 닫혀 있는 상태
    }
    Serial.print("\t");
    if(!closeopen_status){ Serial.print("Closed");}
    else { Serial.print("Open!");}
    Serial.print("\tCount: ");
    Serial.println("not_motion_count");
    
    delay(500);
}

