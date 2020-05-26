
void setup(){
    pinMode(40, OUTPUT);
}

void loop(){
    digitalWrite(40, HIGH);
    delay(3000);
    digitalWrite(40, LOW);
    delay(3000);
}