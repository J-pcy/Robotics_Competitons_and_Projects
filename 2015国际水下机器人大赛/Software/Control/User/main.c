#include "user.h"

OS_TCB	StartUp_TCB;		   					                //����������ƿ�
CPU_STK	StartUp_Stk[STARTUP_TASK_STK_SIZE];                     //���������ջ

uint32_t countpertime = 0;
uint32_t counterpers;

uint32_t table[8] = {
                        0x00000000,//1
                        0x11111111,//2
                        0x22222222,//3
                        0x33333333,//4
                        0x44444444,//5
                        0x55555555,//6
                        0x66666666,//7
                        0x77777777,//8
                    };


uint32_t table_temp[8] =  {0};
       

//========================================
/*             MIAN               */   
//========================================
int main(void)
{


    {
    BSP_LED_Init();    
    
    BSP_Timer4_Init();
    BSP_USART1_Init();
    
    AHRS_MPU6050_Init();
    AHRS_HMC5883L_Init();
    AHRS_IMU_Init();
        
        
        
        
        
        
        
        
        
        
//        
//        if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != SET)
//        {
//            NVIC_SystemReset();
//        }
//    
    
    //while(false == BSP_FLASH_Write(table,8));
        
    //BSP_FLASH_Read(table_temp,8); 
    while(1)
    {
        AHRS_MPU6050_GetFilterData();//��ȡ�˲������ֵ
        AHRS_HMC5883L_GetFilterData();
        AHRS_IMU_Get_Angle();
        
        //DMA_DataUpdate();
        
        DMA_Update_UploadData();
   
        
        
        if (AHRS_IMU_Roll >0.0f )
            BSP_LED1_ON();
        else
            BSP_LED1_OFF();
        
        if (AHRS_IMU_Pitch >0.0f )
            BSP_LED2_ON();
        else
            BSP_LED2_OFF();
        
        
        
        
        
        if (AHRS_IMU_Yaw > 0.0f)
            BSP_LED3_ON();
        else
            BSP_LED3_OFF();
        
        
        
        countpertime ++ ;
    }
    
    
    
    
    
    
    
    
    }
        
        
        
        while(1);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//    
//    
//    //OS_ERR err;

//    __asm("CPSID I");

//    BSP_Init();
//    
//    OSInit(&err);
//    
//    OSTaskCreate(   (OS_TCB     *)&StartUp_TCB,                // ������ƿ�ָ��          
//                    (CPU_CHAR   *)"StartUp",		           // ��������
//                    (OS_TASK_PTR )Task_Start, 	               // �������ָ��
//                    (void       *)0,			               // ���ݸ�����Ĳ���parg
//                    (OS_PRIO     )STARTUP_TASK_PRIO,           // �������ȼ�
//                    (CPU_STK    *)&StartUp_Stk[0],	           // �����ջ����ַ
//                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE/10,	   // ��ջʣ�ྯ����
//                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,	   // ��ջ��С
//                    (OS_MSG_QTY  )0,			               // �ɽ��յ������Ϣ������
//                    (OS_TICK     )0,			               // ʱ��Ƭ��תʱ��
//                    (void       *)0,			               // ������ƿ���չ��Ϣ
//                    (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
//                                  OS_OPT_TASK_STK_CLR),	       // ����ѡ��
//                    (OS_ERR     *)&err);		               // ����ֵ

//  /* ����������ϵͳ������Ȩ����uC/OS-II */
//  OSStart(&err);
}

