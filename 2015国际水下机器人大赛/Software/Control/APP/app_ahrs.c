#include "app.h"

void Task_AHRS(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;
    
    BSP_Timer4_Init();
    BSP_USART1_Init();
    
    AHRS_MPU6050_Init();
    AHRS_HMC5883L_Init();
    AHRS_IMU_Init();
    
    
    
    while(1)
    {
        AHRS_MPU6050_GetFilterData();//��ȡ�˲������ֵ
        AHRS_HMC5883L_GetFilterData();
        AHRS_IMU_Get_Angle();
        
        DMA_DataUpdate();
        
        OSTimeDlyHMSM(0, 0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);	//��ʱ����100ms
    }
}


