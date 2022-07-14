#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "RGB.h"

void RGB_init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x20;  // enable clock for port F
	
	GPIO_PORTF_DEN_R |= 0x0E;   // LEDS pins are Digital
	GPIO_PORTF_DIR_R |= 0X0E;	  // LEDS pins are output
	GPIO_PORTF_DATA_R &= ~0X0E; // LEDS pins are initilaized to zero
}

void led(int mask , char state)
{
	if(state == 1)
		GPIO_PORTF_DATA_R |= mask;
	else
		GPIO_PORTF_DATA_R &= ~mask;
}