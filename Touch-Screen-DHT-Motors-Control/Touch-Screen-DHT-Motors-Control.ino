#include <Nextion.h>
#include <DHT.h>
#include <Servo.h>

#define door 9        // 문 회전 서보모터
#define closet 10     // 옷장 회전 서보모터
#define DHTPIN 4
#define DHTTYPE DHT11

unsigned long t1_prev = 0;               // millis 함수을 위한 변수
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

Servo servo1;
Servo servo2;
DHT dht(DHTPIN,DHTTYPE); 
int angle = 0;


NexTouch *nex_listen_list[] =   // 터치했을때 이벤트가 발생하는 요소
{
  &b0,
  &b1,
  &b2,
  &b3,
  NULL
};

void b0PopCallback(void*ptr)  // b0 버튼(door open)
{
  servo1.write(90);
}

void b1PopCallback(void*ptr)  // b1 버튼(door close)
{
  servo1.write(0);
}

void b2PopCallback(void*ptr)  // b2 버튼(왼쪽방향버튼)
{ 
  angle = angle + 90;
  servo2.write(angle);
  
}  
void b3PopCallback(void*ptr)  // b3 버튼(오른쪽방향버튼)

{
  angle = angle - 90;
  servo2.write(angle);
  
}

void humidity()               // 습도측정
{
  float h = dht.readHumidity();
  char hTemp[10] = {0}; 
  utoa(int(h), hTemp, 10);
  t1.setText(hTemp); 
}

void on_off()                 // 습도에 따른 제습모드 on_off  
{
  float h = dht.readHumidity();
  if(h >= 70)
  {
    t3.setText("ON");           // fan도 작동시켜야됨
  }
  else
  {
    t3.setText("OFF");
  }
}

void setup()
{
  Serial.begin(9600);
  nexInit();
  servo1.attach(door);
  servo1.write(0);
  servo2.attach(closet);
  servo2.write(0);

  b0.attachPop(b0PopCallback,&b0);          
  b1.attachPop(b1PopCallback,&b1);
  b2.attachPop(b2PopCallback,&b2);
  b3.attachPop(b3PopCallback,&b3);
}
void loop()
{
  unsigned long t1_now = millis();
  if(t1_now - t1_prev >= t1_delay)
  {
    t1_prev = t1_now;
    humidity();
  }
  unsigned long t2_now = millis();
  if(t2_now - t2_prev >= t2_delay)
  {
    t2_prev = t2_now;
    on_off();
  }                                                                                                                                                                           
  nexLoop(nex_listen_list);
}