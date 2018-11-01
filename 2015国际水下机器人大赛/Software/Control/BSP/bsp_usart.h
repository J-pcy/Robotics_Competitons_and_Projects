#ifndef __bsp_usart_h
#define __bsp_usart_h


#include "bsp.h"


typedef struct//32B   除 data 7B
{
    volatile uint8_t     _00_DataHead1;//帧头1
    volatile uint8_t     _01_DataHead2;//帧头2
    volatile uint8_t     _02_Function;//功能帧
    volatile float       _03_Pitch;
    volatile float       _04_Roll;
    volatile float       _05_Yaw;
    volatile int16_t     _06_Ax;
    volatile int16_t     _07_Ay;
    volatile int16_t     _08_Az;
    volatile int16_t     _09_Gx;
    volatile int16_t     _10_Gy;
    volatile int16_t     _11_Gz;
    volatile int16_t     _12_Mx;
    volatile int16_t     _13_My;
    volatile int16_t     _14_Mz;
    volatile uint16_t    _15_Speed;
    volatile bool        _16_LED1;
    volatile bool        _17_LED2;
    volatile bool        _18_LED3;
    volatile uint8_t     _19_CRC1;
    volatile uint8_t     _20_CRC2;
    volatile uint8_t     _21_DataTail1;
    volatile uint8_t     _22_DataTail2;
}UploadData_Type;


extern UploadData_Type UploadData;
extern UploadData_Type *pUploadData;




uint16_t cal_CRC(uint8_t *ptr,uint8_t len);

void BSP_USART1_Init(void);

void DMA_DataUpdate(void);

void DMA_DataUpdate(void);



void DMA_Update_UploadData(void);



#endif

