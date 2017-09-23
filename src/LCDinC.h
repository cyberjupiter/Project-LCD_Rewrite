//LCDinC.h

#ifndef LCDinC_h
#define LCDinC_h

#include "Arduino.h"

/* user available commands */
void lcd_setPins(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
void lcd_init(void);
void lcd_write(uint8_t *pChar);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_clear(void);


/* low level commands */
void send_cmd(uint8_t pCmd);
void send_data(uint8_t pData);
void send_pulse(void);

#endif
