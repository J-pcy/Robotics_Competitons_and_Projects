/********************************************
龙丘智能车多功能开发平台
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.0
最后更新:2009年4月19日
相关信息参考下列地址：
博客：  http://longqiu.21ic.org
淘宝店：http://shop36265907.taobao.com
------------------------------------
Code Warrior 5.0
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz    
 
使用说明：
H7,J0外部中断演示，分别亮灭PB0和PB2。
============================================*/
#include "derivative.h"
#include <stdio.h>
#include <string.h>    
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 24 PORTJ_ISR(void)
{         
  PORTB_PB0=~PORTB_PB0;
  PIFJ_PIFJ0=1;   //清除中断标志位 
}
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 25 PORTH_ISR(void)
{
  PORTB_PB2=~PORTB_PB2;
  PIFH_PIFH7=1;   //清除中断标志位        
}
static void Port_Init(void)
{  
    DDRA = 0xff;  //LCD1100,PA0--4,PA67 D1D2
    PORTA= 0x00;   
       
    DDRB = 0xff;  //LED  PTB0--7,
    PORTB= 0xff;  //LEDs on  
   
    DDRE = 0xFF;  //MOTOR CONTROL
    PORTE= 0x00;  //     
}
void SetBusCLK_32M(void)
{   
    CLKSEL=0X00;    // disengage PLL to system
    PLLCTL_PLLON=1;   // turn on PLL
    SYNR =0x40 | 0x03;  // pllclock=2*osc*(1+SYNR)/(1+REFDV)=64MHz;                      
    REFDV=0xc0 | 0x01; 
    POSTDIV=0x00;  
    _asm(nop);          // BUS CLOCK=32M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));   //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;          //engage PLL to system;  
}
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     //for(jj=0;jj<1335;jj++);  //16MHz--1ms
     for(jj=0;jj<2770;jj++);  //32MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
} 
//============================MAIN()===========================
void main(void) 
{
  /* put your own code here */
  byte i=0;
  
  Port_Init();            // initial board 
  SetBusCLK_32M();  
  PORTB=0XFF; 
  EnableInterrupts;
  for(;;) 
  { 
     Dly_ms(500);
     PORTB_PB1=~PORTB_PB1; 
  }
  /* please make sure that you never leave this */
}
 
// ----------------------
