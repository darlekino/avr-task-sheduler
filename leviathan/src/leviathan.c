#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "leviathan.h"
#include "core/sheduler.h"
#include "core/common/constants.h"
#include "core/timers.h"

static void os_loop(void);

ltask_t *ltask_os = NULL;

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

	ltask_os = ltask_create(NULL, NULL);
	lsheduler_init(ltask_os);
}

void los_run(void)
{
	ENABLE_INTERRUPTS
	
	for(;;) os_loop();
}

static void os_loop(void)
{

}