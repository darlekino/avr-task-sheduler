#ifndef _SHEDULER_H_
#define _SHEDULER_H_

#include <stdint.h>
#include "ltask.h"

#ifndef TASKS_LIMIT
#define TASKS_LIMIT 8
#endif

extern ltask_t *current_ltask;

void sheduler_init(void);
uint8_t *linitialize_stack(uint8_t* top_of_stack, ltask_fn fn);
void ltask_add_to_sheduler(ltask_t *ltask);
void ltask_exit(void);
void context_switch(void);

#endif