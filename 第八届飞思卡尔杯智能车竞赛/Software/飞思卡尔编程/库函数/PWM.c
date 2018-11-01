#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;// 2,3,6,7选择选择B或SB（1）   0,1,4,5 选择选择A或SA
  //PWMCLK_PCLK2=1;//全部选择为SB 
  PWMPRCLK=0X03;// (SB)0011 (SA)0011 8分频   24/8=3000000
  PWMSCLA=150;   //预分频寄存器
  PWMSCLB=8; //3000000/2        1500k
  PWMPOL_PPOL7=1;//起始位置高电平
 // PWMPOL_PPOL2=1;//起始位置高电平
  PWMCAE_CAE7=0;//中心对齐
  //PWMCAE_CAE2=1;//中心对齐
  PWMCTL=0X00;//通道级联寄存器
}

void PWMOUT(void)
{
 // PWMPER2=80;//周期寄存器 1k
 // PWMDTY2=70;
  PWMPER7=100;//周期寄存器 15k
  PWMDTY7=70;
  PWME=0x80; //1000 0000
}