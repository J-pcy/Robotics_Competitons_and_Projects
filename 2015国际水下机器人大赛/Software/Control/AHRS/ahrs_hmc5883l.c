#include "ahrs_hmc5883l.h"
#include "app.h"


//	191	-181
//	101	-197
//	166	-143


//==============================================================================
//														第一块板子 校准值
//==============================================================================
//int16_t AHRS_HMC5883L_MAG_Offset_X = ((int16_t) (190 - 189) / 2);//电子罗盘 X轴 校准数据
//int16_t AHRS_HMC5883L_MAG_Offset_Y = ((int16_t) (125 - 178) / 2);//电子罗盘 Y轴 校准数据
//int16_t AHRS_HMC5883L_MAG_Offset_Z = ((int16_t) (155 - 126) / 2);//电子罗盘 Z轴 校准数据
//134  -200   0xFFDF(-33)
//106  -181   0xFFDB(-37)
//137  -135   0x0001(1)


//==============================================================================
//														第二块板子 校准值
//==============================================================================
int16_t AHRS_HMC5883L_MAG_Offset_X = ((int16_t) (191	-181) / 2);//电子罗盘 X轴 校准数据
int16_t AHRS_HMC5883L_MAG_Offset_Y = ((int16_t) (101	-197) / 2);//电子罗盘 Y轴 校准数据
int16_t AHRS_HMC5883L_MAG_Offset_Z = ((int16_t) (166	-143) / 2);//电子罗盘 Z轴 校准数据











float AHRS_HMC5883L_MAG_Scale_X = 1.0f;//电子罗盘 X轴 比例数据
float AHRS_HMC5883L_MAG_Scale_Y = 1.0f;//电子罗盘 Y轴 比例数据
float AHRS_HMC5883L_MAG_Scale_Z = 1.0f;//电子罗盘 Z轴 比例数据

int16_t AHRS_HMC5883L_MAG_Max_X = -4096;//电子罗盘 X轴 最大标定值数据
int16_t AHRS_HMC5883L_MAG_Max_Y = -4096;//电子罗盘 Y轴 最大标定值数据
int16_t AHRS_HMC5883L_MAG_Max_Z = -4096;//电子罗盘 Z轴 最大标定值数据

int16_t AHRS_HMC5883L_MAG_Min_X = +4096;//电子罗盘 X轴 最小标定值数据
int16_t AHRS_HMC5883L_MAG_Min_Y = +4096;//电子罗盘 Y轴 最小标定值数据
int16_t AHRS_HMC5883L_MAG_Min_Z = +4096;//电子罗盘 Z轴 最小标定值数据

#define AHRS_HMC5883L_WindowFilter_Width     10//固定窗口宽度滑动滤波数据 宽度

int16_t AHRS_HMC5883L_MAG_FIFO_X[AHRS_HMC5883L_WindowFilter_Width];//电子罗盘 X轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_HMC5883L_MAG_FIFO_Y[AHRS_HMC5883L_WindowFilter_Width];//电子罗盘 Y轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_HMC5883L_MAG_FIFO_Z[AHRS_HMC5883L_WindowFilter_Width];//电子罗盘 Z轴 固定窗口宽度滑动滤波 FIFO 数据

int16_t AHRS_HMC5883L_MAG_Primitive_X;//电子罗盘 原始融合 X轴 数据
int16_t AHRS_HMC5883L_MAG_Primitive_Y;//电子罗盘 原始融合 Y轴 数据
int16_t AHRS_HMC5883L_MAG_Primitive_Z;//电子罗盘 原始融合 Z轴 数据

int16_t AHRS_HMC5883L_MAG_Temp_X;//电子罗盘 融合最终 X轴 数据
int16_t AHRS_HMC5883L_MAG_Temp_Y;//电子罗盘 融合最终 Y轴 数据
int16_t AHRS_HMC5883L_MAG_Temp_Z;//电子罗盘 融合最终 Z轴 数据

float AHRS_HMC5883L_MAG_Last_X;//电子罗盘 融合最终 X轴 数据
float AHRS_HMC5883L_MAG_Last_Y;//电子罗盘 融合最终 Y轴 数据
float AHRS_HMC5883L_MAG_Last_Z;//电子罗盘 融合最终 Z轴 数据

