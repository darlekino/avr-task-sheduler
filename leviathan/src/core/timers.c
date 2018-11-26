#include <avr/io.h>
#include <avr/interrupt.h>
#include "common/constants.h"
#include "timers.h"

volatile unsigned long timer0_millis = 0;

void TIMER0_OVF_vect(void) __attribute__ ((signal));
void TIMER0_OVF_vect(void)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	m += 1;
	timer0_millis = m;
}

unsigned long millis(void)
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	DISABLE_INTERRUPTS

	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

void init_timer0(void)
{
	//F_CPU/64  1ms (register 8bit)
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	sbi(TIMSK0, TOIE0);
}

void init_timer1(void)
{
	//F_CPU/1  0.004ms (register 16bit)
	sbi(TCCR1B, CS10);

	sbi(TIMSK1, TOIE1);
}