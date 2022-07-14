#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "SysTick.h"

void SysTicK_countSeconds(int n)
{
	int i;
	for(i = 0 ; i < n ; i++)
	{
		NVIC_ST_CTRL_R = 0;            // disable systick timer
		NVIC_ST_RELOAD_R = 80000000-1;   // reload maximun value at reload register
		NVIC_ST_CURRENT_R = 0;         // pass value in reload register to timer to start from it after enable the timer
		NVIC_ST_CTRL_R = 0x05;         // enable systick timer
		while( (NVIC_ST_CTRL_R & 0X10000) == 0){}   // monitoring bit 16 which became 1 after timer reachs 0
	}
}