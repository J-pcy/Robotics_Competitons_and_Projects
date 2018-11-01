#include "derivative.h"
#include "5110.h" 
unsigned char potemp1=0,potemp2=0,potemp3=0,potemp4=0,potemp5=0,potemp6=0,pox=1,poy=0;

unsigned char font6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};
/***************************************************************************/
/*****常用操作**************************************************************/
/* enable global interrupts */
#define GIE      	        	   (SREG |= BIT(7))

/* disable global interrupts */
#define GID       	         	   (SREG &= ~BIT(7))
//#define SLEEP()	asm("sleep")
/* enables an unsigned char to be used as a series of booleans */
#define BIT(x)	           (1 << (x))
#define SETBIT(x, y)   	   (x |= y)
#define CLEARBIT(x, y) 	   (x &= ~y)
#define CHECKBIT(x, y) 	   (x & y)

// ***** Define I/O pins *****
#define BIT7 0x80
#define BIT6 0x40
#define BIT5 0x20
#define BIT4 0x10
#define BIT3 0x08
#define BIT2 0x04
#define BIT1 0x02
#define BIT0 0x01


#define true 1
#define True 1

#define false 0
#define False 0


#define SCLK    BIT4
#define SDIN    BIT3
#define LCD_DC  BIT2
#define LCD_CE  BIT1
#define LCD_RST BIT0

#define LCD_PORT	PORTA
#define LCD_DIR		DDRA

const tabpoint[9]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
/************************************************************************/
void LCD_init(void)
{
//先设置为输出
SETBIT(LCD_DIR,SCLK);
SETBIT(LCD_DIR,SDIN);
SETBIT(LCD_DIR,LCD_DC);
SETBIT(LCD_DIR,LCD_CE);
SETBIT(LCD_DIR,LCD_RST);
// 产生一个让LCD复位的低电平脉冲
CLEARBIT(LCD_PORT,LCD_RST);//LCD_RST = 0;
delay_1us();
SETBIT(LCD_PORT,LCD_RST);//LCD_RST = 1; 
// 关闭LCD
CLEARBIT(LCD_PORT,LCD_CE);//LCD_CE = 0;
delay_1us();	
	// 使能LCD
SETBIT(LCD_PORT,LCD_CE);//LCD_CE = 1;
delay_1us();
 LCD_write_byte(0x21, 0);	// 使用扩展命令设置LCD模式
 LCD_write_byte(0xc8, 0);	// 设置偏置电压
 LCD_write_byte(0x06, 0);	// 温度校正
 LCD_write_byte(0x13, 0);	// 1:48
 LCD_write_byte(0x20, 0);	// 使用基本命令
 LCD_clear();	            // 清屏
 LCD_write_byte(0x0c, 0);	// 设定显示模式，正常显示
 LCD_set_XY(0,1);    
 // 关闭LCD
CLEARBIT(LCD_PORT,LCD_CE);	
}
/********************************************************************************************************************/
void LCD_clear(void)
{
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<500; i++)
	{
        LCD_write_byte(0, 1);
	}			
}
/********************************************************************************************************************/
void delay_1us(void)                 //1us延时函数
{
   unsigned int i;
   for(i=0;i<5;i++);
}
/********************************************************************************************************************/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
    LCD_write_byte(0x40 | Y, 0);// column
    LCD_write_byte(0x80 | X, 0);// row
}
/********************************************************************************************************************/
LCD_write_point(unsigned char X, unsigned char Y)
{
unsigned int temp=0;
if(poy!=X) 
{
potemp1=0x00;
potemp2=0x00;
potemp3=0x00;
potemp4=0x00;        
potemp5=0x00;
potemp6=0x00;                
}
poy=X;

if(Y<=8)
{
if(pox!=1) potemp1=0x00;
pox=1;
temp=potemp1|tabpoint[Y];  
LCD_set_XY(X,8);
LCD_write_byte(temp, 1); 
potemp1=temp;
 }
 
 else if(Y<=16)
{
if(pox!=2) potemp2=0x00;                                  
pox=2;
temp=potemp2|tabpoint[Y-8];
LCD_set_XY(X,9);
LCD_write_byte(temp, 1);         
potemp2=temp;
 }
 
 else if(Y<=24)
{
if(pox!=3) potemp3=0x00;
pox=3;
temp=potemp3|tabpoint[Y-16];
LCD_set_XY(X,10);
LCD_write_byte(temp, 1); 
potemp3=temp;
}

 else if(Y<=32)
{
if(pox!=4) potemp4=0x00;
pox=4;
temp=potemp4|tabpoint[Y-24];
LCD_set_XY(X,11);
LCD_write_byte(temp, 1); 
potemp4=temp;
}

 else if(Y<=40)
{
if(pox!=5) potemp5=0x00;
pox=5;
temp=potemp5|tabpoint[Y-32];
LCD_set_XY(X,12);
LCD_write_byte(temp, 1); 
potemp5=temp;
}

 else if(Y<=48)
{
if(pox!=6) potemp6=0x00;
pox=6;
temp=potemp6|tabpoint[Y-40];
LCD_set_XY(X,13);
LCD_write_byte(temp, 1); 
potemp6=temp;
}
//for(temp=0;temp<50000;temp++);	                                                                                                                                                                                                                                
}
/********************************************************************************************************************/
void LCD_write_char(unsigned char c)
{
    unsigned char line;
	
    c -= 32;
    for (line=0; line<6; line++)
	{
        LCD_write_byte(font6x8[c][line], 1);
	}
}

