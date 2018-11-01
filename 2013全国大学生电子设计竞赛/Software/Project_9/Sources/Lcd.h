#ifndef __LCD_INIT_H__
#define __LCD_INIT_H__

#include <MC9S12XS128.h>
#include "derivative.h"

//位操作定义
#define LCD_CE   PORTA_PA1
#define LCD_RST  PORTA_PA0
#define LCD_DC   PORTA_PA2
#define SDIN     PORTA_PA3
#define SCLK     PORTA_PA4

void LCD_Init(void);
void LCD_clear(void);
void LCD_write_byte(uchar, uchar);
void LCD_set_XY(uchar , uchar );      
void LCD_write_char(uchar );
void LCD_Write_Char(uchar ,uchar ,uchar);
void LCD_Write_Num(uchar ,uchar ,uint,uchar);
void LCD_write_english_string(uchar ,uchar ,char *);
void LCD_Write_String(uchar x, uchar y,char *string); 

#endif