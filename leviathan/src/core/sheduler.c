#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "sheduler.h"
#include "common/asm.h"

ltask_t *current_ltask = NULL;

static void ltask_exit(void);
static void lcontext_switch(void);
static void los_non_savable_yield(void);
static void los_asm_yield_from_tick(void);

void TIMER1_OVF_vect(void) __attribute__ ((signal));
void TIMER1_OVF_vect(void)
{
	los_asm_yield_from_tick();
}

void lsheduler_init(ltask_t *main_ltask)
{
    ltask_add_to_sheduler(main_ltask);
}

void ltask_add_to_sheduler(ltask_t *ltask)
{	
	if (!current_ltask)
	{
		current_ltask = ltask;
		current_ltask->next = ltask;
	}
	else
	{
		for(ltask_t *lt = current_ltask;; lt = lt->next)
		{
			if (lt->next == current_ltask)
			{
				lt->next = ltask;
				ltask->next = current_ltask;
				break;
			}
		}
	}
}

void lcontext_switch(void)
{
    if (current_ltask->next == current_ltask)
        return;

    if (current_ltask->state == LTASK_ACTIVE)
        current_ltask->state = LTASK_READY;

    volatile ltask_t *ltask = current_ltask;

	while (!((ltask = ltask->next)->state == LTASK_READY));

	ltask->state = LTASK_ACTIVE;
	current_ltask = ltask;
}

void ltask_exit(void)
{
    DISABLE_INTERRUPTS
	
    ltask_t *prev_ltask = current_ltask;

    while (!((prev_ltask = prev_ltask->next)->next == current_ltask));

    prev_ltask->next = current_ltask->next;

	ltask_destroy(current_ltask);
    current_ltask = prev_ltask;
    los_non_savable_yield();
}

uint8_t *linitialize_stack(uint8_t *top_of_stack, ltask_fn fn)
{
	uint16_t address = 0;

	// put ltask_exit on stack so when task exits by ret(i) instruction it will go "back" to that function
	address = (uint16_t)ltask_exit;
	top_of_stack[0]  = (address >> 0) & 0xff;
	top_of_stack[-1] = (address >> 8) & 0xff;

	// make space for pc, sreg, and 32 register
	address = (uint16_t)fn;
	top_of_stack[-2] = (address >> 0) & 0xff;
	top_of_stack[-3] = (address >> 8) & 0xff;

	// R0 and SREG (0x80 - interrupts enabled) registers
	top_of_stack[-4] = 0x00;
	top_of_stack[-5] = 0x80;
	
	// R1 - R31
	for (int i = -6; i > -37; i--)
	{
		top_of_stack[i] = 0;
	}
	
	return &top_of_stack[-37];
}

void los_non_savable_yield(void)
{
	lcontext_switch();
	RESTORE_CONTEXT
}

void los_asm_yield_from_tick(void)
{
	SAVE_CONTEXT
	lcontext_switch();
	RESTORE_CONTEXT
}