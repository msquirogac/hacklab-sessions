#include <Arduino.h>

#define BAUDRATE 115200
#define ubrr ((F_CPU/(BAUDRATE*16)-1))

int unused(uint16_t *x)
{
    return *x;
}

void setup()
{
    UBRR0H = (uint8_t)(0);
    UBRR0L = (uint8_t)8;
    UCSR0A = 0x00;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
    UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
    //Serial.begin(115200);
}

void transmit(uint8_t c)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void write(uint8_t *arr, size_t num)
{
    for(size_t i=0; i<num; i++)
    {
        transmit(arr[i]);
    }
}

void print(uint8_t *arr)
{
    while(*arr != 0)
    {
        transmit(*arr);
        arr++;
    }
}

void println(uint8_t *arr)
{
    while(*arr != 0)
    {
        transmit(*arr);
        arr++;
    }
    transmit('\r');
    transmit('\n');
}

void loop()
{
    print("Hola\n");
    println("Adios");
    delay(500);
    //Serial.print("Hola mundo\n");
}