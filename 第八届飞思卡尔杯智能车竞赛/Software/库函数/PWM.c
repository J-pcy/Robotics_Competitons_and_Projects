#include "derivative.h" 
#include "PWM.h"
void PWM_init(void)
{
  PWME=0x00;
  PWMCLK_PCLK7=1;//ʱ��ѡ��Ĵ��� //2,3,6,7ѡ��ѡ��ClockB��ClockSB��1��   0,1,4,5 ѡ��ѡ��ClockA��ClockSA
  PWMPRCLK=0X33;// (ClockB) 0011 (ClockA) 0011 8��Ƶ   40/8=5000000
  PWMSCLA=150;   //Ԥ��Ƶ�Ĵ���
  PWMSCLB=500; //5000000/1000        5k
  PWMPOL_PPOL7=1;//��ʼλ�øߵ�ƽ
  PWMCAE_CAE7=0;//��������
  PWMCTL=0X00;//ͨ�������Ĵ���
}
/*
void PWMOUT(void)
{
  PWMPER7=200;//���ڼĴ��� 50Hz
  PWMDTY7=50;
  PWME=0x80; //1000 0000
}
*/
void MotorAngel(unsigned int angel)//angel�ķ�ΧΪ5-25 180�ȶ��
{
  PWMPER7=200;//���ڼĴ��� 50Hz
  PWMDTY7=angel;
  PWME=0x80; //����P7��
}