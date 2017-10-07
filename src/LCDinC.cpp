/* LCDinC.c
*
* Author : cyberjupiter@AimanMazlan 
* Github : https://github.com/cyberjupiter/Project-LCD_Rewrite
* Rev    : 2.0.0 
* Controller : Arduino UNO (other Arduino should be fine if the pins are set correctly)
*
* This library is based on HD44780U driver. The datasheet is available on the internet.
* Visit my github for the latest revision, bugs and issues report, suggestion for improvement, etc.
*/

#include "LCDinC.h"

/* enumeration for RS,RW,EN pins for easy lookup */
enum{
	RS,
	RW,
	EN
};

uint8_t pins_control[3];
uint8_t pins_data[8];
uint8_t bitMode_flag; //global variables to handle the mode of LCD (8bit/4bit)

void lcd_set8Bit(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
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
	
	bitMode_flag = 0;
}

void lcd_set4Bit(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	pins_control[RS] = rs; 
	pins_control[RW] = rw;
	pins_control[EN] = en;
	
	pins_data[0] = d4;
	pins_data[1] = d5;
	pins_data[2] = d6;
	pins_data[3] = d7;
	
	bitMode_flag = 1;
} 

void lcd_init(void)
{
	/* in the datasheet page 45, we must wait for more than 40ms after the VCC rises to 2.7V before sending commands */
	delay(50); 
	
	if (bitMode_flag == 1)
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			pinMode(pins_control[i], 1);
			digitalWrite(pins_control[i], 0);
		}

		for (uint8_t i = 0; i < 4; i++)
		{
			pinMode(pins_data[i], 1);
			digitalWrite(pins_data[i], 0);
		}
		
		/* initialization by instruction as in page 45-46. we send the same nibble for 3 times. */
		//first go
		write4Bits(INIT_4BIT); 
		delay(5);
		//second go
		write4Bits(INIT_4BIT);
		delay(1);
		//last go
		write4Bits(INIT_4BIT);
		delay(1);
		/* we need to send another nibble that tells the LCD to be set as 4-bit interface. All the instruction after this nibble requires two write since we need to send a byte(1 byte = 2 nibble) */
		write4Bits(0b0010);
		
		//function set, init LCD 2 lines, 5x8
		send_cmd(0b00101000);
		
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
	else
	{
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
		write8Bits(INIT);
		delay(5);
		
		write8Bits(INIT);
		delay(1);
		
		write8Bits(INIT);
		delay(1);

		//function set, init LCD 2 lines, 5x8
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
}

void write8Bits(uint8_t value)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(pins_data[i], (value >> i) & 0b00000001);
	}
	send_pulse();
}

void write4Bits(uint8_t value)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		digitalWrite(pins_data[i], (value >> i) & 0b00000001);
	}
	send_pulse();
}

void send_cmd(uint8_t pCmd)
{
	while(read_busyflag() == 1);
	
	digitalWrite(pins_control[RS], 0);
	digitalWrite(pins_control[RW], 0);
	
	if (bitMode_flag == 0)
	{
		write8Bits(pCmd);
	}
	
	else
	{
		write4Bits(pCmd>>4);
		write4Bits(pCmd);
	} 
}

void send_data(uint8_t pData)
{
	while(read_busyflag() == 1);
	
	digitalWrite(pins_control[RS], 1);
	digitalWrite(pins_control[RW], 0);
	
	if (bitMode_flag == 0)
	{
		write8Bits(pData);
	}
	else
	{
		write4Bits(pData>>4);
		write4Bits(pData);
	}
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
		lcd_write("ERROR");
		while(1);
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
		lcd_write("ERROR");
		while(1);
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
