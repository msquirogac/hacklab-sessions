#include <Arduino.h>

#define BAUDRATE 9600UL
//#define ubrr ((F_CPU/(BAUDRATE*16))-1)

#define UNUSED(x) *((volatile uint8_t*)(&x))

int unused(uint16_t *x)
{
    return *x;
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

void setup()
{
    volatile uint32_t ubrr = (F_CPU/(16L*BAUDRATE))-1;
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0A = 0x00;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
    UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
    //Serial.begin(115200);
}

void nothing(void)
{
    volatile uint16_t a=0;
    a++;
}

void loop()
{
    //*((volatile uint16_t*)(&ubrr)) = ubrr;
    
    //UNUSED(ubrr);
    nothing();
    print("Hola\n");
    println("Adios");
    delay(500);
    //Serial.print("Hola mundo\n");
}