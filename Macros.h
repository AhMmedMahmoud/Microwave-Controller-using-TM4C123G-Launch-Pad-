#ifndef Macros_INCLUDE_FILE
#define Macros_INCLUDE_FILE

/* on and off */
#define on      1
#define off     0

/* clear and notClear */
#define clear      1
#define notClear     0

/* symbolic names for LEDS */
#define red     0x02
#define blue    0x04
#define green   0x08
#define rbg     0x0E

/* symbolic names for LCD commands */
#define clear_display              0x01 
#define returnHome                 0x02 
#define cursorBlink                0x0F 
#define cursorOff                  0x0C 
#define cursorOn                   0x0E 
#define mode_8bit					         0x38          
#define FirstRowPosition0          0x80
#define FirstRowPosition1          0x81
#define FirstRowPosition2          0x82  // and so on
#define SecondRowPosition0         0xC0
#define SecondRowPosition1         0xC1
#define SecondRowPosition2         0xC2  // and so on
#define moveCursorRight            0x06

#endif