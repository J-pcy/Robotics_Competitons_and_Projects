#include "bsp_led.h"


void BSP_LED_Init(void)
{
    GPIO_InitTypeDef Struct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    Struct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&Struct);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_13);
    GPIO_SetBits(GPIOB,GPIO_Pin_14);
    GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void BSP_LED1_Toggle(void)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)));
}

void BSP_LED2_Toggle(void)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)));
}

void BSP_LED3_Toggle(void)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)(1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)));
}

void BSP_LED1_ON(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

void BSP_LED2_ON(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void BSP_LED3_ON(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void BSP_LED1_OFF(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

void BSP_LED2_OFF(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void BSP_LED3_OFF(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