void LCD_write_char2(unsigned char X, unsigned char Y,unsigned char c)
{
 	 
    unsigned char line;
	  LCD_set_XY(X,Y);
    c -= 32;
    for (line=0; line<6; line++)
	{
        LCD_write_byte(font6x8[c][line], 1);
	}
}
/********************************************************************************************************************/
/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数

输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
{
    LCD_set_XY(X,Y);
    while (*s) 
    {
	    LCD_write_char(*s);
	    s++;
    }
}
/********************************************************************************************************************/
/*-----------------------------------------------------------------------
LCD_write_chinese_string: 在LCD上显示汉字

输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；  
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
-----------------------------------------------------------------------*/   
/*                     
void LCD_write_chinese_string(unsigned char X, unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row)
{
    unsigned char i,n;
    
    LCD_set_XY(X,Y);                             //设置初始位置
    
    for (i=0;i<num;)
    {
      	for (n=0; n<ch_with*2; n++)              //写一个汉字
      	{ 
      	    if (n==ch_with)                      //写汉字的下半部分
      	    {
      	        if (i==0) LCD_set_XY(X,Y+1);
      	        else
				{
      	           LCD_set_XY((X+(ch_with+row)*i),Y+1);
				}
            }
      	    LCD_write_byte(HZK[line+i][n],1);
      	}
      	i++;
      	LCD_set_XY((X+(ch_with+row)*i),Y);
    }
}
*/
/********************************************************************************************************************/
/*-----------------------------------------------------------------------
LCD_draw_map      : 位图绘制函数

输入参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）
-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,unsigned char Pix_x,unsigned char Pix_y)
{
    unsigned int i,n;
    unsigned char row;
    
    if (Pix_y%8==0)
	{
	    row=Pix_y/8;      //计算位图所占行数
    }
    else
	{
        row=Pix_y/8+1;
    }
    for (n=0;n<row;n++)
    {
      	LCD_set_XY(X,Y);
        for(i=0; i<Pix_x; i++)
          {
            LCD_write_byte(map[i+n*Pix_x], 1);
          }
        Y++;                         //换行
    }      
}
/********************************************************************************************************************/
/*-----------------------------------------------------------------------
LCD_write_byte    : 写数据到LCD

输入参数：data    ：写入的数据；
          command ：写数据/命令选择；
-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
{
    unsigned char i;

	CLEARBIT(LCD_PORT,LCD_CE);// 使能LCD_CE = 0
    
    if (command == 0)
	{
	    CLEARBIT(LCD_PORT,LCD_DC);// 传送命令 LCD_DC = 0;
	}
    else
	{
	    SETBIT(LCD_PORT,LCD_DC);// 传送数据LCD_DC = 1;
	}
	
	for(i=0;i<8;i++)
	{
	 				//delay_1us();
		if(dat&0x80)
		{
			SETBIT(LCD_PORT,SDIN);//SDIN = 1;
		}
		else
		{
			CLEARBIT(LCD_PORT,SDIN);//SDIN = 0;
		}
		CLEARBIT(LCD_PORT,SCLK);//SCLK = 0;
		dat = dat << 1;
		SETBIT(LCD_PORT,SCLK);//SCLK = 1;
	}
    SETBIT(LCD_PORT,LCD_CE);//LCD_CE = 1;
}



void LCD_Show_Number (unsigned char X,unsigned char Y,unsigned int number) 
{
  LCD_set_XY(X,Y);
  LCD_write_char2(X,Y,number/1000+48);
  LCD_write_char2(X+6,Y,number%1000/100+48);
  LCD_write_char2(X+12,Y,number%100/10+48);
  LCD_write_char2(X+18,Y,number%10+48);
  
}