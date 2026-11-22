
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <Encoder.h>

RTC_DS3231 rtc;


// KY-040 encoder pins
Encoder ncd(18, 19, 23);
int position = 0;

int lastCLK;
int mode = 0;

int setHour;
int setMinute;


unsigned long lastButtonTime = 0;
unsigned long lastEncoderTime = 0;

bool setting = false;

void setup() {
    Serial.begin(115200);
    ncd.begin();
}

void loop() {   
    ncd.updateEncoder();
    ncd.updateButton();

    
    if (ncd.getPosition()!=position){
        position = ncd.getPosition();
        Serial.println(ncd.getPosition());
    }
    
    if (ncd.wasPressed()){
        Serial.println("Нажал");
    }

    if (ncd.wasLongPressed()){
        Serial.println("Удержание");
    }


    

   
   
}
