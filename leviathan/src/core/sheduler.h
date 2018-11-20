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
void ltask_run(ltask_fn fn, uint8_t stack_size);
void ltask_exit(void);
void ltask_destroy(ltask_t *ltask);
void context_switch(void);

#endif