#include "bsp.h"

void BSP_Init(void)
{
    SysTick_init();
    
    BSP_LED_Init();

}


void SysTick_init(void)
{
  /* ��ʼ����ʹ��SysTick��ʱ�� */
  SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ);
  
  /*  ����1ms �ж�һ�Σ���os��Ƶ��Ϊ1000hz */
	if (SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))	
	{ 
		/* Capture error */ 
		while (1);
	}
}



