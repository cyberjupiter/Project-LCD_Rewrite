//LCDinC.c

#include "LCDinC.h"

enum{
	RS,
	EN
};

uint8_t pins_control[2];
uint8_t pins_data[8];

void lcd_set8bit(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	//instrucion register pins
	pins_control[RS] = rs; 
	pins_control[EN] = en;
	
	//data register pins
	pins_data[0] = d0;
	pins_data[1] = d1;
	pins_data[2] = d2;
	pins_data[3] = d3;
	pins_data[4] = d4;
	pins_data[5] = d5;
	pins_data[6] = d6;
	pins_data[7] = d7;
}

void lcd_init(void)
{
	delay(50);
	for (uint8_t i = 0; i < 2; i++)
	{
		pinMode(pins_control[i], 1);
		digitalWrite(pins_control[i], 0);
	}

	for (uint8_t i = 0; i < 8; i++)
	{
		pinMode(pins_data[i], 1);
		digitalWrite(pins_data[i], 0);
	}

	//initialization by instruction as in datasheet. also known as wake-up call
	send_cmd(INIT);
	delay(5);
	
	send_cmd(INIT);
	delay(1);
	
	send_cmd(INIT);
	delay(1);

	//init LCD 2 lines, 5x8
	send_cmd(0b00111000);
	delay(5);

	//display on, cursor off
	send_cmd(0b00001100); 

	//shift cursor right
	send_cmd(0b00000110);

	//line 1, position 1
	send_cmd(0b10000000);
	
	//write  "INIT OK"
	lcd_write("INIT OK");
	delay(500);
	lcd_clear();
}

void send_cmd(uint8_t pCmd)
{
	digitalWrite(pins_control[RS], 0);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pCmd >> i) & 0b00000001);
	}
	send_pulse();
}

void send_data(uint8_t pData)
{
	digitalWrite(pins_control[RS], 1);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pData >> i) & 0b00000001);
	}
	send_pulse();
}

void send_pulse(void)
{
	digitalWrite(pins_control[EN], 1);
	delay(10);
	digitalWrite(pins_control[EN], 0);
}

void lcd_write(uint8_t *pChar)
{
	for(uint8_t i = 0; pChar[i] != '\0'; i++)
	{
		send_data(pChar[i]);
	}
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
	if (row == 0)
	{
		send_cmd(ROW0 + col);
	}
	
	else if (row == 1)
	{
		send_cmd(ROW1 + col);
	}
	
	else if (row == 2)
	{
		send_cmd(ROW2 + col);
	}
	
	else
	{
		send_cmd(ROW3 + col);
	}
} 

void lcd_clear(void)
{
	send_cmd(CLEAR); 
}

void lcd_displayOn(void)
{
	send_cmd(DISPLAYON);
}

void lcd_displayOff(void)
{
	send_cmd(DISPLAYOFF);
}

void lcd_cursorOn(void)
{
	send_cmd(CURSORON);
}

void lcd_cursorOff(void)
{
	send_cmd(CURSOROFF);
}

void lcd_blinkOn(void)
{
	send_cmd(BLINKON);
}

void lcd_blinkOff(void)
{
	send_cmd(BLINKOFF);
}
