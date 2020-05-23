#include <avr/io.h>
#include <avr/delay.h>
#include <stddef.h>

#define TX_PIN  (PIND)
#define TX_PORT (PORTD)
#define TX_DDR  (DDRD)
#define TX_NUM  (2)


/*
DDRn
PINn
PORTn
*/

void transmit(uint8_t c)
{
    // start bit
    TX_PORT &= ~(1<<TX_NUM);
    _delay_us(104);

    // data
    for(size_t i=0; i<8; i++)
    {
        if (c&0x01)
            TX_PORT |= (1<<TX_NUM);
        else
            TX_PORT &= ~(1<<TX_NUM);
        _delay_us(104);
        c = c>>1;
    }
    // stop bit
    TX_PORT |= (1<<TX_NUM);
    _delay_us(104);
}

int main(void)
{
    TX_DDR |= (1<<TX_NUM);

    TX_PORT |= (1<<TX_NUM);
    while(1)
    {
        /*
        TX_PORT |= (1<<TX_NUM);
        _delay_ms(1);
        TX_PORT &= ~(1<<TX_NUM);
        _delay_ms(1);
       TX_PORT ^= (1<<2);
       _delay_ms(1);
        */
       transmit('H');
       _delay_ms(500);
    }
}