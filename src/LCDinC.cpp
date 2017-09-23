//LCDinC.cpp

#include "LCDinC.h"

enum{
	RS,
	EN
};

uint8_t pins_control[2];
uint8_t pins_data[8];

void lcd_setPins(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
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

	delay(500);

	//init LCD 2 lines, 5x8
	send_cmd(0b00111000);
	delay(300);

	//display on, cursor on
	send_cmd(0b00001110); 
	delay(30);

	//clear LCD
	send_cmd(0b00000001); 
	delay(30);

	//shift cursor right
	send_cmd(0b00000110);
	delay(30);

	//line 1, position 6
	send_cmd(0b10000110);
	delay(30);

	//write  "INIT OK"
	lcd_write("INIT OK");
}

void send_cmd(uint8_t pCmd)
{
	digitalWrite(pins_control[RS], 0);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pCmd >> i) & 0b0000001);
	}
	send_pulse();
}

void send_data(uint8_t pData)
{
	digitalWrite(pins_control[RS], 1);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pData >> i) & 0b0000001);
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
		send_cmd(0b10000000 + col);
	}
	
	else if (row == 1)
	{
		send_cmd(0b11000000 + col);
	}
	else if (row == 2)
	{
		send_cmd(0b10010100 + col);
	}
	
	else
	{
		send_cmd(0b11010100 + col);
	}
} 

void lcd_clear(void)
{
	send_cmd(0b00000001); 
}
