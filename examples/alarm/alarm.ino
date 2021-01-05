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

    // set alarm1
    rtc.format(DS323x::AlarmSel::A1, DS323x::Format::HOUR_12);
    rtc.dydt(DS323x::AlarmSel::A1, DS323x::DYDT::DYDT_DATE);
    rtc.ampm(DS323x::AlarmSel::A1, DS323x::AMPM::AMPM_AM);
    rtc.weekday(DS323x::AlarmSel::A1, 21);
    rtc.hour(DS323x::AlarmSel::A1, 12);
    rtc.minute(DS323x::AlarmSel::A1, 34);
    rtc.second(DS323x::AlarmSel::A1, 56);
    rtc.rate(DS323x::A1Rate::MATCH_SECOND);

    Serial.print("Alarm 1 is set to  : ");
    Serial.println(rtc.alarm(DS323x::AlarmSel::A1).timestamp());
    Serial.print("Alarm 1 alarm rate : ");
    Serial.println((uint8_t)rtc.rateA1());

    // set alarm2
    rtc.format(DS323x::AlarmSel::A2, DS323x::Format::HOUR_24);
    rtc.dydt(DS323x::AlarmSel::A2, DS323x::DYDT::DYDT_DAY);
    rtc.ampm(DS323x::AlarmSel::A2, DS323x::AMPM::AMPM_PM);
    rtc.day(DS323x::AlarmSel::A2, 12);
    rtc.hour(DS323x::AlarmSel::A2, 23);
    rtc.minute(DS323x::AlarmSel::A2, 45);
    rtc.rate(DS323x::A2Rate::MATCH_MINUTE);

    Serial.print("Alarm 2 is set to  : ");
    Serial.println(rtc.alarm(DS323x::AlarmSel::A2).timestamp());
    Serial.print("Alarm 2 alarm rate : ");
    Serial.println((uint8_t)rtc.rateA2());

    pinMode(PIN_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), measure, FALLING);

    // alarm flags must be cleard to get next alarm
    if (rtc.hasAlarmed(DS323x::AlarmSel::A1))
        rtc.clearAlarm(DS323x::AlarmSel::A1);
    if (rtc.hasAlarmed(DS323x::AlarmSel::A2))
        rtc.clearAlarm(DS323x::AlarmSel::A2);

    // enable alarm1/2
    rtc.interruptControl(DS323x::InterruptCtrl::ALARM);
    rtc.enableAlarm1(true);
    rtc.enableAlarm2(true);

    // reset rtc time
    uint32_t origin_us = micros();
    rtc.now(DateTime(2020, 11, 23, 14, 44, 50));
    curr_us = prev_us = (origin_us + micros()) / 2; // compensate trigger comm time
}

void loop()
{
    if (prev_us != curr_us)
    {
        prev_us = curr_us;

        DateTime now = rtc.now();
        Serial.print("alarm has come! current time : ");
        Serial.println(now.timestamp());

        // alarm flags must be cleard to get next alarm
        if (rtc.hasAlarmed(DS323x::AlarmSel::A1))
            rtc.clearAlarm(DS323x::AlarmSel::A1);
        if (rtc.hasAlarmed(DS323x::AlarmSel::A2))
            rtc.clearAlarm(DS323x::AlarmSel::A2);
    }

    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 1000)
    {
        DateTime now = rtc.now();
        Serial.println(now.timestamp());
        prev_ms = millis();
    }
}
