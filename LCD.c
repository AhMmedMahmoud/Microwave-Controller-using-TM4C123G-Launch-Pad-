#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "Delay.h"
#include "LCD.h"
#include "Macros.h"

/*  @Connentions
	 	    VSS  ->  GND
		    VDD  ->  VBUS
	       	    VO   ->  V_POTENTIONMETER_10KOHM
	   	    RS   ->   PA6
	       	    RW   ->   GND
		    E    ->   PA7
		    D0   ->   PB0
		    D1   ->   PB1
		    D2   ->   PB2
		    D3   ->   PB3
		    D4   ->   PB4
		    D5   ->   PB5
		    D6   ->   PB6
	   	    D7   ->   PB7
		    A    ->   VBUS
	            K    ->   GND
*/


/* initialization of LCD */
void LCD_init(void)
{
	//step1 enable clock of ports we will use and waiting util execution
	SYSCTL_RCGCGPIO_R |= 0x03; 				// enable clcok for PORTA , PORTB 
	while( (SYSCTL_PRGPIO_R & 0X03) != 0X03);		// waiting until execution of enabling clock

	//step2 configure the pins we will use
	GPIO_PORTA_DIR_R |=0xC0; 	                 // configure A6,A7  as output   (RS,E)
	GPIO_PORTA_DEN_R |=0xC0;			 // configure A6,A7  as digital  (RS,E)
	GPIO_PORTB_DIR_R |=0xFF; 			 // configure B0,B1,B2,B3,B4,B5,B6,B7  as output     (D0,D1,D2,D3,D4,D5,D6,D7) 
	GPIO_PORTB_DEN_R |=0xFF; 			 // configure B0,B1,B2,B3,B4,B5,B6,B7  as digital    (D0,D1,D2,D3,D4,D5,D6,D7) 
	
	//step3 select mode,remove any old value displayed on screen
	LCD_Cmd(mode_8bit);            //select 8-bits mode of LCD                    (must after configure lcd pins)
	LCD_Cmd(moveCursorRight);      //direction of cursor to right                 (must)   (without it sometimes doenot work)
	LCD_Cmd(clear_display);        //clear whatever is written on display         (optional)
	LCD_Cmd(cursorBlink);          //turn on display mode and cursor blinking     (must)   (without it sometimes doenot work)
}

/* select command of LCD */
void LCD_Cmd(unsigned char command)
{
	GPIO_PORTA_DATA_R = GPIO_PORTA_DATA_R & 0x3F;    //  PA7 PA6 = 00     --> E=0,RS=0       ACTIVATE COMMAND MODE
	GPIO_PORTB_DATA_R =command;                      //  make B0-B7 carry certain command equivalent value
	GPIO_PORTA_DATA_R |= 0x80; 	                 //  PA7 PA6 = 10     --> E=1,RS=0       upping E  
	delay_micro(1);                                  //  waiting
	GPIO_PORTA_DATA_R = GPIO_PORTA_DATA_R & 0x3F;    //  PA7 PA6 = 00     --> E=0,RS=0       droping E to pass values on D0-D7 inside LCD
	
	if(command <4) 
		delay_milli(2);
	else 
		delay_micro(37); 
}



void LCD_writeLetter(char clearBeforeWrite,char str,char clearAfterWrite,char homePosition)    //Function to write on LCD
{
	if(clearBeforeWrite)
		LCD_Cmd(clear_display);       //clear whatever is written on display 
	
	if(homePosition)
		LCD_Cmd(FirstRowPosition0);   //Force cusor to begining of first row  
	delay_milli(50);
	
	GPIO_PORTA_DATA_R = (GPIO_PORTA_DATA_R & 0x3F) + 0x40; 	  //  PA7 PA6 = 01 -->  E=0,RS=1        ACTIVATE DATA MODE
	GPIO_PORTB_DATA_R =str;   				  //  make B0-B7 carry the character
	GPIO_PORTA_DATA_R |= 0x80;                                //  PA7 PA6 = 11 -->  E=1,RS=1        upping E 
	delay_micro(1);																						//  waiting
	GPIO_PORTA_DATA_R = GPIO_PORTA_DATA_R & 0x3F;             //  PA7 PA6 = 00 -->  E=0,RS=0        droping E to pass values on D0-D7 inside LCD
	delay_milli(1);		 
	
	delay_milli(50);
	
	if(clearAfterWrite == 1)
		LCD_Cmd(clear_display);
}


void LCD_writeString (char clearBeforeWrite,char *str,char clearAfterWrite)    //Function to write on LCD
{
	int i;
	if(clearBeforeWrite)
		LCD_Cmd(clear_display);       //clear whatever is written on display 
	
	for(i=0;str[i]!=0;i++)    // Send each char of string till the NULL
	{
		GPIO_PORTA_DATA_R = (GPIO_PORTA_DATA_R & 0x3F) + 0x40; 	  //  PA7 PA6 = 01 -->  E=0,RS=1        ACTIVATE DATA MODE
		GPIO_PORTB_DATA_R =str[i];   				  //  make B0-B7 carry the character
		GPIO_PORTA_DATA_R |= 0x80;                                //  PA7 PA6 = 11 -->  E=1,RS=1        upping E 
		delay_micro(1);																						//  waiting
		GPIO_PORTA_DATA_R = GPIO_PORTA_DATA_R & 0x3F;             //  PA7 PA6 = 00 -->  E=0,RS=0        droping E to pass values on D0-D7 inside LCD
		delay_milli(1);
	}
	
	if(clearAfterWrite == 1)
		LCD_Cmd(clear_display);
}
