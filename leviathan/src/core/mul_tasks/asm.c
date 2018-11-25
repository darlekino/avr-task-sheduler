#include "asm.h"
#include "sheduler.h"

void los_non_savable_yield(void)
{
	context_switch();
	RESTORE_CONTEXT
}

void los_asm_yield_from_tick(void)
{
    SAVE_CONTEXT
    context_switch();
    RESTORE_CONTEXT
}