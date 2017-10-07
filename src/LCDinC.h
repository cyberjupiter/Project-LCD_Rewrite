/* LCDinC.h
*
* Author : cyberjupiter@AimanMazlan 
* Github : https://github.com/cyberjupiter/Project-LCD_Rewrite
* Rev    : 2.0.0 
* Controller : Arduino UNO (other Arduino should be fine if the pins are set correctly)
*
* This library is based on HD44780U driver. The datasheet is available on the internet.
* Visit my github for the latest revision, bugs and issues report, suggestion for improvement, etc.
*/

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LCDinC_h
#define LCDinC_h

#define INIT          0b00110000
#define INIT_4BIT	  0b0011
#define CLEAR         0b00000001

#define ROW0          0b10000000
#define ROW1          0b11000000
#define ROW2          0b10010100
#define ROW3          0b11010100

#define DISPLAYON     0b00001100
#define DISPLAYOFF    0b00001000
#define CURSORON      0b00001110
#define CURSOROFF     0b00001100
#define BLINKON       0b00001111
#define BLINKOFF      0b00001110

#define ENTRYLEFT     0b00000100
#define ENTRYRIGHT    0b00000110
#define ENTRYLEFTON   0b00000111
#define ENTRYRIGHTON  0b00000101

#define SHIFTALLLEFT  0b00011000
#define SHIFTALLRIGHT 0b00011000

	/* user available commands */
	void lcd_set8Bit(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
	void lcd_set4Bit(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7); 
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
	void entryMode(uint8_t *position, uint8_t *shift_flag);
	void shift_allLeft(void);
	void shift_allRight(void); 

	/* mid level commands */
	void send_cmd(uint8_t pCmd);
	void send_data(uint8_t pData);

	/* low level commands */
	void write8Bits(uint8_t value);
	void write4Bits(uint8_t value);
	void send_pulse(void);
	uint8_t read_busyflag(void);

#endif

#ifdef __cplusplus
} // extern "C"
#endif
