#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;// 2,3,6,7ѡ��ѡ��B��SB��1��   0,1,4,5 ѡ��ѡ��A��SA
  PWMCLK_PCLK5=1;// 2,3,6,7ѡ��ѡ��B��SB��1��   0,1,4,5 ѡ��ѡ��A��SA
  PWMPRCLK=0X11;// (B)0011  (A)0011 8��Ƶ     40/2=2000000
  PWMSCLA=3;   // 20M/6 =2M
  PWMSCLB=3; //
  PWMPOL_PPOL7=1;//��ʼλ�øߵ�ƽ
  PWMPOL_PPOL5=1;//��ʼλ�øߵ�ƽ
  PWMCAE_CAE7=1;//���Ķ���
  PWMCAE_CAE5=1;//���Ķ���
  PWMCTL=0X00;//ͨ�������Ĵ���
}

void PWMOUT_L(unsigned char k)
{
  PWMPER7=100;//���ڼĴ��� 15k
  PWMDTY7=k;
  PWME_PWME7=1;
}



void PWMOUT_R(unsigned char k) 
{

  PWMPER5=100;//���ڼĴ��� 20k
  PWMDTY5=k;
  PWME_PWME5=1;
}