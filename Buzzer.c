#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "Buzzer.h"

/*  @Connentions
                Buzzer -> A2
*/

void Buzzer_init()
{
	SYSCTL_RCGCGPIO_R |= 0x01;   // enable clock for port A
	GPIO_PORTA_DEN_R |= 0x04;    // pin A2 is digital 
	GPIO_PORTA_DIR_R |= 0x04;    // pin A2 is output
}

void Buzzer(char state)
{
	if(state == 1)
		GPIO_PORTA_DATA_R |= 0x04;
	else
		GPIO_PORTA_DATA_R &= ~0X04;
}

