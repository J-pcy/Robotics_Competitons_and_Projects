#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "5110.h"
#include "PLL.h"
void main(void)
{
PLL_init_80M();
LCD_init();
EnableInterrupts;
for(;;) 
{
_FEED_COP();
LCD_write_english_string(0,2,"Program 1"); 
LCD_Show_Number (0,3,8754);
} 
}
