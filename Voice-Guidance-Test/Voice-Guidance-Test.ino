#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

void setup () {
    Serial.begin (9600);
    mp3_set_serial (Serial);  //set Serial for DFPlayer-mini mp3 module 
    delay(1);  //wait 1ms for mp3 module to set volume
    mp3_set_volume (15);
}

const int delay_dat[] = {8500, 4500, 3500, 2500, 2500, 3500, 2500, 3500, 3500, 2500};

void loop () {        
    for(int i=0;i<10;++i){
        mp3_play(i+1);
        delay(delay_dat[i]);
    }
}
