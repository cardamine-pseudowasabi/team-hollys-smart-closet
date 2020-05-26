// Arduino Mega2560 (Main Controller part)
// 1. Touch Screen
// 2. DHT Sensing
// 3. Door Control (Servo motor)
// 4. Communication with Smartphone (Bluetooth)
// 5. Motion Sensing
// 6. Communication with Sub-Controller (UART)
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Nextion.h>
#include <DHT.h>

// pin numbering
const int motionPin = 3;
const int dhtPin = 4;
const int doorPin = 9;
const int btRx = 10;
const int btTx = 11;

// declaration class instances
SoftwareSerial btSerial(btTx, btRx);
Servo doorServo;
int doorAngle = 180;
DHT dhtModule(dhtPin, DHT11);

// enable/disable motion detect
boolean enableMotionDetect = false;

// for touch screen (Nextion)
unsigned long t1_prev = 0;            // millis 함수을 위한 변수
unsigned long t2_prev = 0;
unsigned long t1_delay = 1000;
unsigned long t2_delay = 1000;

NexButton b0 = NexButton(0,5,"b0");   // 터치스크린의 button,text,picture 객체선언 
NexButton b1 = NexButton(0,6,"b1");
NexButton b2 = NexButton(0,9,"b2");
NexButton b3 = NexButton(0,10,"b3");

NexText t1 = NexText(0,1,"t1");
NexText t2 = NexText(0,2,"t2");
NexText t3 = NexText(0,8,"t3");

NexPicture p0 = NexPicture(0,3,"p0");
NexPicture p1 = NexPicture(0,4,"p1");

NexTouch *nex_listen_list[] = { // 터치했을때 이벤트가 발생하는 요소
    &b0,
    &b1,
    &b2,
    &b3,
    NULL
};

void b0PopCallback(void *ptr) {  // b0 버튼(door open)
    doorAngle = 60;
    doorServo.write(doorAngle);
}

void b1PopCallback(void *ptr) {  // b1 버튼(door close)
    doorAngle = 180;
    doorServo.write(doorAngle);
}

void b2PopCallback(void *ptr) {  // b2 버튼(왼쪽 방향 버튼)
    /*angle = angle + 90;
    servo2.write(angle);*/

    if(Serial1.available()){
        Serial1.write(2);
        Serial.println("Stepper motor: CCW");
    }
}  

void b3PopCallback(void *ptr) { // b3 버튼(오른쪽 방향 버튼)
    /*angle = angle - 90;
    servo2.write(angle);*/

    if(Serial1.available()){
        Serial1.write(5);
        Serial.println("Stepper motor: CW");
    }
}

void getHumidity() {            // 습도 측정
    float h = dhtModule.readHumidity();
    char hTemp[10] = {0}; 
    utoa(int(h), hTemp, 10);
    t1.setText(hTemp); 
}

void on_off() {                 // 습도에 따른 제습모드 on_off  
    float h = dhtModule.readHumidity();
    if(h >= 70) {
        t3.setText("ON");       // fan도 작동시켜야됨
    }
    else {
        t3.setText("OFF");
    }
}

void setup() {
    // for monitoring
    Serial.begin(9600);
    
    // Mega <-> Uno UART communication -> Serial1 object (19:Rx, 18:Tx)
    Serial1.begin(9600);

    // Mega <-> Smartphone Bluetooth communication
    btSerial.begin(9600);

    doorServo.attach(doorPin);
    doorServo.write(doorAngle);

    // touch screen
    nexInit();
    b0.attachPop(b0PopCallback,&b0);          
    b1.attachPop(b1PopCallback,&b1);
    b2.attachPop(b2PopCallback,&b2);
    b3.attachPop(b3PopCallback,&b3);
}

void loop() {
    // setting bluetooth module
    /*if(btSerial.available()){Serial.write(btSerial.read());}
    if(Serial.available()){btSerial.write(Serial.read());}*/

    unsigned long t1_now = millis();

    if(t1_now - t1_prev >= t1_delay) {
        t1_prev = t1_now;
        getHumidity();
    }

    unsigned long t2_now = millis();

    if(t2_now - t2_prev >= t2_delay) {
        t2_prev = t2_now;
        on_off();
    }                                                                                                                                                                           

    nexLoop(nex_listen_list);
}
