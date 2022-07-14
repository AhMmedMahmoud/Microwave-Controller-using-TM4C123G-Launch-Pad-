#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

/* initialization of LCD */
void LCD_init(void);

/* select command of LCD */
void LCD_Cmd(unsigned char command);

/* send String to LCD */
void LCD_writeString (char clearBeforeWrite,char *str,char clearAfterWrite);

/* clear then write letter on LCD */
void LCD_writeLetter(char clearBeforeWrite,char str,char clearAfterWrite,char homePosition);


#endif 