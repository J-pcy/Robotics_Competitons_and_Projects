#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/******—” ±ms******/
void Delay_ms(uint mt) 
{
  uint mx,my;
  for(mx=7800;mx>0;mx--)
    for(my=mt;my>0;my--);
}          

/******—” ±us******/
void Delay_us(uint ut)
{
  uint ux,uy;
  for(ux=0;ux<=8;ux++) 
     for(uy=ut;uy>0;uy--);
}

