#include <avr/io.h>
#include <util/delay.h>
#include "src/leviathan.h"
#include "src/core/asm.h"

/*
nano
"C:\Program Files (x86)\Arduino\hardware\tools\avr\bin\avrdude.exe" -CC:"\Program Files (x86)\Arduino\hardware\tools\avr\etc\avrdude.conf" -v -patmega328p -carduino -PCOM4 -b57600 -D -Uflash:w:D:\projects\leviathan\leviathan\Debug\leviathan.hex:i

uno
"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude.exe" -CC:"\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -patmega328p -carduino -PCOM4 -b115200 -D -Uflash:w:D:\projects\leviathan\leviathan\Debug\leviathan.hex:i
*/

void task01(void);
void task02(void);
void task03(void);

uint8_t stack_of_task01[64];
uint8_t stack_of_task02[64];

void task01()
{
	//ltask_run_v2(task02, 64);
	ltask_run_v2(task02, &stack_of_task02[63]);
	#define LED_PIN 2
	static unsigned long last_time = 0;
	static int led_on = 1;

	DDRB |= 1 << LED_PIN;
	
	while(1) 
	{
		unsigned int current = millis();
		if (current - last_time >= 500)
		{
			if(led_on)
			{
				PORTB |= 1 << LED_PIN;	
			}
			else
			{
				PORTB &= ~(1 << LED_PIN);	
			}
			led_on = !led_on;
			last_time = current;	
		}
	}
}

void task02()
{
	#define LED_PIN 3
	static unsigned long last_time = 0;
	static int led_on = 1;
	
	DDRB |= 1 << LED_PIN;

	while (1)
	{
		unsigned int current = millis();
		if (current - last_time >= 500)
		{
			if(led_on)
			{
				PORTB |= 1 << LED_PIN;
			}
			else
			{
				PORTB &= ~(1 << LED_PIN);
				//break;
			}
			
			led_on = !led_on;
			last_time = current;
		}
	}
}

void task03()
{
	#define LED_PIN 1
	static unsigned long last_time = 0;
	static int led_on = 1;

	DDRB |= 1 << LED_PIN;

	while (1)
	{
		unsigned int current = millis();
		if (current - last_time >= 500)
		{
			if(led_on)
			{
				PORTB |= 1 << LED_PIN;
			}
			else
			{
				PORTB &= ~(1 << LED_PIN);
			}
			
			led_on = !led_on;
			last_time = current;
		}
	}
}

int main(void) 
{	
	los_init();
	
	//ltask_run(task01, 255);
	//ltask_run(task02, 255);
	
	ltask_run_v2(task01, &stack_of_task01[63]);
	//ltask_run_v2(task02, &stack_of_task02[63]);
	
	los_run();
    return 0;
}
