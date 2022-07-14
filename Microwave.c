#include "Microwave.h"
#include "RGB.h"
#include "Delay.h"
#include "KeyPad.h"
#include "LCD.h"
#include "Buzzer.h"
#include "Switches.h"
#include "SysTick.h"
#include "Macros.h"

/* global variables */
char* seconds[60] = {  "00","01","02","03","04","05","06","07","08","09","10"
											,"11","12","13","14","15","16","17","18","19","20"
											,"21","22","23","24","25","26","27","28","29","30"
											,"31","32","33","34","35","36","37","38","39","40"
											,"41","42","43","44","45","46","47","48","49","50"
											,"51","52","53","54","55","56","57","58","59"		
										};
char* minutes[31] = {  "00:","01:","02:","03:","04:","05:","06:","07:","08:","09:","10:"
											,"11:","12:","13:","14:","15:","16:","17:","18:","19:","20:"
											,"21:","22:","23:","24:","25:","26:","27:","28:","29:","30:"
										};
char cooking = 0;
int  second  = 1;										
										


/* microwave functions */
void setup()
{
	RGB_init();
	keypad_init();
	LCD_init();
	Buzzer_init();
	Switches_init();
}

void selectMode(char *read)
{
		led(rbg,off);
		second = 1;
		LCD_writeString(clear,"select a choice",notClear);	
		while((*read = keypad_getkey() ) == 0){}
		delay_milli(280);
}


void getTime(int* mins,int* secs)
{
		char m2,m1,s2,s1;
		while(1)
		{
					LCD_writeString(clear,"Enter Time",notClear);
					
					while((m2 = keypad_getkey() ) == 0){}	
					delay_milli(280);	
					LCD_writeString(clear,"00:0",notClear);
					LCD_writeLetter(notClear,m2,notClear,0);
					if( (m2-'0' > 3 && m2-'0' <= 9) || m2 == 'A' || m2 == 'B' || m2 == 'C' ||m2 == 'D' || m2 == '#' || m2 == '*')
					{
						LCD_writeString(clear,"ERR",notClear);
						delay_milli(2000);
						continue;
					}
					
					
					while((m1 = keypad_getkey() ) == 0){}		
					delay_milli(280);		
					LCD_writeString(clear,"00:",notClear);
					LCD_writeLetter(notClear,m2,notClear,0);
					LCD_writeLetter(notClear,m1,notClear,0);
					if(m1 == 'A' || m1 == 'B' || m1 == 'C' || m1 == 'D' || m1 == '#' || m1 == '*')
					{
						LCD_writeString(clear,"ERR",notClear);
						delay_milli(2000);
						continue;
					}	
					
					
					*mins = (m2-'0')*10+(m1-'0');		
					

					
					while((s2 = keypad_getkey() ) == 0){}
					delay_milli(280);				
					LCD_writeString(clear,"0",notClear);
					LCD_writeLetter(notClear,m2,notClear,0);
					LCD_writeLetter(notClear,':',notClear,0);
					LCD_writeLetter(notClear,m1,notClear,0);
					LCD_writeLetter(notClear,s2,notClear,0);
					if((s2-'0' > 6 && s2-'0' <= 9) || s2 == 'A' || s2 == 'B' || s2 == 'C' || s2 == 'D' || s2 == '#' || s2 == '*')
					{
						LCD_writeString(clear,"ERR",notClear);
						delay_milli(2000);
						continue;
					}	
					
					
					while((s1 = keypad_getkey() ) == 0){}
					delay_milli(280);				
					LCD_writeLetter(clear,m2,notClear,0);
					LCD_writeLetter(notClear,m1,notClear,0);
					LCD_writeLetter(notClear,':',notClear,0);
					LCD_writeLetter(notClear,s2,notClear,0);
					LCD_writeLetter(notClear,s1,notClear,0);
					if(s1 == 'A' || s1 == 'B' || s1 == 'C' || s1 == 'D' || s1 == '#' || s1 == '*')
					{
						LCD_writeString(clear,"ERR",notClear);
						delay_milli(2000);
						continue;
					}	
					
					*secs = (s2-'0')*10+(s1-'0');
					break;
		}	
}

void getWeight(char* food,int* mins,int* secs)
{
		char m2;
		while(1)
		{			
					LCD_writeString(clear,food,notClear);
					LCD_writeString(notClear," weight?",notClear);
			
					while((m2 = keypad_getkey() ) == 0){}	
					delay_milli(280);
					if( m2 == '0' || m2 == 'A' || m2 == 'B' || m2 == 'C' ||m2 == 'D' || m2 == '#' || m2 == '*')
					{
						LCD_writeString(clear,"ERR",notClear);
						delay_milli(1500);
						continue;
					}
					LCD_writeLetter(clear,m2,notClear,1);
					LCD_writeString(notClear," kilo",notClear);
					delay_milli(2000);
					
					if(food[0] == 'B')
					{
						*mins = (int)( (m2-'0')*0.5 );	
						*secs = ( ((m2-'0')*0.5)-*mins )*60;
					}
					else
					{
						*mins = (int)( (m2-'0')*0.2 );	
						*secs = ( ((m2-'0')*0.2)-*mins )*60;
					}
					break;
		}	
}

void startCooking(int mins,int secs)
{
	int i ,j;
	cooking = 1;
	led(rbg,on);
	for(j = mins; j >= 0 ; j--)
	{					
		for(i = secs; i >= 0 ;i--)
		{	
			if(cooking == 1)
				LCD_writeString(clear,minutes[j],notClear);	
			if(cooking == 1)
				LCD_writeString(notClear,seconds[i],notClear);	
			if(cooking == 1)
				SysTicK_countSeconds(second);
		}
		secs = 59;
	}	
}

void finishCooking()
{
	int i;
	if(cooking == 1)
	{
			for(i= 0 ; i < 3 ; i++)
			{
				led(rbg,on);
				delay_milli(200);
				led(rbg,off);
				delay_milli(200);
			}
			Buzzer(on);
			SysTicK_countSeconds(3);
			Buzzer(off);
	}
}