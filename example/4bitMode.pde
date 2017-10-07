//an example of using 8-bit mode to write Hello World!

#include <LCDinC.h>

void setup() 
{        //change the pins accroding to your circuit
         //(RS, RW, EN, D4, D5, D6, D7)
lcd_set4Bit(12, 11, 10, 4, 5, 6, 7);  
lcd_init();
lcd_write("Hello World!");
}
  
void loop() 
{
}
