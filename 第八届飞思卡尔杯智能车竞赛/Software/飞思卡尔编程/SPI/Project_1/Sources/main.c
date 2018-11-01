#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void SPI_Init(void)   
{
  SPI0BR=0x77;   //(SPPR+1)*2^(SPR+1) 8*2^8
  SPI0CR1=0x50;   //SPIE SPE SPTIE MSTR CPOL CPHA SSOE LSBFE 
}                //  0   1    0    1      0  0    0    0

void SPI_Sendata(unsigned char dat)
{
   SPI0DR = dat; //Ð´ÈëÊý¾Ý
   while(!(SPI0SR&0x20));
}
  
void main(void) 
{
  /* put your own code here */
EnableInterrupts;
for(;;) 
{
    _FEED_COP(); /* feeds the dog */
    SPI_Sendata(0xff);
  } /* loop forever */
  /* please make sure that you never leave main */
}
