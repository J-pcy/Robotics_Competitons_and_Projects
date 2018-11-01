#include "derivative.h"
#include "Uart0.h"
#define uint unsigned int
void SCI0_init(void)
{
    SCI0BD=52; //开机默认为8M  9600
    SCI0CR1=0x00;//正常工作模式，1：8：1 无奇偶校验
    SCI0CR2=0X0c;//0010 1100
}

void Uart_putchar(char ch)
{
  while(!(SCI0SR1&0x80));    //检查数据发送寄存器      1：发送寄存器为空可以发送数据
  SCI0DRH=0;
  SCI0DRL=ch;
} 

void Uart_putstring( char *dat)
{
   while(*dat != '\0')           //判断字符串是否发送完毕
 {
  Uart_putchar(*dat);        //发送单个字符
  dat++;                 //字符地址加1，指向先下一个字符
 }
}

void Uart_output(uint data)
{
    Uart_putchar(data/1000+48);
    Uart_putchar(data%1000/100+48);
    Uart_putchar(data%1000%100/10+48);
    Uart_putchar(data%1000%100%10+48);
    Uart_putstring("\r\n");
}