#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stddef.h>
#include <util/delay.h>

/*
void _delay(uint16_t count)avr_mcu_section.h>
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

void pin1_on() {PORTB |= (1<<5);}
void pin1_off() {PORTB &= ~(1<<5);}
void pin1_toggle() {PORTB ^= (1<<5);}

void pin2_on() {PORTB |= (1<<4);}
void pin2_off() {PORTB &= ~(1<<4);}
void pin2_toggle() {PORTB ^= (1<<4);}

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

/*
void delay(uint16_t count)
{
  uint16_t start=ticks;
  uint16_t stop = start+count;
  while (stop - ticks > 0)
  {
  }  
}
*/
/*
typedef void (*faddr)();
faddr filler __attribute__((used)) = NULL;
*/

typedef struct main
{
  void (*fun)();
  uint16_t pticks;
} task_t;

void task(task_t *t, uint16_t period)
{
  uint16_t cticks = ticks;
  if(cticks - t->pticks >= period)
  {
    t->pticks = cticks;
    t->fun();
  }
}

task_t t1 = {&pin1_toggle, 0};
task_t t2 = {&pin2_toggle, 0};

int main(void)
{
  DDRB = (1<<5)|(1<<4);
  timer_init();
  while(1)
  {
    task(&t1, 200);
    task(&t2, 300);
  }
}

/*
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
	{ AVR_MCU_VCD_SYMBOL("UDR0"), .what = (void*)&UDR0, },
	{ AVR_MCU_VCD_SYMBOL("UDRE0"), .mask = (1 << UDRE0), .what = (void*)&UCSR0A, },
};
*/