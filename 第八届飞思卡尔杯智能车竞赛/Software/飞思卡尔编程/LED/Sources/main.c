#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "PLL.h"
void delay(unsigned int x) 
{
  unsigned int i,j;
  for(i=0;i<x;i++)
  {
    for(j=0;j<4100;j++);//24M 总线时钟时大约为1ms
  }
}

void main(void) 
{
  EnableInterrupts
  PLL_init_48M();
  /* put your own code here */
  DDRH=0xff;
  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    PTH=0x00;
    delay(1000);                                                          
    PTH=0xff;
    delay(1000);    
  } /* loop forever */
  /* please make sure that you never leave main */
}
