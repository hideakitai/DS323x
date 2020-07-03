#include <DS323x.h>

DS323x rtc;

const uint8_t PIN_INTERRUPT = 22;
uint32_t curr_us = 0;
uint32_t prev_us = 0;

void measure()
{
    curr_us = micros();
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    rtc.attach(Wire);
    rtc.squareWaveFrequency(DS323x::SquareWaveFreq::SQWF_1_HZ);

    delay(1500);

    rtc.interruptControl(DS323x::InterruptCtrl::SQW); // default is ALRAM

    pinMode(PIN_INTERRUPT, INPUT_PULLUP);
    uint32_t origin_us = micros();
    rtc.trigger(); // first falling edge comes 1sec after this (same as second(0))
    curr_us = prev_us = (origin_us + micros()) / 2; // compensate trigger comm time
    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), measure, FALLING);
}

void loop()
{
    if (prev_us != curr_us)
    {
        uint32_t diff = curr_us - prev_us;
        prev_us = curr_us;
        Serial.print("square wave interval[us] : ");
        Serial.println(diff);
    }
}
