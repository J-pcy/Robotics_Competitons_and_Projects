#include "derivative.h"      /* derivative-specific definitions */
#include "ADC.h"

void AD_Init(void) 
{
    ATD0CTL4 = 0x04;                    /* 设定采样时间与时钟 */
    ATD0CTL3 = 0x88;                    /* 右对齐 */
    ATD0CTL0 = 0x0F;                    /* Set wrap around */
    ATD0CTL1 = 0x2F;                    /* 10位的精度 */
    ATD0CTL2 = 0x40;                    /* 快速清零 */
}

/*
*********************************************************************************************************
* Description: AD_Measure10
*********************************************************************************************************
*/
unsigned int  AD_Measure10(unsigned char Channel) 
{
    ATD0CTL5_Cx = Channel;
    while(ATD0STAT0_SCF == 0);
    return ATD0DR0;
}

/*
*********************************************************************************************************
* Description: AD_Measure12
*********************************************************************************************************
*/
unsigned int AD_Measure12(unsigned char Channel) 
{
    ATD0CTL1 = 0x4F;                    /* 12位的精度 */
    ATD0CTL5_Cx = Channel;
    while(ATD0STAT0_SCF == 0);
    return ATD0DR0;
}
