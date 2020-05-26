// Arduino Uno (Sub Controller part)
// 1. UART communication with Main-Controller
// 2. UART communication with NodeMCU
// 3. 1602 LCD Display
// 4. Stepper motor control
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // (address, column, row)
String serialRead;

SoftwareSerial fromMain(4, 5);
//SoftwareSerial fromNodeMCU(

const int stepsPerRevolution = 50;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
    Serial.begin(9600);
    fromMain.begin(9600);

    lcd.init();
    lcd.backlight();

    myStepper.setSpeed(180);
}

int motorControl = 0;
String read_data, temperature, weather_info;

void loop() {
    // 서보 모터 제어 루틴
    if(fromMain.available()){
        motorControl = Serial.read();

        if(motorControl == 2){
            myStepper.step(stepsPerRevolution);
            motorControl = 0;
        }
        else if(motorControl == 5){
            myStepper.step(-stepsPerRevolution);
            motorControl = 0;
        }
    }

    /////////////////////////////////

    // 날씨 정보 받아오는 루틴
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

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(temperature);

        lcd.setCursor(0, 1);
        lcd.print(weather_info);
    }
    delay(500);
}
