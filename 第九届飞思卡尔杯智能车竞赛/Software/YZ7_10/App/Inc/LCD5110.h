#ifndef _LCD5110_H_
#define _LCD5110_H_

#define  LCD_RST  PTC11
#define  LCD_DC   PTC8//
#define  LCD_CE  PTC9//
#define  LCD_DIN  PTC7//
#define  LCD_SCLK PTC6//
#define  LCD_BL   PTC5  //
void LCD_write_byte(uint8 dat,uint8 command);//写数据或命令 command=0命令  command=1数据
void LCD_clear(void);     //5110清屏
void LCD_init(void);      //LCD_5110初始化
void LCD_set_XY(uint8 X, uint8 Y);
void LCD_write_char(uint8 c);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s); 



#endif