#include "bsp.h"

void BSP_Init(void)
{
    SysTick_init();
    
    BSP_LED_Init();

}


void SysTick_init(void)
{
  /* 初始化并使能SysTick定时器 */
  SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ);
  
  /*  配置1ms 中断一次，即os的频率为1000hz */
	if (SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))	
	{ 
		/* Capture error */ 
		while (1);
	}
}



