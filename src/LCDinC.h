//LCDinC.h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LCDinC_h
#define LCDinC_h

#define DISPLAYON   0b00001100
#define DISPLAYOFF  0b00001000
#define CURSORON    0b00001110
#define CURSOROFF   0b00001100
#define BLINKON     0b00001111
#define BLINKOFF    0b00001110
  
#include "Arduino.h"

/* user available commands */
void lcd_setPins(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
void lcd_init(void);
void lcd_write(uint8_t *pChar);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_clear(void);
void lcd_displayOn(void);
void lcd_displayOff(void);
void lcd_cursorOn(void);
void lcd_cursorOff(void);
void lcd_blinkOn(void);
void lcd_blinkOff(void);

/* low level commands */
void send_cmd(uint8_t pCmd);
void send_data(uint8_t pData);
void send_pulse(void);

#endif

#ifdef __cplusplus
} // extern "C"
#endif
