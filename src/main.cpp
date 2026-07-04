#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" HomeSweetHome");
    Serial.println(" Version: Development");
    Serial.println("=================================");
}

void loop()
{
    delay(1000);
}
