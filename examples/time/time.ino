#include <DS323x.h>

DS323x rtc;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    rtc.attach(Wire);
    rtc.now(DateTime(2020, 11, 23, 14, 23, 45));
}

void loop()
{
    DateTime now = rtc.now();
    Serial.println(now.timestamp());
    delay(1000);
}
