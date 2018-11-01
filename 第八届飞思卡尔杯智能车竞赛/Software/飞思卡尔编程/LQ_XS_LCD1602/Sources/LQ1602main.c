/*
Code Warrior 5.0
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock: 8.000MHz
pllclock:16.000MHz
============================================
液晶管脚接线定义 
PIN1 GND
PIN2 5V
PIN3 10K电阻接地 
PIN4 LCD1602_RS   PORTB_PB0	//Data Command Pin		1 data		0 command
PIN5 LCD1602_RW   PORTB_PB1 //Read Write Pin		  1 read 		0 write
PIN6 LCD1602_EN   PORTB_PB2 //LCD Enable Signal
PIN7-14 LCDIO     PORTA
*****************************************************
*/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "LQ1602.h" 

#pragma CODE_SEG DEFAULT
void SetBusCLK_16M(void)
{   
    CLKSEL=0X00;				
    PLLCTL_PLLON=1;		
    SYNR=0x00 | 0x01; 	                        				
    REFDV=0x80 | 0x01;                                                           
    POSTDIV=0x00;                               
    _asm(nop);          
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  
    CLKSEL_PLLSEL =1;		           
    DDRA=0XFF;
    PORTA=0XFF;
    DDRB=0XFF;
    PORTB=0X00;//直接定义PA为输出 PB口为待定
}

void delayms(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<1335;jj++);    //16MHz--1ms  
}

void main(void) 
{
  //unsigned char x, y, z,i;
	//char buf[17];
	SetBusCLK_16M(); 
	LcdInit();
	  
  DisableInterrupts; /* enable interrupts */
  /* include your code here */ 
  
	//LCD_WT_CHAR(8,  0, 0x66);
				
  for(;;) 
  {
    LcdClear();
    delayms(500);
    LcdWriteStr(1,0,"LongQiu s.&t.");      
    LcdWriteStr(0,1,">>>2010-11-02<<<"); 
    delayms(5000);
    PORTB_PB7=~PORTB_PB7;     
  } /* loop forever */
  /* please make sure that you never leave main */
}
