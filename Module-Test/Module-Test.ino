// Stepper motor module test
#include <Stepper.h>

const int stepsPerRevolution = 50;
const int stepsPerRevolution = 52;
// 50 = 90도
// 52 -> 오차 보정

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
// 순서대로 8, 9, 10, 11번 핀

void setup() {
    myStepper.setSpeed(180);
    // 각도를 줄이면 속도를 높이고
    // 각도를 늘리면 속도를 줄여도 잘 돌아가는 듯
}

void loop() {
    myStepper.step(stepsPerRevolution);
    delay(2000);
}