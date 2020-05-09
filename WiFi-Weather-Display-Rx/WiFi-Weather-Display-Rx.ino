// NodeMCU -> Arduino Uno (Rx)
// 시리얼 통신(SoftwareSerial)으로 LCD에 표시할 각종 정보 받아옴
// NodeMCU의 출력 전압이 3v3이어서 LCD 직접 제어가 불가능하고 (전압 증폭을 하면 모를까)
// 3v3 전압을 이용하는 12864 OLed는 호환되지 않는다고 해서,
// 각종 정보 디스플레이 (스위치를 통해 지역별, 강수량/확률, 일교차 정보 표시할 예정) 하기 위한
// 서브 아두이노 : 이 말은, 메인 아두이노를 별도로 두고 서브 방향에서 또 정보를 받아올 예정.
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // (address, column, row)
String serialRead;

void setup(){
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

void loop(){
    String read_data;
    read_data = Serial.readString();

    if(read_data.length()){  
        int i = read_data.indexOf("$");
        int j = read_data.indexOf("#");
        String temperature = read_data.substring(0, i);
        String weather_info = read_data.substring(i + 1, j);

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
 
    delay(1000);
}
