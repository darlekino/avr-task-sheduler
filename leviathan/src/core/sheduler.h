#ifndef _SHEDULER_H_
#define _SHEDULER_H_

#include <stdint.h>
#include "ltask.h"

#ifndef TASKS_LIMIT
#define TASKS_LIMIT 8
#endif

#define IDLE_LTASK_STACK 64

extern volatile ltask_t *current_ltask;

void sheduler_init(void);
ltask_t *ltask_create(ltask_fn fn, uint8_t *top_of_stack);
void ltask_destroy(ltask_t *ltask);
void ltask_add_to_sheduler(ltask_t *ltask);
void ltask_exit(void);
void context_switch(void);

#endif