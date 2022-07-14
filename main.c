#include "Microwave.h"
#include "LCD.h"
#include "delay.h"
#include "Macros.h"

/* main */
int main ()
{
		char read;
		int mins,secs;	
	
		/* setup */
		setup();
		
		/* infinite loop */
		while(1)
		{
				//selectMode
				selectMode(&read);	
				
			
				if(read == 'A')
				{		  
						//show pop corn for 2s
					  LCD_writeString(clear,"Pop Corn",notClear);
						delay_milli(2000);
						LCD_Cmd(clear_display);
						//cooking
						startCooking(1,0);								
						//finish cooking
						finishCooking();	
				}
				else if(read == 'B' || read == 'C')
				{		  					
						// get weight from user
						if(read == 'B')
							getWeight("Beef",&mins,&secs);
						else
							getWeight("Chicken",&mins,&secs);
						//cooking
						startCooking(mins,secs);									
						//finish cooking
						finishCooking();	
				}
				else if(read == 'D')
				{		  				
						// get time from user
						getTime(&mins,&secs);							
						//cooking
						startCooking(mins,secs);								
						//finish cooking
						finishCooking();	
				}
		}
}

