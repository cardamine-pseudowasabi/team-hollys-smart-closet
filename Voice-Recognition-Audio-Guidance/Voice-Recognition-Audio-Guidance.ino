#include <SoftwareSerial.h>
#include <Servo.h>
#include <DHT.h>

const int audioGuidancePin = 50;
const int tempOpenPin = 52;
const int tempClosePin = 53;
boolean isDoorOpen;

const int dhtPin = 4;
const int doorPin = 9;
const int btRx = 10;
const int btTx = 11;

DHT dhtModule(dhtPin, DHT11);
float humidity;

// declaration class instances
SoftwareSerial btSerial(btTx, btRx);
Servo doorServo;
int doorAngle = 180;

void setup() {
    // for monitoring
    Serial.begin(9600);

    // Mega <-> Touch Screen
//    Serial3.begin(9600);
    
    // Mega <-> Uno UART communication -> Serial1 object (19:Rx, 18:Tx)
    Serial1.begin(9600);

    // Mega <-> Smartphone Bluetooth communication
    btSerial.begin(9600);

    doorServo.attach(doorPin);
    doorServo.write(doorAngle);
    

    pinMode(audioGuidancePin, OUTPUT);
    pinMode(tempOpenPin, INPUT_PULLUP);
    pinMode(tempClosePin, INPUT_PULLUP);

    isDoorOpen = false;
}

void getHumidity() {            // 습도 측정
    humidity = dhtModule.readHumidity();
    //char hTemp[10] = {0}; 
    //utoa(int(humidity), hTemp, 10);
    //t1.setText(hTemp); 
    //Serial.println(humidity);
}

unsigned long t1_prev = 0;            // millis 함수을 위한 변수
unsigned long t1_delay = 1500;

void loop() {
    unsigned long t1_now = millis();

    if(t1_now - t1_prev >= t1_delay) {
        t1_prev = t1_now;
        getHumidity();
        //btSerial.print(String("H")+String(int(humidity)));
    }

    
    // setting bluetooth module
    if(btSerial.available()){Serial.write(btSerial.read());}
    if(Serial.available()){btSerial.write(Serial.read());}

    
    int tempOpenInput = digitalRead(tempOpenPin);
    int tempCloseInput = digitalRead(tempClosePin);
    if(tempOpenInput == LOW && tempCloseInput == LOW){
        Serial.println("Both pressed - Do nothing");
    }
    else {
        if(tempOpenInput == LOW && !isDoorOpen){
            isDoorOpen = true;
            
            digitalWrite(audioGuidancePin, HIGH);
            delay(10);
            digitalWrite(audioGuidancePin, LOW);

            doorAngle = 60;
            doorServo.write(doorAngle);
            Serial.println("Door open!");
        }

        if(tempCloseInput == LOW && isDoorOpen){
            isDoorOpen = false;
            
            doorAngle = 180;
            doorServo.write(doorAngle);
            Serial.println("Door close!");
        }
    }


    // Sub -> Main 데이터 받아오기
    // 이 데이터를 다시 스마트폰으로 던져줄 것
    static String prevData, currentData;
    if(Serial1.available()){
        currentData = Serial1.readString();
        
        if(prevData != currentData){
            Serial.print(currentData);
            prevData = currentData;
        }

        btSerial.print(String("H")+String(humidity));
        btSerial.print(String("W")+currentData);
    }
}
