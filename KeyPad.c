#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "Delay.h"
#include "KeyPad.h"


/*  @Connentions
                 [PE1 - PE4] -> [R1 - R4]  Raws
                 [PC4 - PC7] -> [C1 - C4]  Cols
*/

                                                                    
unsigned const char symbol[4][4] = {{ '1', '2',  '3', 'A'},      
																		{ '4', '5',  '6', 'B'},      
																		{ '7', '8',  '9', 'C'},      
																		{ '*', '0',  '#', 'D'}
																	 }; 

void keypad_init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x14;            //enable clc for port C & D  
	
	GPIO_PORTC_DEN_R |= 0xF0;             //digital enable pins in port C
  GPIO_PORTE_DEN_R |= 0x1E;             //digital enable pins in port E
	
  GPIO_PORTC_DIR_R |= 0xF0;             //set directions cols are o/ps
  GPIO_PORTE_DIR_R |= 0x00;             //set directions raws are i/ps
	
  GPIO_PORTE_PDR_R |= 0x1E;             //pull down resistor on Raws
}

char keypad_getkey(void)
{
	int i,j;
  while(1)
  {
    for(i = 0; i < 4; i++)                        //columns traverse
    {
      GPIO_PORTC_DATA_R = (1U << (i+4));         // 0001 000         0010 0000      0100 0000    1000 0000
      delay_micro(2);
      for(j = 0; j < 4; j++)                     //raws traverse
      {
        if((GPIO_PORTE_DATA_R & 0x1E) & (1U << (j+1)))     // 0000 0010        0000 0100    0000 1000     0001 0000
          return symbol[j][i];
      }
    }
  }
}