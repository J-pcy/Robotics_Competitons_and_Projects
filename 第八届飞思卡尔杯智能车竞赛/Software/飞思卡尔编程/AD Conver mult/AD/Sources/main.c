#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#define uint unsigned int
#define uchar unsigned char
unsigned int ADValue;
void AD_Init(void) 
{
    ATD0CTL4 = 0x04;                    /* 设定采样时间与时钟 */
    ATD0CTL3 = 0x80;                    /* 右对齐 采样16次*/ 
    ATD0CTL0 = 0x0F;                    /* Set wrap around */
    ATD0CTL1 = 0x4F;                    /* 12位的精度 */
    ATD0CTL2 = 0x40;                    /* 快速清零 */
    ATD0CTL5_SCAN=1;
}


/*
*********************************************************************************************************
* Description: AD_Measure12
*********************************************************************************************************
*/
uint  AD_Measure12(uchar Channel) 
{    
   unsigned int * adr = &ATD0DR2;
   unsigned char i;
   ADValue = 0;
   for(i=0;i<16;i++)
   {
   ATD0CTL5_Cx = Channel;
   while(ATD0STAT0_SCF == 0);
   ADValue += *adr++;
   }
   ADValue /= 16;
   return ADValue;
}



void main(void) 
{
  /* put your own code here */
  uint data;
  AD_Init();
  SCI0_init();
	EnableInterrupts;
  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    data=AD_Measure12(2);
    Uart_putchar(data/1000+48);
    Uart_putchar(data%1000/100+48);
    Uart_putchar(data%1000%100/10+48);
    Uart_putchar(data%1000%100/10+48);
    Uart_putstring("\r\n");
  } /* loop forever */
  /* please make sure that you never leave main */
}

