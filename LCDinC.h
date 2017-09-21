//LCDinC.h

#ifndef LCDinC_h
#define LCDinC_h

#include "Arduino.h"

#define RS  12
#define EN  10
#define D0  9
#define D1  8
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7

void LCD_Cinit(void);
void send_cmd(uint8_t wCmd);
void send_pulse(uint8_t instruc); //0 or 1; 0 for control command, 1 for data command
void lcd_write(uint8_t *wChar);

#endif
