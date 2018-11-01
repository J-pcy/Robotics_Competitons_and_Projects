#ifndef __ADC_H_ 
#define __ADC_H_
#include "derivative.h"      /* derivative-specific definitions */
extern void AD_Init(void) ;
extern unsigned int  AD_Measure10(unsigned char Channel); 
#endif
