#ifndef __bsp_flash_h
#define __bsp_flash_h

#include "bsp.h"

typedef struct
{
    int16_t Flash_Data_AHRS_HMC5883L_MAG_Offset_X;
    int16_t Flash_Data_AHRS_HMC5883L_MAG_Offset_Y;
    int16_t Flash_Data_AHRS_HMC5883L_MAG_Offset_Z;
}Flash_Data_TypeDef;

extern Flash_Data_TypeDef Flash_Data;

bool BSP_FLASH_Write(uint32_t *pData, uint8_t Size);

void BSP_FLASH_Read(uint32_t *pData,uint8_t Size);



#endif
