#ifndef _LQOLED_H
#define _LQOLED_H
#include "derivative.h"
 extern byte longqiu96x64[768];
 void LCD_Init(void); //液晶初始化
 void LCD_CLS(void);//清屏
 void LCD_P6x8Str(byte x,byte y,byte ch[]);//输出ASCII码 x为0-128 y为0~7 6*8
 void LCD_P8x16Str(byte x,byte y,byte ch[]);//输出ASCII码 x为0-128 y为0~7 16*8
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
 void LCD_Fill(byte dat);
#endif

