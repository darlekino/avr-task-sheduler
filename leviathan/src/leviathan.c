#include <avr/io.h>
#include <avr/interrupt.h>
#include "leviathan.h"
#include "core/sheduler.h"
#include "core/asm.h"
#include "core/constants.h"

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

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

void TIMER1_OVF_vect(void) __attribute__ ((signal));
void TIMER1_OVF_vect(void)
{
	los_asm_yield_from_tick();
}

unsigned long millis() 
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

void ltask_run(ltask_fn fn, uint8_t stack_size)
{
	DISABLE_INTERRUPTS

	uint8_t *top_of_stack = linitialize_stack((uint8_t *)malloc(stack_size) + stack_size - 1, fn);
	ltask_t *ltask = ltask_create(fn, top_of_stack);
	ltask_add_to_sheduler(ltask);

	ENABLE_INTERRUPTS
}

void ltask_run_v2(ltask_fn fn, uint8_t* top_of_stack)
{
	DISABLE_INTERRUPTS

	top_of_stack = linitialize_stack(top_of_stack, fn);
	ltask_t *ltask = ltask_create(fn, top_of_stack);
	ltask_add_to_sheduler(ltask);

	ENABLE_INTERRUPTS
}

void init_timer0() 
{
	//F_CPU/64  1ms (register 8bit)
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);

	sbi(TIMSK0, TOIE0);
}

void init_timer1()
{	
	//F_CPU/1  0.004ms (register 16bit)
	sbi(TCCR1B, CS10);

	sbi(TIMSK1, TOIE1);
}


void los_init(void)
{
    sheduler_init();

    init_timer0();
	init_timer1();
}

void los_run(void)
{
    ENABLE_INTERRUPTS
	
	// yield without saving the context
	los_non_savable_yield();
}