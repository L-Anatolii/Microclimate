#include <Arduino.h>

class Encoder{

private:
    uint8_t clk_pin;
    uint8_t dt_pin;
    uint8_t btn_pin;
    uint8_t lastState;
    int position = 0;
    int step = 0;

    const int8_t encoderTable[16] = {
        0, -1,  1,  0,
        1,  0,  0, -1,
       -1,  0,  0,  1,
        0,  1, -1,  0
    };
        // Button
    bool buttonState = HIGH;
    bool lastButtonReading = HIGH;

    bool pressedEvent = false;
    bool longPressedEvent = false;
    bool longPressTriggered = false;

    unsigned long lastDebounceTime = 0;
    unsigned long pressStartTime = 0;

    const unsigned long debounceDelay = 30;
    const unsigned long longPressTime = 1000;


public:
    Encoder(uint8_t clk_pin, uint8_t dt_pin, uint8_t btn_pin){
        this->clk_pin = clk_pin;
        this->dt_pin = dt_pin;
        this->btn_pin = btn_pin;
    };


    void begin(){
        pinMode(clk_pin, INPUT_PULLUP);
        pinMode(dt_pin, INPUT_PULLUP);
        pinMode(btn_pin, INPUT_PULLUP);

        lastState = digitalRead(clk_pin)<<1 | digitalRead(dt_pin);
        buttonState = digitalRead(btn_pin);
        lastButtonReading = buttonState;
    }

    void updateEncoder() {
        uint8_t currentState =
            (digitalRead(clk_pin) << 1) |
             digitalRead(dt_pin);

        uint8_t index = (lastState << 2) | currentState;

        step += encoderTable[index];

        lastState = currentState;     

        if(step>=4){
            step = 0;
            position++;

        }else if(step<=-4){
            step = 0;
            position--;
        }
           
    }

    void updateButton(){
        pressedEvent = false;
        longPressedEvent = false;
        
        bool reading = digitalRead(btn_pin); 
        
        if(reading != lastButtonReading){
            lastDebounceTime = millis();
            lastButtonReading = reading;
        }

        if (millis() - lastDebounceTime >= debounceDelay) {

            if (reading != buttonState) {
                buttonState = reading;

                if (buttonState == LOW) {
                    // Button pressed
                    pressStartTime = millis();
                    longPressTriggered = false;
                }
                else {
                    // Button released

                    if (!longPressTriggered) {
                        pressedEvent = true;
                    }
                }
            }
        }
        if (buttonState == LOW && !longPressTriggered) {

            if (millis() - pressStartTime >= longPressTime) {
                longPressedEvent = true;
                longPressTriggered = true;
            }
        }
    }

    int getPosition() {
        return position;
    }

    void setPosition(int value) {
        position = value;
    }

    bool isPressed() {
        return buttonState == LOW;
    }

    // Button events
    bool wasPressed() {
        return pressedEvent;
    }

    bool wasLongPressed() {
        return longPressedEvent;
    }

};