bool AHRS_HMC5883L_MAG_Calibrate = false;//校准标识

bool AHRS_HMC5883L_MAG_CalibrateSaving = false;//校准标识

bool AHRS_HMC5883L_MAG_FIFOFiltering_Use_Flag = false;//是否使用滑动滤波

void AHRS_HMC5883L_Init(void)
{
    uint8_t ID[3];
    uint8_t i;
    uint8_t Config[3][2] = {0x00,0x70,0x01,0xA0,0x02,0x00};
    OS_ERR err;
    uint16_t count;
    
    I2C_Soft_Init(2);
    
    count = 0;
    
    do 
    {
        if (count >=256)
            NVIC_SystemReset();
        else
            count++;
        
        I2C_Soft_ReadBytes(2,0x3C,0x0A,0x03,ID);
    }while(ID[0]!= 'H' || ID[1] != '4' || ID[2] != '3');
    
    for (i=0; i<3; i++)
    {
        I2C_Soft_WriteBytes(2,0x3C,Config[i][0],0x01,&Config[i][1]);
        //OSTimeDlyHMSM(0, 0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);	//延时阻塞10ms
    }
}



void AHRS_HMC5883L_ReadData(void)
{
    uint8_t temp[6];

    I2C_Soft_ReadBytes(2,0x3C,0x03,6,temp);
    
    AHRS_HMC5883L_MAG_Primitive_X = ((int16_t)temp[0]<<8) | temp[1];
    AHRS_HMC5883L_MAG_Primitive_Y = ((int16_t)temp[4]<<8) | temp[5];
    AHRS_HMC5883L_MAG_Primitive_Z = ((int16_t)temp[2]<<8) | temp[3];

}


