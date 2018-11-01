/***************************************************
龙丘智能科技
Transplanted by 龙丘
E-mail:chiusir@yahoo.cn
软件版本:V1.1
最后更新:2010年11月2日
相关信息参考下列地址：
网站：  http://www.lqist.cn
淘宝店：http://shop36265907.taobao.com
------------------------------------
Code Warrior 5.0
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock: 8.000MHz
pllclock:16.000MHz
============================================
液晶管脚接线定义 
PIN1 GND
PIN2 5V
PIN3 GND(或者适当的电阻接地) 
PIN4 LCD1602_RS   PORTB_PB0	//Data Command Pin		1 data		0 command
PIN5 LCD1602_RW   PORTB_PB1 //Read Write Pin		  1 read 		0 write
PIN6 LCD1602_EN   PORTB_PB2 //LCD Enable Signal
PIN7-14 LCDIO     PORTA
*****************************************************
*/
#include "derivative.h"
#include "LQ1602.h"

void LcdInit(){
  DDRA = 0xff;
  DDRB = 0xff;
  LCDIO_DIR = LCDIO_DIR_OUT;
	LcdDelay();
	LcdCommand(CLR,0);          //clear screen 
	LcdCommand(DATA_MODE,1);    //set 8 bit data transmission mode 
	LcdCommand(INPUTMODE_CUR_R | INPUTMODE_ALL_D, 1);   // cursor right, disable moving
	LcdCommand(SCREEN_OPEN | SCREEN_OPEN_CUR | SCREEN_OPEN_TWI, 1); //open display (enable lcd display)
	LcdCommand(LINE1_HEAD,1);   //set lcd first display address 
	LcdCommand(CLR,1);          //clear screen
}

////////////////////////////////////////////////////////////////////
// write command function
//
void LcdCommand(byte command,byte BusyC){
	if (BusyC) ReadStatus(); //Test it busy or not
	LCDIO=command;
	LCD1602_RS=0;   
	LCD1602_RW=0;
	LCD1602_EN=0;
	LCD1602_EN=0;
	LCD1602_EN=1;
}

byte ReadStatus(void)
{
  byte cRtn;
	LCDIO_DIR = LCDIO_DIR_IN;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	LCD1602_EN = 0;
	LCD1602_EN = 0;
	LCD1602_EN = 1;
	while (LCDIO & BUSY); //Test Busy State
	cRtn = LCDIO_DIR;     // if Not save the port value, it should be change
	LCDIO_DIR = LCDIO_DIR_OUT;
	return(cRtn);
}

void LcdDelay(void)
{
  word i, j;
  for (i = 0; i < 300; i++)
    for(j = 0; j < 3000; j++);
}

void LcdClear(void)
{
  LcdCommand(CLR,1);          //clear screen
}
////////////////////////////////////////////////////////////////////
// write data function
//
void LcdData(byte dat,byte BusyC)
{
	if (BusyC) ReadStatus(); //Test it busy or not
	LCDIO=dat;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_EN=0;
	LCD1602_EN=0;
	LCD1602_EN=1;
}

////////////////////////////////////////////////////////////////////
// write lcd a character function
//
void LcdWriteChar( byte x,byte y,byte dat){
  LcdSetXY(x, y); 
	LcdData(dat,1);
}

////////////////////////////////////////////////////////////////////
// set display address function
//
void LcdSetXY( byte x, byte y ){
	byte address;
	if (y == LINE1) 
    address = LINE1_HEAD + x;
	else 
    address = LINE2_HEAD + x;
	LcdCommand(address,1); 
}

////////////////////////////////////////////////////////////////////
// write lcd string function
//
void LcdWriteStr(byte X,byte Y,char *s)
{
    LcdSetXY( X, Y ); //set address 
    while (*s)  // write character
    {
      LcdData(*s, 1);   
      s++;
    }
}