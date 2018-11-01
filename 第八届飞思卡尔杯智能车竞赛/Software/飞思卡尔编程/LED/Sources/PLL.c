#include "derivative.h"
void PLL_init_48M(void)
{
//Fpllclk = 2 * Foscclk * (SYNR+1)/(REFDV+1)
  CLKSEL=0X00;
  PLLCTL_PLLON=1;
  SYNR=0x00|0x02; //0X00|0X02
  REFDV=0x80|0x01; //0X00|0X01;
  _asm(nop);
  _asm(nop);
  while(CRGFLG_LOCK!=1);
  CLKSEL=0x80; 
}
//设置锁相环的时钟为48MHz 总线频率为24MHz

void PLL_init_80M(void)
{
//Fpllclk = 2 * Foscclk * (SYNR+1)/(REFDV+1)
  CLKSEL=0X00;
  PLLCTL_PLLON=1;
  SYNR=0xc0|0x04; //0x11 000100  //还有一些疑问
  REFDV=0x80|0x01; //10 000001
  _asm(nop);
  _asm(nop);
  while(CRGFLG_LOCK!=1);
  CLKSEL=0x80; 
}
