#include <stdlib.h>
#include <stdint.h>
#include "ltask.h"

ltask_t *ltask_create(uint8_t *stack, uint8_t* top_of_stack)
{
	static int counter = 0;
	ltask_t *ltask = (ltask_t *)malloc(sizeof(ltask_t));
	ltask->next = NULL;
	ltask->stack = stack;
	ltask->top_of_stack = top_of_stack;
	ltask->state = LTASK_READY;
	ltask->num = counter++;
	return ltask;
}

void ltask_destroy(ltask_t *ltask)
{
	free((void *)ltask->stack);
	free((void *)ltask);
}