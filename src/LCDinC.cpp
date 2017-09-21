//LCD_c.cpp

#include "LCD_C.h"

uint8_t pins_control[2] = {RS, EN};
uint8_t pins_data[8] = {D0, D1, D2, D3, D4, D5, D6, D7};


void LCD_Cinit(void)
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
  
  //init LCD 2 lines, 5x7
  send_cmd(0b00111000);
  //send pulse
  send_pulse(0);

  delay(300);

  //display on, cursor on
  send_cmd(0b00001110); 
  //send pulse
  send_pulse(0);
  delay(30);
  
  //clear LCD
  send_cmd(0b00000001); 
  //send pulse
  send_pulse(0);
  delay(30);
  
  //shift cursor right
  send_cmd(0b00000110);
  //send pulse
  send_pulse(0);
  delay(30);
  
  //line 1, position 6
  send_cmd(0b10000110);
  //send pulse
  send_pulse(0);
  delay(30);
  
  //write 'AIMAN, INIT OK'
  lcd_write("AIMAN, INIT OK");
}

void send_cmd(uint8_t wCmd)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(pins_data[i], (wCmd >> i) & 0b0000001);
  }
}

void send_pulse(uint8_t wInstruc)
{
  if (wInstruc == 0)
  {
    digitalWrite(RS, 0);
    digitalWrite(EN, 1);
    delay(10);
    digitalWrite(EN, 0);
  }
  
  else
  {
    digitalWrite(RS, 1);
    digitalWrite(EN, 1);
    delay(10);
    digitalWrite(EN, 0);
  }
}

void lcd_write(uint8_t *wChar)
{
  for(uint8_t i = 0; wChar[i] != '\0'; i++)
    {
      send_cmd(wChar[i]);
      send_pulse(1);
    }
}


