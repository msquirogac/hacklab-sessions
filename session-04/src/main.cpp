#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stddef.h>
#include <util/delay.h>

/*
void _delay(uint16_t count)
{
  for (volatile size_t i = 0; i < count; i++)
  {
    asm ("nop");
    asm ("nop");
    asm ("nop");
    asm ("nop");
  }
}

#define delay(x) _delay( (x*1000L)/1275L )
*/

#define pin_on() {PORTB |= (1<<5);}
#define pin_off() {PORTB &= ~(1<<5);}
#define pin_toggle() {PORTB ^= (1<<5);}

void timer_init(void)
{
  TCCR0A = 0x00;
  TCCR0B = (1<<CS01)|(1<<CS00);
  TIMSK0 = (1<<TOIE0);
  sei();
}


volatile uint16_t ticks=0;
ISR(TIMER0_OVF_vect)
{
  ticks++;
}

void delay(uint16_t count)
{
  uint16_t start=ticks;
  uint16_t stop = start+count;
  while (stop - ticks > 0)
  {
  }  
}

int main(void)
{
  DDRB = (1<<5); 
  timer_init();
  //pin_off();
  while(1)
  {
    pin_on();
    delay(100);
    pin_off();
    delay(100);
  }
}
