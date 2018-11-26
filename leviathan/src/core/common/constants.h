#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define ENABLE_INTERRUPTS 	asm volatile("sei");
#define DISABLE_INTERRUPTS 	asm volatile("cli");

#define SUCCESS_RESULT  0
#define FAILED_RESULT  -1

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif