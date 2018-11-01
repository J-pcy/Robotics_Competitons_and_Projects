//#include "5110.h"
#ifndef __5110_H_ 
#define __5110_H_
#include "derivative.h"
void LCD_init(void);
void LCD_clear(void);
void delay_1us(void); 
void LCD_set_XY(unsigned char x,unsigned char y);
void LCD_write_char(unsigned char c);
void LCD_write_char2(unsigned char X, unsigned char Y,unsigned char c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
//void LCD_write_chinese_string(unsigned char X, unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row);
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,unsigned char Pix_x,unsigned char Pix_y);
void LCD_write_byte(unsigned char dat, unsigned char command);
void LCD_Show_Number (unsigned char X,unsigned char Y,unsigned int number); 

/********************************************************************************************************************/
#endif 