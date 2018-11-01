#include "app.h"

void Task_LED1(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;
    
    while(1)
    {
        BSP_LED1_Toggle();
        OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//ÑÓÊ±×èÈû100ms
    }
}

void Task_LED2(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;
    
    while(1)
    {
        BSP_LED2_Toggle();
        OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);	//ÑÓÊ±×èÈû100ms
    }
}

void Task_LED3(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;
    
    while(1)
    {
        BSP_LED3_Toggle();
        OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);	//ÑÓÊ±×èÈû100ms
    }
}
