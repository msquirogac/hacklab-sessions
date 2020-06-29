#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
}

char str[] = "Hola";

void loop()
{
    Serial.print(str);
    delay(1000);
}