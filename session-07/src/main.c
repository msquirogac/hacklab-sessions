#include <Arduino.h>
#include <avr_debugger.h>

void setup()
{
    dbg_start();
    pinMode(13, OUTPUT);
}

void loop()
{
    dbg_message("Hello World");
    digitalWrite(13, !digitalRead(13));
}