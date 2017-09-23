# Project LCD_Rewrite

This project is my attempt to rewrite entire Arduino LCD library in C language.

This project serves two purposes:
  1. As a platform to enhance my poor C programming.
  2. To allow portability with minor changes when used on different microcontrollers that only support C.
  
The library is based on HD44780U datasheet. The library is still under development(buggy and exploitable atm). Current version only works in 8-bit mode. I will be porting 4-bit mode in the same source files. For now, I am just stuffing more functions for the user. The documentation of the library will be updated from time to time.
