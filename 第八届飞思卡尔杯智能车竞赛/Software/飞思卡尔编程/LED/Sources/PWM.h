#include "derivative.h" 
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK3=1;// 2,3,6,7ѡ��ѡ��B��SB��1��   0,1,4,5 ѡ��ѡ��A��SA
  PWMCLK_PCLK2=1;//ȫ��ѡ��ΪSB 
  PWMPRCLK=0X03;// (SB)0011 (SA)0011 8��Ƶ   24/8=3000000
  PWMSCLA=150;   //Ԥ��Ƶ�Ĵ���
  PWMSCLB=15; //3000000/2*15        100k
  PWMPOL_PPOL3=1;//��ʼλ�øߵ�ƽ
  PWMPOL_PPOL2=1;//��ʼλ�øߵ�ƽ
  PWMCAE_CAE3=1;//���Ķ���
  PWMCAE_CAE2=1;//���Ķ���
  PWMCTL=0X00;//ͨ�������Ĵ���
}

void PWMOUT(void)
{
  PWMPER2=80;//���ڼĴ��� 1k
  PWMDTY2=70;
  PWMPER3=80;//���ڼĴ��� 1k
  PWMDTY3=50;
  PWME=0x0c; //0000 1100
}