#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
void main(void) 
{
SCI0_init();
EnableInterrupts;
for(;;) 
{
  _FEED_COP(); /* feeds the dog */
 // Uart_putstring("I Love you");
  Uart_putchar(48);
  Uart_putstring("\r\n");
} /* loop forever */
  /* please make sure that you never leave main */
}
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 6 void IRQ_int(void)
{
     Uart_putstring("We are friends");
}
#pragma CODE_SEG DEFAULT