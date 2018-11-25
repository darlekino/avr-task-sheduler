#ifndef _LTASK_H_
#define _LTASK_H_

#include "../constants.h"

typedef enum { LTASK_READY, LTASK_WAIT, LTASK_ACTIVE } ltask_state_t;

typedef struct ltask_t
{
    volatile uint8_t *top_of_stack;
    volatile uint8_t *stack;
    volatile ltask_state_t state;
    volatile struct ltask_t *next;
	volatile int num;
} ltask_t;

typedef void (*ltask_fn)(void);

ltask_t *ltask_create(uint8_t *stack, uint8_t* top_of_stack);
void ltask_destroy(ltask_t *ltask);

#endif