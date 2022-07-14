#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "Switches.h"
#include "RGB.h"
#include "Delay.h"
#include "Macros.h"

/*  @Connentions
									SW1  -> F4
									SW2  -> F0
									SW3  -> A3
*/

extern char  cooking;
extern int  second;
char paused = 0;

void Switches_init()
{
	SYSCTL_RCGCGPIO_R |= 0x21;    // enable clock for port A,F
	
	/* F0 is locked to unlock it we put special value in lock register and set bit 0 in commit register */
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   GPIO_PORTF_CR_R |= 0x01;
	
	
	GPIO_PORTF_DEN_R |= 0x11;       // F0, F1 are digital
	GPIO_PORTA_DEN_R |= 0x08;       // A3 is digital
	
	GPIO_PORTF_DIR_R &= ~0x11;      // F0, F1 are input
	GPIO_PORTA_DIR_R &= ~0X08;      // A3 is input
	
	GPIO_PORTF_PUR_R |= 0X11;       // F0, F1 are pull up resistors
	GPIO_PORTA_PUR_R |= 0x08;       // A3 is pull up resistor
	
	GPIO_PORTF_IM_R  |= 0x10;   // enable interrupt functionality on sw1
	GPIO_PORTF_IS_R  &= ~0x10;  // make F4 edge senstive        
	GPIO_PORTF_IBE_R |= 0x10;   //controlled by  GPIO_PORTF_IEV_R   
	GPIO_PORTF_IEV_R &= ~0x10;  //falling edge 
	GPIO_PORTF_ICR_R |= 0xFF;   //clear any prior interrupt       
	NVIC_PRI7_R = (NVIC_PRI7_R& ~(1<<21 | 1<<22 | 1<<23) ) + (1<<22);  // set priorty 2
	NVIC_EN0_R|=0x40000000;     //enable port f interrupt
	
	GPIO_PORTA_IM_R  |= 0x08;   // enable interrupt functionality on sw3
	GPIO_PORTA_IS_R  &= ~0x08;  // make A3 edge senstive        
	GPIO_PORTA_IBE_R |= 0x08;   //controlled by  GPIO_PORTF_IEV_R   
	GPIO_PORTA_IEV_R &= ~0x08;  //falling edge 
	GPIO_PORTA_ICR_R |= 0xFF;   //clear any prior interrupt       
	NVIC_PRI7_R = (NVIC_PRI7_R& ~(1<<21 | 1<<22 | 1<<23) ) + (1<<22);  // set priorty 2
	NVIC_EN0_R|=0x01;           //enable port A interrupt
	
	__enable_irq();	
}

char getSwitch(int n)
{
	if(n == 1)
				return GPIO_PORTF_DATA_R & 0X10 ;	
	else if(n == 2)
		return GPIO_PORTF_DATA_R & 0X01 ;	
	else
		return GPIO_PORTA_DATA_R & 0X08;
}

void GPIOF_Handler(void)  // press pause at first time
{	
	if(cooking == 1 )
	{
		paused = 1;
		led(rbg,off);
		
		delay_milli(500);
		while(1)
		{	
				if((getSwitch(2) & 0x01) != 0x01)  //press start at second time
				{
					led(rbg,on);
					break;
				}
				else if((getSwitch(1) & 0x10) != 0x10)  //press pause at second time
				{
					cooking = 0;
					second = 0;
					break;
				}
				else if( (NVIC_ST_CTRL_R & 0x10000) == 0x10000)
				{
					GPIO_PORTF_DATA_R ^= 0x0E;
					
					NVIC_ST_CTRL_R = 0;
					NVIC_ST_RELOAD_R = 1600000-1;
					NVIC_ST_CURRENT_R = 0; 
					NVIC_ST_CTRL_R = 0x05;
				}			
				//else if interrupt occur on a3 goto GPIOA_Handler
		}
		paused = 0;
	}	
	
	
	GPIO_PORTF_ICR_R |= 0X10;  // Acknowledge interrupt
}



void GPIOA_Handler(void) // press SW3 at first time
{	
	// disable interrupt on sw1
	GPIO_PORTF_IM_R  &= ~0x10;
	
	if(cooking == 1)
	{			
			delay_milli(500);
			while(1)
			{
				if( (getSwitch(3) & 0x08) != 0x08)  //press SW3 at second time
				{
						while(1)
						{
								if(getSwitch(2) != 0x01)
									break;
								else if(paused == 1)
									break;
								else if( (NVIC_ST_CTRL_R & 0x10000) == 0x10000)
								{
										GPIO_PORTF_DATA_R ^= 0x0E;
										
										NVIC_ST_CTRL_R = 0;
										NVIC_ST_RELOAD_R = 1600000-1;
										NVIC_ST_CURRENT_R = 0; 
										NVIC_ST_CTRL_R = 0x05;
								}
						}						
						GPIO_PORTF_ICR_R |= 0X10;  // Acknowledge interrupt	
						led(rbg,on);
						delay_milli(500);
						break;
				}
				else if( (NVIC_ST_CTRL_R & 0x10000) == 0x10000)
				{
						GPIO_PORTF_DATA_R ^= 0x0E;
						
						NVIC_ST_CTRL_R = 0;
						NVIC_ST_RELOAD_R = 1600000-1;
						NVIC_ST_CURRENT_R = 0; 
						NVIC_ST_CTRL_R = 0x05;
				}
			}
	}
	
	GPIO_PORTA_ICR_R |= 0X08;  // Acknowledge interrupt
	
	// enable interrupt on sw1
	GPIO_PORTF_IM_R  |= 0x10;
}