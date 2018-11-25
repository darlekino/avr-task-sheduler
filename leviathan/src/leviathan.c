#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "leviathan.h"
#include "core/mul_tasks/sheduler.h"
#include "core/mul_tasks/asm.h"
#include "core/constants.h"
#include "core/time/timers.h"

ltask_t *ltask_os = NULL;
void main_loop(void);

void TIMER1_OVF_vect(void) __attribute__ ((signal));
void TIMER1_OVF_vect(void)
{
	los_asm_yield_from_tick();
}

void ltask_run(ltask_fn fn, size_t stack_size)
{
	DISABLE_INTERRUPTS

	uint8_t *stack = (uint8_t *)malloc(stack_size);
	uint8_t *top_of_stack = linitialize_stack(stack + stack_size - 1, fn);
	ltask_t *ltask = ltask_create(stack, top_of_stack);
	ltask_add_to_sheduler(ltask);

	ENABLE_INTERRUPTS
}

void los_init(void)
{    
    init_timer0();
	init_timer1();

	ltask_os = ltask_create(NULL);
	sheduler_init(ltask_os);
}

void los_run(void)
{
	ENABLE_INTERRUPTS
	
	for(;;) main_loop();
}

void main_loop(void)
{
	 
}