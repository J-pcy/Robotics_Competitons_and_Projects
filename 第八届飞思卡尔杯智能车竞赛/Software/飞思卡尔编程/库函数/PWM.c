#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;// 2,3,6,7ѡ��ѡ��B��SB��1��   0,1,4,5 ѡ��ѡ��A��SA
  //PWMCLK_PCLK2=1;//ȫ��ѡ��ΪSB 
  PWMPRCLK=0X03;// (SB)0011 (SA)0011 8��Ƶ   24/8=3000000
  PWMSCLA=150;   //Ԥ��Ƶ�Ĵ���
  PWMSCLB=8; //3000000/2        1500k
  PWMPOL_PPOL7=1;//��ʼλ�øߵ�ƽ
 // PWMPOL_PPOL2=1;//��ʼλ�øߵ�ƽ
  PWMCAE_CAE7=0;//���Ķ���
  //PWMCAE_CAE2=1;//���Ķ���
  PWMCTL=0X00;//ͨ�������Ĵ���
}

void PWMOUT(void)
{
 // PWMPER2=80;//���ڼĴ��� 1k
 // PWMDTY2=70;
  PWMPER7=100;//���ڼĴ��� 15k
  PWMDTY7=70;
  PWME=0x80; //1000 0000
}