#ifndef __PLL_H_ 
#define __PLL_H_
#include "derivative.h"
extern void PLL_init_48M(void);//�������໷��ʱ��Ϊ48MHz ����Ƶ��Ϊ24MHz

extern void PLL_init_80M(void);

extern void Cpu_Delay100US(uint16_t us100);

#endif