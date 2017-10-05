//LCDinC.c

#include "LCDinC.h"

enum{
	RS,
	RW,
	EN
};

uint8_t pins_control[3];
uint8_t pins_data[8];

void lcd_set8bit(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	//instrucion register pins
	pins_control[RS] = rs; 
	pins_control[RW] = rw;
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
	for (uint8_t i = 0; i < 3; i++)
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
	while(read_busyflag() == 1);
	
	digitalWrite(pins_control[RS], 0);
	digitalWrite(pins_control[RW], 0);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pCmd >> i) & 0b00000001);
	}
	send_pulse();
}

void send_data(uint8_t pData)
{
	while(read_busyflag() == 1);
	
	digitalWrite(pins_control[RS], 1);
	digitalWrite(pins_control[RW], 0);
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (pData >> i) & 0b00000001);
	}
	send_pulse();
}

void send_pulse(void)
{
	digitalWrite(pins_control[EN], 1);
	delay(1);
	digitalWrite(pins_control[EN], 0);
	delay(1);
}

uint8_t read_busyflag(void)
{
	uint8_t busy_flag;
	
	pinMode(pins_control[7], 0);
	digitalWrite(pins_control[RS], 0);
	digitalWrite(pins_control[RW], 1);
	
	busy_flag = digitalRead(pins_control[7]);
	
	return busy_flag;
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
	
	else if (row == 3)
	{
		send_cmd(ROW3 + col);
	}
	
	else
	{
		return 1;
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

void entryMode(uint8_t *position, uint8_t *shift_flag)
{
	if (position == "left" && shift_flag == "off") //decrement address, shift off. write each char to left
	{
		send_cmd(ENTRYLEFT);
	}
	
	else if(position == "left" && shift_flag == "on") //increment address, shift on. write each char to left by shifting
	{
		send_cmd(ENTRYLEFTON);
	}
	
	else if(position == "right" && shift_flag == "off") //increment address, shift off. write each char to right, normal method
	{
		send_cmd(ENTRYRIGHT);
	}
	
	else if(position == "right" && shift_flag == "on") //not working, will fix
	{
		send_cmd(ENTRYRIGHTON);
	}
	
	else
	{
		lcd_write("INVALID CODE");
	}
}
	

void shift_allLeft(void)
{
	send_cmd(SHIFTALLLEFT);
}

void shift_allRight(void)
{
	send_cmd(SHIFTALLRIGHT);
}
