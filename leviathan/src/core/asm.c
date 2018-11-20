#include "asm.h"
#include "sheduler.h"


uint8_t *linitialize_stack(uint8_t* top_of_stack, ltask_fn fn)
{
    uint16_t address = 0;

    // indicates top of the stack - debug information
    *top_of_stack = 0x13;
    top_of_stack--;
    *top_of_stack = 0x37;
    top_of_stack--;

    // initialize stack so the task can be started by osTaskSwitchContext
    // task is not running, so we need to put some random values into the context

    // put osTaskExit on stack so when task exits by ret(i) instruction it will go "back" to that function
    address = (uint16_t)ltask_exit;
    *top_of_stack = (address >> 0) & 0xff;
    top_of_stack--;
    *top_of_stack = (address >> 8) & 0xff;
    top_of_stack--;

    // put start of the stack on top since it will be fetched by ret(i) instruction of the context switch
    address = (uint16_t)fn;
    *top_of_stack = (address >> 0) & 0xff;
    top_of_stack--;
    *top_of_stack = (address >> 8) & 0xff;
    top_of_stack--;

    // R0 and SREG (0x80 - interrupts enabled) registers
    *top_of_stack = 0x00;
    top_of_stack--;
    *top_of_stack = 0x80;
    top_of_stack--;

    // initialize registers R1-R23
    // R1 is expected to be zero
    *top_of_stack = 0x00;
    top_of_stack--;
    *top_of_stack = 0x02;
    top_of_stack--;
    *top_of_stack = 0x03;
    top_of_stack--;
    *top_of_stack = 0x04;
    top_of_stack--;
    *top_of_stack = 0x05;
    top_of_stack--;
    *top_of_stack = 0x06;
    top_of_stack--;
    *top_of_stack = 0x07;
    top_of_stack--;
    *top_of_stack = 0x08;
    top_of_stack--;
    *top_of_stack = 0x09;
    top_of_stack--;
    *top_of_stack = 0x10;
    top_of_stack--;
    *top_of_stack = 0x11;
    top_of_stack--;
    *top_of_stack = 0x12;
    top_of_stack--;
    *top_of_stack = 0x13;
    top_of_stack--;
    *top_of_stack = 0x14;
    top_of_stack--;
    *top_of_stack = 0x15;
    top_of_stack--;
    *top_of_stack = 0x16;
    top_of_stack--;
    *top_of_stack = 0x17;
    top_of_stack--;
    *top_of_stack = 0x18;
    top_of_stack--;
    *top_of_stack = 0x19;
    top_of_stack--;
    *top_of_stack = 0x20;
    top_of_stack--;
    *top_of_stack = 0x21;
    top_of_stack--;
    *top_of_stack = 0x22;
    top_of_stack--;
    *top_of_stack = 0x23;
    top_of_stack--;

    // initialize R24-R25
    // which is a function argument
    *top_of_stack = 0x0;
    top_of_stack--;
    *top_of_stack = 0x0;
    top_of_stack--;

    // initialize R26-R31
    *top_of_stack = 0x26;
    top_of_stack--;
    *top_of_stack = 0x27;
    top_of_stack--;
    *top_of_stack = 0x28;
    top_of_stack--;
    *top_of_stack = 0x29;
    top_of_stack--;
    *top_of_stack = 0x30;
    top_of_stack--;
    *top_of_stack = 0x31;
    top_of_stack--;

    return top_of_stack;
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