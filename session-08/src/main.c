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

// Rows
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define PORT_R PORTB

// Columns
#define C0 4
#define C1 5
#define C2 6
#define C3 7
#define PORT_C PORTD

void keypad_init()
{
    DDRB = 0x00;
    PORT_R = 0x00;
    DDRD = 0x00;
    PORT_C = _BV(C0) | _BV(C1) | _BV(C2) | _BV(C3);
}

void keypad_ctrl(uint8_t row)
{
    DDRB = _BV(row);
}

uint8_t keypad_read(void)
{
    return ((~PIND) >> 4) & 0x0F;
}

char maps[2][4] = {
    {'1','2','3','4'},
    {'+','-','*','='}
    };

uint8_t map0(uint8_t v)
{
    if(v)
    {
        uint8_t r=0;
        while (v >>= 1)
        {
            r++;
        }
        return maps[0][r];
    }
    return 0;
}

uint8_t map1(uint8_t v)
{
    if(v)
    {
        uint8_t r=0;
        while (v >>= 1)
        {
            r++;
        }
        return maps[1][r];
    }
    return 0;
}

uint8_t keysOld[4];

struct
{
    char mem[4][6];
    uint8_t row;
    uint8_t col;
} stack;

volatile uint8_t key;

int main()
{
    DDRC = _BV(D4) | _BV(D5) | _BV(D6) | _BV(D7) | _BV(RS) | _BV(E);
    PORTC = 0x00;
    lcd_init();
    uart_init();
    keypad_init();
    memset(stack.mem, 0, 6);
    while(1)
    {
        _delay_ms(100);
        uint8_t keys[4];
        uint8_t event[4];
        for(uint8_t i=0; i<4; i++)
        {
            keypad_ctrl(i);
            keys[i] = keypad_read();
            event[i] = (keysOld[i]^keys[i])&keys[i];
            keysOld[i] = keys[i];
            _delay_ms(1);
        }

        key = map0(event[0]);
        if(key)
        {
            if(stack.col < 5)
            {
                stack.mem[stack.row][stack.col] = key;
                stack.col++;
            }
        }
        key = map1(event[1]);
        if(key)
        {
            switch (key)
            {
            case '=':
            {
                if(stack.col > 0)
                {
                    stack.col = 0;
                    stack.row = (stack.row+1)%4;
                    memset(stack.mem[stack.row], 0, 6);
                }
            } break;
            case '+':
            {
                uint8_t row0, row1;
                char *p1, *p2;
                if(stack.col == 0)
                {
                    stack.row = (stack.row+3)%4;
                }
                row0 = (stack.row+3)%4;
                row1 = stack.row;
                p1 = stack.mem[row0];
                p2 = stack.mem[row1];
                uint16_t num1 = atoi(p1);
                uint16_t num2 = atoi(p2);
                itoa(num1+num2, p1, 10);
                stack.col = 0;
                memset(stack.mem[stack.row], 0, 6);
            } break;
            default:
                break;
            }
        }
        /*
        uint8_t row0 = (stack.row == 0)? 3: stack.row-1;
        uint8_t row1 = stack.row;
        */
        uint8_t row0 = (stack.row+3)%4;
        uint8_t row1 = stack.row;
        sprintf(l1, "%15s", stack.mem[row0]);
        sprintf(l2, "%15s", stack.mem[row1]);

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