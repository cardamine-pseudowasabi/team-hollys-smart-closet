// ESP8266 (NodeMCU) - 날씨 정보 받아와서 처리 후,
// Arduino Uno 방향으로 Tx

// 지금 상태는 일별 날씨 데이터를 전부 불러와서 파싱하는 방식인데,
// 이를 수정해서 서울/경기 지역별 현재 날씨, 오늘의 강수량 및 강수 확률, 일교차 정보 출력해주고
// 미세먼지 api 개발자 등록 후 데이터 받아와서 추가적으로 처리할 예정

// 추가적으로 VS Code로 작성 시 한 프로젝트 폴더에서 동시에 두 개 이상의 MCU를 제어할 수 없는데,
// 이를 해결하기 위한 방안으로
// 1. VS Code와 Arduino IDE 동시 사용 (Arduino IDE에서 텍스트 편집 후 저장 시 VS Code에 실시간 반영 확인됨)
// 2. git 브랜치를 나누어서 보드별로 처리하는 방식 (VS Code 작업 창 여러 개를 띄워서 개발 환경을 만들 수 있음)
// 3. 보드별 스케치 폴더를 별도의 VS Code 별도의 작업 창으로 실행하여 처리
//
// 3번 방식으로 하면 전체 프로젝트 코드를 하나의 상위 폴더 (team-hollys-smart-closet)에서 관리 가능
// 3번 방식이 유효하다는 검증 완료 (2020-05-09 14:09)
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "ignorethis.h"

SoftwareSerial WiFi2Ardu(3, 1); // GPIO 3 = RX, GPIO 1 = TX

const char* ssid = my_ssid;  // AP SSID
const char* password = my_pw; // AP password

const char * rss = "/wid/queryDFSRSS.jsp?zone=4111156600";
const char * server = "www.kma.go.kr";
const int port_num = 80;


void setup() {
	Serial.begin(9600);
	WiFi2Ardu.begin(9600);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	Serial.println("WiFi Connection Initiate");

	while (WiFi.status() != WL_CONNECTED) {
		Serial.println("connecting...");
		delay(1000);
	}

	Serial.println("WiFi Connected !");
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	WiFiClient client;

	String temperature, weather_info;
	String tmp;

	if (client.connect(server, port_num)) {
		client.print(String("GET ") + rss + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");
		delay(10); int i;

		while(client.available()){
			String line = client.readStringUntil('\n');

			i= line.indexOf("</temp>");
			if (i > 0) {
				tmp = "<temp>";
				temperature = line.substring(line.indexOf(tmp) + tmp.length(), i);
				Serial.print("Temperature: ");
				temperature = temperature.substring(0, temperature.indexOf("."));
				Serial.println(temperature);
			}

			i= line.indexOf("</wfEn>");
			if (i > 0) {
				tmp = "<wfEn>";
				weather_info = line.substring(line.indexOf(tmp) + tmp.length(), i);
				Serial.print("Weather: ");
				Serial.println(weather_info);  
			}
		}
	}

	if(temperature.length() && weather_info.length()){
		WiFi2Ardu.print(String(temperature)+"'C$"+weather_info+"#");
		digitalWrite(LED_BUILTIN, HIGH);
	} // $ = concatenate, # = terminate

	delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
}
