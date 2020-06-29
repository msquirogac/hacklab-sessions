#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define D4 0
#define D5 1
#define D6 2
#define D7 3
#define RS 4
#define E  5

#define setRS() {PORTC |= _BV(RS);}
#define clrRS() {PORTC &= ~_BV(RS);}
#define setE() {PORTC |= _BV(E);}
#define clrE() {PORTC &= ~_BV(E);}
#define wrtData(x) {PORTC = (PORTC&0xF0) |(x&0x0F);}

void write4b(uint8_t rs, uint8_t data)
{
    clrE();
    if(rs) {
        setRS();
        }
    else {
        clrRS();
    }
    wrtData(data);
    setE();
    _delay_us(10);
    clrE();
    _delay_us(10);
}

void write8b(uint8_t rs, uint8_t data)
{
    write4b(rs, data>>4);
    write4b(rs, data>>0);
}

void lcd_init()
{
    write4b(0, 0x03);
    _delay_ms(10);
    write4b(0, 0x03);
    _delay_ms(10);
    write4b(0, 0x03);
    _delay_ms(10);
    write4b(0, 0x02);
    _delay_ms(1);

    write8b(0, 0x28);
    _delay_ms(1);
    write8b(0, 0x0C);
    _delay_ms(1);
    write8b(0, 0x01);
    _delay_ms(1);
}

void lcd_setCursor(uint8_t row, uint8_t col)
{
    uint8_t offset = (row > 0)? 0x40: 0x00;
    uint8_t addr = offset + col;
    write8b(0, (1<<7)|(addr));
    _delay_ms(10);
}

void lcd_write(uint8_t *arr, size_t num)
{
    for(size_t i=0; i<num; i++)
    {
        write8b(1, arr[i]);
    }
}





#define BAUDRATE 9600UL
void uart_init()
{
    volatile uint32_t ubrr = (F_CPU/(16L*BAUDRATE))-1;
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0A = 0x00;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
    UCSR0C = _BV(USBS0) | _BV(UCSZ01) | _BV(UCSZ00);
    //Serial.begin(115200);
}

void uart_transmit(uint8_t c)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void uart_write(uint8_t *arr, size_t num)
{
    for(size_t i=0; i<num; i++)
    {
        uart_transmit(arr[i]);
    }
}

char l1[16];
char l2[16];

int main()
{
    DDRC = _BV(D4) | _BV(D5) | _BV(D6) | _BV(D7) | _BV(RS) | _BV(E);
    PORTC = 0x00;
    lcd_init();
    uart_init();
    DDRB = _BV(5);
    while(1)
    {
        _delay_ms(100);
        PORTB ^= _BV(5);

        sprintf(l1, "%15d", 1234);
        sprintf(l2, "%15d", 12354);

        lcd_setCursor(0, 0);
        lcd_write(l1, 15);
        lcd_setCursor(1, 0);
        lcd_write(l2, 15);

        uart_write(l1, 15);
        uart_write("\n", 1);
        uart_write(l2, 15);
        uart_write("\n", 1);
    }
}