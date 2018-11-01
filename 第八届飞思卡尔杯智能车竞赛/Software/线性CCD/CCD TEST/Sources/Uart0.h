#ifndef __Uart0_H_
#define __Uart0_H_
#include "derivative.h"
extern void SCI0_init(void);
extern void Uart_putchar(unsigned char ch);
extern void Uart_putstring( char *dat);
extern void Uart_output(unsigned int data);
#endif