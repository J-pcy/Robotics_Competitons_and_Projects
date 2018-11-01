#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
unsigned int   SCI0_flag0,Rec;

void SCI0_init()
  {
    SCI0BD=52; //����Ĭ��Ϊ8M  9600
    SCI0CR1=0x00;//��������ģʽ��1��8��1 ����żУ��
    SCI0CR2=0X2c;//0010 1100
  }

void delay()
{
  
  unsigned int i,j;
  for(i=0;i<300;i++)
  for(j=0;j<500;j++);
}

void Uart_putchar(char ch)
{
  while(!(SCI0SR1&0x80));    //������ݷ��ͼĴ���      1�����ͼĴ���Ϊ�տ��Է�������
  SCI0DRH=0;
  SCI0DRL=ch;
} 

void Uart_putstring(unsigned char *dat)
{
   while(*dat != '\0')           //�ж��ַ����Ƿ������
 {
  Uart_putchar(*dat);        //���͵����ַ�
  dat++;                 //�ַ���ַ��1��ָ������һ���ַ�
  delay();
 }
}
  
  
void main(void) 
{
SCI0_init();
EnableInterrupts;
for(;;) 
{
  _FEED_COP(); /* feeds the dog */
  switch(Rec)
  {
    case 'a':Uart_putstring("we receive a 'a' ");break;
    case 'b':Uart_putstring("we receive a 'b' ");break;
    default:break;
  }
 // Uart_putstring("I Love you");
//if(SCI0_flag0==1)
//{
//  SCI0_flag0=0;
//  Uart_putchar(Rec);
//  Uart_putstring("successful");
//  Uart_putstring("\r\n");
//}
  
} /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 20 void SCI0_ISR(void)
{
 if(SCI0SR1_RDRF==1)
 {
//   SCI0_flag0=1;
   Rec=SCI0DRL;
 } 
}














