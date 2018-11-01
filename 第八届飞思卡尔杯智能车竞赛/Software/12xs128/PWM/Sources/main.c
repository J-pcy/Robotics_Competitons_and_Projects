#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


void PWM_Init(void) 
{
  PWME = 0x00;       //��ֹPWMģ��
  PWMPRCLK = 0x66;   //CLOCKA��CLOCKB 64��Ƶ
  PWMSCLA = 125;     //SA��Ƶ����125
  PWMSCLB = 125;     //SB��Ƶ����125
  PWMCLK = 0x07;     //ͨ��0��1ѡ��SA��ΪPWMʱ�ӣ�ͨ��2ѡ��SB��ΪPWMʱ��
  PWMPER0 = 200;     //���ڼĴ���
  PWMPER1 = 200;     
  PWMPER2 = 200;
  PWMDTY0 = 0;       //ռ�ձȼĴ���
  PWMDTY1 = 0;
  PWMDTY2 = 0;
  PWMPOL = 0x07;     //���ԼĴ���
  PWMCAE = 0x00;     //��������
  PWMCTL = 0x00;     //������
  PWME = 0x07;       //ʹ��PWMͨ��0��1��2
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
