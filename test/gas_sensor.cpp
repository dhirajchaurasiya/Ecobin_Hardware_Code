#include<Arduino.h>
float co2, nh3, cn_ratio;
void setup(){
    Serial.begin(9600);
    pinMode(4,INPUT);
}
void loop(){
    int mq135Value = analogRead(4);
    Serial.println(mq135Value);
    co2 = mq135Value * 0.1;
    nh3 = mq135Value * 0.08;
    cn_ratio = nh3 != 0 ? co2 / nh3 : 0;
    delay(100);
}