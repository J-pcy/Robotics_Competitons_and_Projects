#ifndef __PWM_H_ 
#define __PWM_H_
#include "derivative.h"
extern void PWM_init(void);
extern void PWMOUT_L(unsigned char k);
extern void PWMOUT_R(unsigned char k);
#endif