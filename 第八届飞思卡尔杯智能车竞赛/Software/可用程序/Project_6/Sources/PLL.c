#include "derivative.h"
#include "PLL.h"
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
  /*  Initialization code after reset */
    MMCCTL1 = 0;                //Memory Mapping Control
    DIRECT  = 0;                //Direct Page Register
    IVBR    = 0xFF;             //Interrupt Vector Base Register

    ECLKCTL = 0xC0;             //ECLK Control Register
                                //ECLK disabled
                                //ECLKX2 disabled

/*  System clock initialization */
    CLKSEL  = 0;                //S12XECRG Clock Select Register,Select clock source from XTAL and set bits in CLKSEL reg.
                                //System clocks are derived from OSCCLK
                                //Oscillator is disabled in Stop Mode
                                //Loop controlled Pierce Oscillator is selected
                                //IPLL keeps running in Wait Mode
                                //RTI keeps running in Wait Mode
                                //COP keeps running in Wait Mode

    PLLCTL  = 0x81;             //S12XECRG IPLL Control Register,Disable the PLL
                                //Clock monitor is enabled
                                //IPLL is turned off
                                //FM off
                                //Fast wake-up from full stop mode is disabled
                                //RTI stops running during Pseudo Stop Mode
                                //COP stops running during Pseudo Stop Mode
                                //Detection of crystal clock failure forces the MCU in Self Clock Mode

    SYNR    = 0x44;             //S12XECRG Synthesizer Register,Set the multiplier register
                                //VCOFRQ[1:0] = 0b01,48MHz < fVCO<= 80MHz
                                //SYNDIV[5:0] = 0b000100,SYNDIV = 4

    REFDV   = 0x81;             //S12XECRG Reference Divider Register,Set the divider register
                                //REFFRQ[1:0] = 0b10,6MHz < fREF <= 12MHz
                                //REFDIV[5:0] = 0b000001,REFDIV = 1

    POSTDIV = 0;                //S12XECRG Post Divider Register,Set the post divider register
                                //POSTDIV[4:0] = 0b00000,POSTDIV = 0,If POSTDIV = $00 then fPLL is identical to fVCO (divide by one).

    PLLCTL  = 0xC1;             //S12XECRG IPLL Control Register
                                //Clock monitor is enabled
                                //IPLL is turned on
                                //FM off
                                //Fast wake-up from full stop mode is disabled
                                //RTI stops running during Pseudo Stop Mode
                                //COP stops running during Pseudo Stop Mode
                                //Detection of crystal clock failure forces the MCU in Self Clock Mode
    while(CRGFLG_LOCK == 0);    //Wait until the PLL is within the desired tolerance of the target frequency
    CLKSEL  = 0x80;             //S12XECRG Clock Select Register,Select clock source from PLL
                                //System clocks are derived from PLLCLK (fBUS = fPLL / 2).

/*  High Temperature Control */
    VREGHTCL= 0x10;

/*  Interrupt Enable Register */
    CRGINT &= (unsigned char)~(unsigned char)0x12;

/*  VREGCTRL - Control Register */
    VREGCTRL &= (unsigned char)~(unsigned char)0x02;

/*  CRG COP Control Register */
    COPCTL = 0x00;

/*  Interrupt Control Register */
    IRQCR &= (unsigned char)~(unsigned char)0x40;
}
