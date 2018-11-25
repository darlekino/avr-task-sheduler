#ifndef _LEVIATHAN_H_
#define _LEVIATHAN_H_

#include "core/ltask.h"

unsigned long millis(void);

void los_init(void);
void los_run(void);

void ltask_run(ltask_fn fn, uint8_t stack_size);
void ltask_run_v2(ltask_fn fn, uint8_t* top_of_stack);

#endif