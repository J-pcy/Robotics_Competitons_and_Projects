#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


void PWM_Init(void) 
{
  PWME = 0x00;       //禁止PWM模块
  PWMPRCLK = 0x66;   //CLOCKA、CLOCKB 64分频
  PWMSCLA = 125;     //SA分频因子125
  PWMSCLB = 125;     //SB分频因子125
  PWMCLK = 0x07;     //通道0、1选择SA作为PWM时钟，通道2选择SB作为PWM时钟
  PWMPER0 = 200;     //周期寄存器
  PWMPER1 = 200;     
  PWMPER2 = 200;
  PWMDTY0 = 0;       //占空比寄存器
  PWMDTY1 = 0;
  PWMDTY2 = 0;
  PWMPOL = 0x07;     //极性寄存器
  PWMCAE = 0x00;     //左对齐输出
  PWMCTL = 0x00;     //不级联
  PWME = 0x07;       //使能PWM通道0、1、2
}

void delay(unsigned int delay_time)
{
  unsigned int loop_i,loop_j;
  for(loop_i=0;loop_i<delay_time;loop_i++)
  {
    for(loop_j=0;loop_j<500;loop_j++)
    {
      ;
    }
  }
}



void main(void) 
{
  /* put your own code here */
  unsigned char loop_index;
  PWM_Init();
  
  


//	EnableInterrupts;


  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    for(loop_index=0;loop_index<200;loop_index++)
    {
      PWMDTY0 = loop_index;
      PWMDTY1 = loop_index;
      PWMDTY2 = loop_index;
      delay(2000);
    }
    for (loop_index=200;loop_index>0;loop_index--)
    {
      PWMDTY0 = loop_index;
      PWMDTY1 = loop_index;
      PWMDTY2 = loop_index;
      delay(2000);
    }
  } /* loop forever */
  /* please make sure that you never leave main */
}
