#ifndef __Uart0_H_
#define __Uart0_H_
extern void SCI0_init(void);
extern void Uart_putchar(char ch);
extern void Uart_putstring( char *dat);
#endif