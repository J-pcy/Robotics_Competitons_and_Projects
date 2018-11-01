#include "includes.h"
#define uint unsigned int
void SCI0_init(void)
{
    SCI0BD=22; //40M  115200
    SCI0CR1=0x00;//��������ģʽ��1��8��1 ����żУ��
    SCI0CR2=0X0c;//0010 1100
}

void Uart_putchar(unsigned char ch)
{
  while(SCI0SR1_TDRE == 0) 
  {           /* Is the transmitter empty? */
  }
  SCI0DRL = (uint8_t)ch;
} 

void Uart_putstring( char *dat)
{
   while(*dat != '\0')           //�ж��ַ����Ƿ������
 {
  Uart_putchar(*dat);        //���͵����ַ�
  dat++;                 //�ַ���ַ��1��ָ������һ���ַ�
 }
}

void Uart_output(unsigned int data) 
{

  Uart_putchar(data/1000+48);
  Uart_putchar(data%1000/100+48);
  Uart_putchar(data%1000%100/10+48);
  Uart_putchar(data%1000%100%10+48);
  Uart_putchar('\r');
  Uart_putchar('\n');
}

