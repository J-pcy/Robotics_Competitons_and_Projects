#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "PLL.h"
#include "PWM.h"
void main(void) 
{
  PLL_init_48M();
  PWM_init();
  PWMOUT();
  for(;;) 
  {
    _FEED_COP(); 
    PORTB=0xff;
    PUCR=0;
  } 
}
