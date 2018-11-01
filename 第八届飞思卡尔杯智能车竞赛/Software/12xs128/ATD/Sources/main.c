#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


void ATDO_Init(void)
{
  ATD0DIEN = 0x00;
  ATD0CTL1 = 0x00;
  ATD0CTL2 = 0x40;
  ATD0CTL3 = 0x88;
  ATD0CTL4 = 0x03;
  ATD0CTL5 = 0x20;
  
}



void main(void) 
{
  /* put your own code here */
  
  unsigned char AD_result;
  ATDO_Init();
  DDRB = 0xFF;
  PORTB = 0xFF;

//	EnableInterrupts;


  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    while(!ATD0STAT2L_CCF0)
    {
      ;
    }
    AD_result = ATD0DR0L;
    PORTB = AD_result;
  } /* loop forever */
  /* please make sure that you never leave main */
}
