#include "asm.h"
#include "sheduler.h"


uint8_t *linitialize_stack(uint8_t* top_of_stack, ltask_fn fn)
{
    uint16_t address = 0;

    // put ltask_exit on stack so when task exits by ret(i) instruction it will go "back" to that function
    address = (uint16_t)ltask_exit;
    top_of_stack[0]  = (address >> 0) & 0xff;
    top_of_stack[-1] = (address >> 8) & 0xff;

    // make space for pc, sreg, and 32 register
    address = (uint16_t)fn;
    top_of_stack[-2] = (address >> 0) & 0xff;
    top_of_stack[-3] = (address >> 8) & 0xff;

    // R0 and SREG (0x80 - interrupts enabled) registers
    top_of_stack[-4] = 0x00;
    top_of_stack[-5] = 0x80;
	
	// R1 - R31
    for (int i = -6; i > -37; i--)
    {
        top_of_stack[i] = 0;
    }
    
    return &top_of_stack[-37];
}

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