//LCDinC.h

#ifndef LCDinC_h
#define LCDinC_h

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

#include "Arduino.h"

void lcd_init(void);
void send_cmd(uint8_t wCmd);
void send_data(uint8_t wData);
void send_pulse(void);
void lcd_write(uint8_t *wChar);

#endif
