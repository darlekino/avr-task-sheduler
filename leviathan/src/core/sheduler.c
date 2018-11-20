#include <stdlib.h>
#include "sheduler.h"
#include "asm.h"

volatile ltask_t *current_ltask = NULL;

static void idle_ltask(void)
{
    while (1);
}

void sheduler_init(void)
{
    ltask_run(idle_ltask, IDLE_LTASK_STACK);
}

void ltask_run(ltask_fn fn, uint8_t stack_size)
{	
	ENABLE_INTERRUPTS
	
	static int counter = 0;
    ltask_t *ltask = (ltask_t *)malloc(sizeof(ltask_t));
    ltask->next = NULL;
    ltask->top_of_stack = linitialize_stack((uint8_t *)malloc(stack_size) + stack_size - 1, fn);
    ltask->state = LTASK_READY;
	ltask->num = counter++;

    if (!current_ltask)
    {
        current_ltask = ltask;
        current_ltask->next = ltask;
    }
    else
    {
        for(volatile ltask_t *lt = current_ltask;; lt = lt->next)
        {
            if (lt->next == current_ltask)
            {
                lt->next = ltask;
                ltask->next = current_ltask;
                break;
            }
        }
    }
	
	DISABLE_INTERRUPTS
}

void context_switch(void)
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

void ltask_destroy(ltask_t *ltask)
{
	free(ltask->top_of_stack);
    free(ltask);
}