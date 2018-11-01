#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;// 2,3,6,7选择选择B或SB（1）   0,1,4,5 选择选择A或SA
  PWMCLK_PCLK5=1;// 2,3,6,7选择选择B或SB（1）   0,1,4,5 选择选择A或SA
  PWMPRCLK=0X11;// (B)0011  (A)0011 8分频     40/2=2000000
  PWMSCLA=3;   // 20M/6 =2M
  PWMSCLB=3; //
  PWMPOL_PPOL7=1;//起始位置高电平
  PWMPOL_PPOL5=1;//起始位置高电平
  PWMCAE_CAE7=1;//中心对齐
  PWMCAE_CAE5=1;//中心对齐
  PWMCTL=0X00;//通道级联寄存器
}

void PWMOUT_L(unsigned char k)
{
  PWMPER7=100;//周期寄存器 15k
  PWMDTY7=k;
  PWME_PWME7=1;
}



void PWMOUT_R(unsigned char k) 
{

  PWMPER5=100;//周期寄存器 20k
  PWMDTY5=k;
  PWME_PWME5=1;
}