#include "bsp_timer4.h"

void BSP_Timer4_Init(void)
{
    TIM_TimeBaseInitTypeDef Struct1;
    NVIC_InitTypeDef NVIC_Struct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    
    TIM_TimeBaseStructInit(&Struct1); 
    
    Struct1.TIM_ClockDivision = TIM_CKD_DIV1;
    Struct1.TIM_CounterMode = TIM_CounterMode_Up;
    Struct1.TIM_Period = 0xffff;
    Struct1.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM1,&Struct1);
    Struct1.TIM_Prescaler = 71;
    TIM_TimeBaseInit(TIM4,&Struct1);
    
    
    TIM_PrescalerConfig(TIM1, 0, TIM_PSCReloadMode_Update);
    TIM_UpdateDisableConfig(TIM1, ENABLE);
    TIM_SelectInputTrigger(TIM1, TIM_TS_ITR3);
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_External1);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);	
    
    TIM_TimeBaseInit(TIM4, &Struct1);
  	TIM_ARRPreloadConfig(TIM4, ENABLE);	
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Regular);
  	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);
  	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);

    
    TIM_TimeBaseStructInit(&Struct1); 
    
    Struct1.TIM_ClockDivision = TIM_CKD_DIV1;
    Struct1.TIM_CounterMode = TIM_CounterMode_Up;
    Struct1.TIM_Period = 9999;
    Struct1.TIM_Prescaler = 7200-1;
    TIM_TimeBaseInit(TIM3,&Struct1);
    
    NVIC_Struct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_Struct);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    
    
    

    
    TIM_Cmd(TIM1, ENABLE); 
  	TIM_Cmd(TIM4, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    
}

uint32_t micros(void)
{
 	uint32_t temp=0 ;
 	temp = TIM1->CNT; //读高16位时间
 	temp = temp<<16;
 	temp += TIM4->CNT; //读低16位时间
 	return temp;
}
