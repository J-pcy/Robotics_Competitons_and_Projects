#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void delay(void) {
  unsigned int loop_i,loop_j;
  for(loop_i=0;loop_i<500;loop_i++) {
    for(loop_j=0;loop_j<500;loop_j++) {
      ;
    }
  }
}   



void main(void) 
{

  /* put your own code here */
  unsigned char loop_index;
  _FEED_COP(); /* feeds the dog */

  
  DDRB = 0xFF;
  loop_index = 0;
  while(1) 
  {
    switch(loop_index) 
    {
      case 0:PORTB = 0xFE;break;
      case 1:PORTB = 0xFD;break;
      case 2:PORTB = 0xFB;break;
      case 3:PORTB = 0xF7;break;
      case 4:PORTB = 0xEF;break;
      case 5:PORTB = 0xDF;break;
      case 6:PORTB = 0xBF;break;
      case 7:PORTB = 0x7F;
    }
    loop_index++;
    if(loop_index == 8) 
    {
      loop_index = 0;
    }
    delay();
  } 
  
/*  while(1) 
  { 
    PORTB = 0xFE;
  }    */



  /* loop forever */
  /* please make sure that you never leave main */
}
