#ifndef _LQ1602_
#define _LQ1602_

#define 	LINE1					      0
#define 	LINE2					      1
#define 	LINE1_HEAD				  0x80
#define 	LINE2_HEAD			  	0xC0
#define 	DATA_MODE				    0x38    // 8 bit data transmission mode, double line

// LCD1602 Command Macro
#define 	CLR						      0x01
#define		BUSY 					      0x80 		// LCD Busy Tag
#define   CURSOR_RESET        0x02
#define   INPUTMODE_CUR_R     0x06
#define   INPUTMODE_CUR_L     0x04
#define   INPUTMODE_ALL_E     0x05
#define   INPUTMODE_ALL_D     0x04
#define 	SCREEN_OPEN 				0x0C
#define 	SCREEN_OPEN_CUR			0x02
#define 	SCREEN_OPEN_TWI			0x01
#define   SCSHIFT_CUR_L       0x10    // Screen or Cursor shift
#define   SCSHIFT_CUR_R       0x14
#define   SCSHIFT_SCR_L       0x18
#define   SCSHIFT_SCR_R       0x1C

// LCD1602 Pins
#define   LCD1602_RS          PORTB_PB0	//Data Command Pin		1 data		0 command
#define   LCD1602_RW          PORTB_PB1 //Read Write Pin		  1 read 		0 write
#define   LCD1602_EN          PORTB_PB2 //LCD Enable Signal
#define   LCDIO               PORTA
#define   LCDIO_DIR           DDRA
#define   LCDIO_DIR_IN        0x00
#define   LCDIO_DIR_OUT       0xFF

void LcdCommand(byte command,byte BusyC);	//write command function
void LcdData(byte temp,byte BusyC);			//write data function
void LcdSetXY( byte x, byte y );			//set display address function
void LcdWriteChar( byte x,byte y,byte dat);//write lcd a character function
void LcdWriteStr(byte X,byte Y,char *s);//write lcd string function
void LcdInit(void);											   //LCD initize function
byte ReadStatus(void);
void LcdDelay(void);                       //LCD Delay Function 
void LcdClear(void);

#endif