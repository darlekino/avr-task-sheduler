#ifndef _LEVIATHAN_H_
#define _LEVIATHAN_H_

#include "stdlib.h"
#include "core/common/ltask.h"

unsigned long millis(void);

void los_init(void);
void los_run(void);

void ltask_run(ltask_fn fn, size_t stack_size);

#endif