#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#include "ADC.h"
#define uint unsigned int
#define uchar unsigned char

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
    data=AD_Measure12(1);
    Uart_putchar(data/1000+48);
    Uart_putchar(data%1000/100+48);
    Uart_putchar(data%1000%100/10+48);
    Uart_putchar(data%1000%100%10+48);
    Uart_putstring("\r\n");
  } /* loop forever */
  /* please make sure that you never leave main */
}

