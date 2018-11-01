#include "derivative.h"
#include "Uart0.h"
void SCI0_init()
{
    SCI0BD=52; //����Ĭ��Ϊ8M  9600
    SCI0CR1=0x00;//��������ģʽ��1��8��1 ����żУ��
    SCI0CR2=0X0c;//0010 1100
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
 }
}