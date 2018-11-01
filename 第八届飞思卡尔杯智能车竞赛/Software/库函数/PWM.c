#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;//时钟选择寄存器 //2,3,6,7选择选择ClockB或ClockSB（1）   0,1,4,5 选择选择ClockA或ClockSA
  PWMPRCLK=0X33;// (ClockB) 0011 (ClockA) 0011 8分频   40/8=5000000
  PWMSCLA=150;   //预分频寄存器
  PWMSCLB=500; //5000000/1000        5k
  PWMPOL_PPOL7=1;//起始位置高电平
  PWMCAE_CAE7=0;//左对齐输出
  PWMCTL=0X00;//通道级联寄存器
}
/*
void PWMOUT(void)
{
  PWMPER7=200;//周期寄存器 50Hz
  PWMDTY7=50;
  PWME=0x80; //1000 0000
}
*/
void MotorAngel(unsigned int angel)//angel的范围为5-25 180度舵机
{
  PWMPER7=200;//周期寄存器 50Hz
  PWMDTY7=angel;
  PWME=0x80; //开启P7口
}