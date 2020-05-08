// EtherCard (ENC28J60) 를 이용한 기상청 RSS 데이터 불러오기
#include <EtherCard.h> // Library - https://github.com/njh/EtherCard

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[3000];
static uint32_t timer;
const char server[] PROGMEM = "www.kma.go.kr";
// PROGMEM: Store data in flash memory instead of SRAM (https://www.arduino.cc/en/pmwiki.php?n=Reference/PROGMEM)
//byte buffer2[2000];
int count_callback;

// browseUrl 에 등록할 콜백 함수
static void website_callback (byte status, word offset, word length) {
    ++ count_callback;
    Serial.println(); Serial.print("[callback start] "); Serial.println(count_callback);
    Serial.print("status: "); Serial.println(status);
    Serial.print("offset: "); Serial.println(offset);
    Serial.print("length: "); Serial.println(length);
    Ethernet::buffer[offset+2500] = 0;
    Serial.print((const char*) Ethernet::buffer + offset);
    /*Ethernet::readPacketSlice(buffer2, 500, len);
    Ethernet::packetReceive();
    Ethernet::bufferSize
    Serial.println((const char*) buffer2);*/
    Serial.println(); Serial.print("[callback end] "); Serial.println(count_callback);
}
// 전체 텍스트 길이 정보는 받아와지는데, 읽어들이는 패킷의 길이가 512 byte 로 고정
// 중간에 정보 잘림 문제 발생
// 전체 텍스트를 읽어 오기에는 아두이노 메모리 부족
// Ethernet.h 혹은 WiFi 쉴드 사용할 때 쓰는 라인별로 읽어오는 메소드가 있으면 좋을텐데....

void setup () {
    Serial.begin(57600);
    Serial.println("Korea Weather RSS by Ethernet Card (EtherCard)");

    // 53 pin (Arduino Mega2560)
    if (!ether.begin(sizeof Ethernet::buffer, mymac, 53)){
        Serial.println("Ethernet controller failure");
    }
    if (!ether.dhcpSetup()){
        Serial.println("DHCP failed");
    }

    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GateWay:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);

    // IP 주소를 구하기 위한 DNS 이용
    if (!ether.dnsLookup(server)){
        Serial.println("DNS failed");
    }

    count_callback = 0;
}

void loop () {
    ether.packetLoop(ether.packetReceive());

    if (millis() > timer) {
        timer = millis() + 5000;
        Serial.println();
        Serial.print("[Requesting]");
        ether.browseUrl(PSTR("/"), "wid/queryDFSRSS.jsp?zone=1129070500", server, website_callback);
        // 한국 기상청 RSS
    }
}