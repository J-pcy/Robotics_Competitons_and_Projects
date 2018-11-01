#include "user.h"

OS_TCB	StartUp_TCB;		   					                //定义任务控制块
CPU_STK	StartUp_Stk[STARTUP_TASK_STK_SIZE];                     //定义任务堆栈

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
        AHRS_MPU6050_GetFilterData();//获取滤波后的数值
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
//    OSTaskCreate(   (OS_TCB     *)&StartUp_TCB,                // 任务控制块指针          
//                    (CPU_CHAR   *)"StartUp",		           // 任务名称
//                    (OS_TASK_PTR )Task_Start, 	               // 任务代码指针
//                    (void       *)0,			               // 传递给任务的参数parg
//                    (OS_PRIO     )STARTUP_TASK_PRIO,           // 任务优先级
//                    (CPU_STK    *)&StartUp_Stk[0],	           // 任务堆栈基地址
//                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE/10,	   // 堆栈剩余警戒线
//                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,	   // 堆栈大小
//                    (OS_MSG_QTY  )0,			               // 可接收的最大消息队列数
//                    (OS_TICK     )0,			               // 时间片轮转时间
//                    (void       *)0,			               // 任务控制块扩展信息
//                    (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
//                                  OS_OPT_TASK_STK_CLR),	       // 任务选项
//                    (OS_ERR     *)&err);		               // 返回值

//  /* 启动多任务系统，控制权交给uC/OS-II */
//  OSStart(&err);
}