void AHRS_HMC5883L_GetFilterData(void)
{
    uint8_t i;
    int32_t sum[7] = {0,0,0,0,0,0,0};
    
    AHRS_HMC5883L_ReadData();
    
    if (AHRS_HMC5883L_MAG_FIFOFiltering_Use_Flag == true)
    {
    
        for (i=0; i<AHRS_HMC5883L_WindowFilter_Width-1; i++)
        {
            AHRS_HMC5883L_MAG_FIFO_X[i] = AHRS_HMC5883L_MAG_FIFO_X[i+1];
            AHRS_HMC5883L_MAG_FIFO_Y[i] = AHRS_HMC5883L_MAG_FIFO_Y[i+1];
            AHRS_HMC5883L_MAG_FIFO_Z[i] = AHRS_HMC5883L_MAG_FIFO_Z[i+1];
        }

        AHRS_HMC5883L_MAG_FIFO_X[9] = AHRS_HMC5883L_MAG_Primitive_X;
        AHRS_HMC5883L_MAG_FIFO_Y[9] = AHRS_HMC5883L_MAG_Primitive_Y;
        AHRS_HMC5883L_MAG_FIFO_Z[9] = AHRS_HMC5883L_MAG_Primitive_Z;

        for (i=0; i<10; i++)
            sum[0] += AHRS_HMC5883L_MAG_FIFO_X[i];
        for (i=0; i<10; i++)
            sum[1] += AHRS_HMC5883L_MAG_FIFO_Y[i];
        for (i=0; i<10; i++)
            sum[2] += AHRS_HMC5883L_MAG_FIFO_Z[i];

        AHRS_HMC5883L_MAG_Temp_X = sum[0]/10;
        AHRS_HMC5883L_MAG_Temp_Y = sum[1]/10;
        AHRS_HMC5883L_MAG_Temp_Z = sum[2]/10;
    }
    else
    {
        AHRS_HMC5883L_MAG_Temp_X = AHRS_HMC5883L_MAG_Primitive_X;
        AHRS_HMC5883L_MAG_Temp_Y = AHRS_HMC5883L_MAG_Primitive_Y;
        AHRS_HMC5883L_MAG_Temp_Z = AHRS_HMC5883L_MAG_Primitive_Z;
    }
    
    
    
    if (AHRS_HMC5883L_MAG_Calibrate == true)
    {
        if(AHRS_HMC5883L_MAG_Min_X>AHRS_HMC5883L_MAG_Temp_X)AHRS_HMC5883L_MAG_Min_X=(int16_t)AHRS_HMC5883L_MAG_Temp_X;
				if(AHRS_HMC5883L_MAG_Min_Y>AHRS_HMC5883L_MAG_Temp_Y)AHRS_HMC5883L_MAG_Min_Y=(int16_t)AHRS_HMC5883L_MAG_Temp_Y;
				if(AHRS_HMC5883L_MAG_Min_Z>AHRS_HMC5883L_MAG_Temp_Z)AHRS_HMC5883L_MAG_Min_Z=(int16_t)AHRS_HMC5883L_MAG_Temp_Z;

				if(AHRS_HMC5883L_MAG_Max_X<AHRS_HMC5883L_MAG_Temp_X)AHRS_HMC5883L_MAG_Max_X=(int16_t)AHRS_HMC5883L_MAG_Temp_X;
				if(AHRS_HMC5883L_MAG_Max_Y<AHRS_HMC5883L_MAG_Temp_Y)AHRS_HMC5883L_MAG_Max_Y=(int16_t)AHRS_HMC5883L_MAG_Temp_Y;
				if(AHRS_HMC5883L_MAG_Max_Z<AHRS_HMC5883L_MAG_Temp_Z)AHRS_HMC5883L_MAG_Max_Z=(int16_t)AHRS_HMC5883L_MAG_Temp_Z;
    }
    
    if (AHRS_HMC5883L_MAG_CalibrateSaving == true)
    {
        AHRS_HMC5883L_MAG_Calibrate = false;
        AHRS_HMC5883L_MAG_CalibrateSaving = false;
        
        AHRS_HMC5883L_Calibrate_Saving();
    }
    
    AHRS_HMC5883L_MAG_Last_X = ((float)(AHRS_HMC5883L_MAG_Temp_X - AHRS_HMC5883L_MAG_Offset_X)) * AHRS_HMC5883L_MAG_Scale_X;
    AHRS_HMC5883L_MAG_Last_Y = ((float)(AHRS_HMC5883L_MAG_Temp_Y - AHRS_HMC5883L_MAG_Offset_Y)) * AHRS_HMC5883L_MAG_Scale_Y;
    AHRS_HMC5883L_MAG_Last_Z = ((float)(AHRS_HMC5883L_MAG_Temp_Z - AHRS_HMC5883L_MAG_Offset_Z)) * AHRS_HMC5883L_MAG_Scale_Z;
}





void AHRS_HMC5883L_Calibrate_Saving(void)
{
    uint32_t data[3];
    
    AHRS_HMC5883L_MAG_Offset_X = (AHRS_HMC5883L_MAG_Min_X + AHRS_HMC5883L_MAG_Max_X) / 2;
    AHRS_HMC5883L_MAG_Offset_Y = (AHRS_HMC5883L_MAG_Min_Y + AHRS_HMC5883L_MAG_Max_Y) / 2;
    AHRS_HMC5883L_MAG_Offset_Z = (AHRS_HMC5883L_MAG_Min_Z + AHRS_HMC5883L_MAG_Max_Z) / 2;
    
    

    data[0] = Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_X = AHRS_HMC5883L_MAG_Offset_X;
    data[1] = Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_Y = AHRS_HMC5883L_MAG_Offset_Y;
    data[2] = Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_Z = AHRS_HMC5883L_MAG_Offset_Z;
    
    while (BSP_FLASH_Write(data,3) == false);
}





void AHRS_HMC5883L_Calibrate_Read(void)
{
    uint32_t data[3];
    
    BSP_FLASH_Read(data,3);
    
    Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_X = AHRS_HMC5883L_MAG_Offset_X = (int16_t) data[0];
    Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_Y = AHRS_HMC5883L_MAG_Offset_Y = (int16_t) data[1];
    Flash_Data.Flash_Data_AHRS_HMC5883L_MAG_Offset_Z = AHRS_HMC5883L_MAG_Offset_Z = (int16_t) data[2];
    

}

