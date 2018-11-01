#ifndef __PLL_H_ 
#define __PLL_H_
#include "derivative.h"
extern void PLL_init_48M(void);//设置锁相环的时钟为48MHz 总线频率为24MHz

extern void PLL_init_80M(void);
#endif