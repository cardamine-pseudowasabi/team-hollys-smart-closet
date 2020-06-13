// Arduino Uno (Sub Controller part)
// 1. UART communication with Main-Controller   [Done]
// 2. UART communication with NodeMCU           [Done]
// 3. 1602 LCD Display                          [Done]
// 4. Stepper motor control                     [Done]
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// sda = a4
// scl = a5
// mega(tx1) = 4
// mega(rx1) = 5

LiquidCrystal_I2C lcd(0x27, 16, 2); // (address, column, row)
String serialRead;

SoftwareSerial fromMain(4, 5);
//SoftwareSerial fromNodeMCU(7, 8); // 이유는 모르겠지만 이렇게 하면 fromMain이 작동하지 않음

const int stepsPerRevolution = 52;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int motorControl = 0;
String read_data, temperature, weather_info;

int ClothSectionType;
int CurrentSectionType;

void setup() {
    Serial.begin(9600);
    fromMain.begin(9600);
    //fromNodeMCU.begin(9600);

    lcd.init();
    lcd.backlight();

    myStepper.setSpeed(60);

    ClothSectionType = -1;
    CurrentSectionType = 0; // 현재 옷 구역을 0 (여름 옷 구역)으로 초기화
}

unsigned long fromMain_prev = 0;            // millis 함수을 위한 변수
unsigned long delay_val = 2000;
unsigned long LCD_prev = 0;

void loop() {
    // 날씨 정보 받아오는 루틴
    unsigned long LCD_now = millis();
    read_data = Serial.readString();
    if(read_data.length()){  
        int i = read_data.indexOf("$");
        int j = read_data.indexOf("#");
        temperature = read_data.substring(0, i);
        weather_info = read_data.substring(i + 1, j);

        Serial.print("Temperature: ");
        Serial.println(temperature);
        Serial.print("Weather Info: ");
        Serial.println(weather_info);

        //int k = temperature.indexOf(".");
        /*temperature = temperature.substring(0, temperature.indexOf("."));
        // 소수점 버리고 정수형으로만 자르기*/
        int intTemp = temperature.toInt();

        //Serial.println(intTemp);

        // 기온 상태에 따라 옷장 회전
        if(intTemp > 24){ // 여름 옷 구역
            ClothSectionType = 0;
        }
        else if(intTemp > 16 && intTemp <= 24){ // 봄 옷 구역 (봄/가을 얇은 옷 구역)
            ClothSectionType = 1;
        }
        else if(intTemp > 9 && intTemp <= 16){ // 가을 옷 구역 (봄/가을 두꺼운 옷 구역)
            ClothSectionType = 2;
        }
        else { // 겨울 옷 구역
            ClothSectionType = 3;
        }

        // 현재 옷 구역과 추천 옷 구역이 다른 경우, 옷장을 회전한다.
        if(ClothSectionType != -1/* && intTemp != 0*/){
            if(ClothSectionType != CurrentSectionType){
                myStepper.step((ClothSectionType - CurrentSectionType) * stepsPerRevolution);
                delay(500);
            }
        }
    }
    
    if(LCD_now - LCD_prev >= delay_val) { // 일정 주기로 LCD에 write
        LCD_prev = LCD_now;
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(temperature);

        lcd.setCursor(0, 1);
        lcd.print(weather_info);
    }
    
    // 스텝 모터 제어 루틴
    if(fromMain.available()){ // 비동기 통신 (UART) 활성화
        // Mega2560 보드로부터 모터 제어 데이터를 수신하고, 그에 따라 스텝 모터를 회전
        motorControl = fromMain.read();
        Serial.println(motorControl);
        
        if(motorControl == 2){
            myStepper.step(-stepsPerRevolution);
            delay(500);
            motorControl = 0;
        }
        else if(motorControl == 5){
            myStepper.step(stepsPerRevolution);
            delay(500);
            motorControl = 0;
        }
    }

    // 현재 처리한 데이터를 Mega2560 보드로 전송
    unsigned long fromMain_now = millis();
    if(fromMain_now - fromMain_prev >= delay_val) {
        fromMain_prev = fromMain_now;
        fromMain.println(read_data);
        Serial.println("transmit ! ");
    }
    
    //delay(50);
    /*myStepper.step(stepsPerRevolution);
    delay(500);*/
}
