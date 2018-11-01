#include "ahrs_mpu6050.h"
#include "app.h"

int16_t AHRS_MPU6050_GYRO_Offset_X;//陀螺仪 X轴 校准数据
int16_t AHRS_MPU6050_GYRO_Offset_Y;//陀螺仪 Y轴 校准数据
int16_t AHRS_MPU6050_GYRO_Offset_Z;//陀螺仪 Z轴 校准数据

#define AHRS_MPU6050_WindowFilter_Width     10//固定窗口宽度滑动滤波数据 宽度

int16_t AHRS_MPU6050_GYRO_FIFO_X[AHRS_MPU6050_WindowFilter_Width];//陀螺仪 X轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_MPU6050_GYRO_FIFO_Y[AHRS_MPU6050_WindowFilter_Width];//陀螺仪 Y轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_MPU6050_GYRO_FIFO_Z[AHRS_MPU6050_WindowFilter_Width];//陀螺仪 Z轴 固定窗口宽度滑动滤波 FIFO 数据

int16_t AHRS_MPU6050_TEMPER_FIFO_O[AHRS_MPU6050_WindowFilter_Width];//内部温度仪 固定窗口宽度滑动滤波 FIFO 数据

int16_t AHRS_MPU6050_ACCEL_FIFO_X[AHRS_MPU6050_WindowFilter_Width];//加速度仪 X轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_MPU6050_ACCEL_FIFO_Y[AHRS_MPU6050_WindowFilter_Width];//加速度仪 Y轴 固定窗口宽度滑动滤波 FIFO 数据
int16_t AHRS_MPU6050_ACCEL_FIFO_Z[AHRS_MPU6050_WindowFilter_Width];//加速度仪 Z轴 固定窗口宽度滑动滤波 FIFO 数据

int16_t AHRS_MPU6050_ACCEL_Primitive_X;//加速度仪 原始融合 X轴 数据
int16_t AHRS_MPU6050_ACCEL_Primitive_Y;//加速度仪 原始融合 Y轴 数据
int16_t AHRS_MPU6050_ACCEL_Primitive_Z;//加速度仪 原始融合 Z轴 数据

int16_t AHRS_MPU6050_TEMPER_Primitive_O;//内部温度仪 原始融合  数据

int16_t AHRS_MPU6050_GYRO_Primitive_X;//陀螺仪 原始融合 X轴 数据
int16_t AHRS_MPU6050_GYRO_Primitive_Y;//陀螺仪 原始融合 Y轴 数据
int16_t AHRS_MPU6050_GYRO_Primitive_Z;//陀螺仪 原始融合 Z轴 数据

int16_t AHRS_MPU6050_ACCEL_Last_X;//加速度仪 融合最终 X轴 数据
int16_t AHRS_MPU6050_ACCEL_Last_Y;//加速度仪 融合最终 Y轴 数据
int16_t AHRS_MPU6050_ACCEL_Last_Z;//加速度仪 融合最终 Z轴 数据

int16_t AHRS_MPU6050_TEMPER_Last_O;//内部温度仪 融合最终  数据

int16_t AHRS_MPU6050_GYRO_Last_X;//陀螺仪 融合最终 X轴 数据
int16_t AHRS_MPU6050_GYRO_Last_Y;//陀螺仪 融合最终 Y轴 数据
int16_t AHRS_MPU6050_GYRO_Last_Z;//陀螺仪 融合最终 Z轴 数据

bool AHRS_MPU6050_FIFOFiltering_Use_Flag = false;//窗口滑动滤波是否使用

void AHRS_MPU6050_Init(void)
{
    uint8_t ID;
    uint8_t i;
    uint8_t Config[6][2] = {0x6B,0x01,0x1A,0x03,0x1B,0x18,0x1C,0x08,0x37,0x32,0x6A,0x00};
    OS_ERR err;
    uint16_t count;
    
    
    I2C_Soft_Init(1);
    
    count = 0;
    do 
    {
        
        if (count >=256)
            NVIC_SystemReset();
        else
            count++;
        I2C_Soft_ReadBytes(1,0xD0,0x75,0x01,&ID);
    }while(ID!=0x68);
    
    for (i=0; i<6; i++)
    {
        I2C_Soft_WriteBytes(1,0xD0,Config[i][0],0x01,&Config[i][1]);
        //OSTimeDlyHMSM(0, 0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);	//延时阻塞10ms
    }
    
}






void AHRS_MPU6050_ReadData(void)
{
    uint8_t temp[14];

    I2C_Soft_ReadBytes(1,0xD0,0x3B,14,temp);
    
    AHRS_MPU6050_ACCEL_Primitive_X = ((int16_t)temp[0]<<8) | temp[1];
    AHRS_MPU6050_ACCEL_Primitive_Y = ((int16_t)temp[2]<<8) | temp[3];
    AHRS_MPU6050_ACCEL_Primitive_Z = ((int16_t)temp[4]<<8) | temp[5];

    AHRS_MPU6050_TEMPER_Primitive_O = ((int16_t)temp[6]<<8) | temp[7];

    AHRS_MPU6050_GYRO_Primitive_X = ((int16_t)temp[8]<<8) | temp[9];
    AHRS_MPU6050_GYRO_Primitive_Y = ((int16_t)temp[10]<<8) | temp[11];
    AHRS_MPU6050_GYRO_Primitive_Z = ((int16_t)temp[12]<<8) | temp[13];
}



void AHRS_MPU6050_GetFilterData(void)
{
    uint8_t i;
    int32_t sum[7] = {0,0,0,0,0,0,0};
    
    AHRS_MPU6050_ReadData();
    
    if (AHRS_MPU6050_FIFOFiltering_Use_Flag == true)
    {
        for (i=0; i<AHRS_MPU6050_WindowFilter_Width-1; i++)
        {
            AHRS_MPU6050_GYRO_FIFO_X[i] = AHRS_MPU6050_GYRO_FIFO_X[i+1];
            AHRS_MPU6050_GYRO_FIFO_Y[i] = AHRS_MPU6050_GYRO_FIFO_Y[i+1];
            AHRS_MPU6050_GYRO_FIFO_Z[i] = AHRS_MPU6050_GYRO_FIFO_Z[i+1];

            AHRS_MPU6050_TEMPER_FIFO_O[i] = AHRS_MPU6050_TEMPER_FIFO_O[i+1];

            AHRS_MPU6050_ACCEL_FIFO_X[i] = AHRS_MPU6050_ACCEL_FIFO_X[i+1];
            AHRS_MPU6050_ACCEL_FIFO_Y[i] = AHRS_MPU6050_ACCEL_FIFO_Y[i+1];
            AHRS_MPU6050_ACCEL_FIFO_Z[i] = AHRS_MPU6050_ACCEL_FIFO_Z[i+1];
        }

        AHRS_MPU6050_GYRO_FIFO_X[9] = AHRS_MPU6050_ACCEL_Primitive_X;
        AHRS_MPU6050_GYRO_FIFO_Y[9] = AHRS_MPU6050_ACCEL_Primitive_Y;
        AHRS_MPU6050_GYRO_FIFO_Z[9] = AHRS_MPU6050_ACCEL_Primitive_Z;

        AHRS_MPU6050_TEMPER_FIFO_O[9] = AHRS_MPU6050_TEMPER_Primitive_O;

        AHRS_MPU6050_ACCEL_FIFO_X[9] = AHRS_MPU6050_GYRO_Primitive_X;
        AHRS_MPU6050_ACCEL_FIFO_Y[9] = AHRS_MPU6050_GYRO_Primitive_Y;
        AHRS_MPU6050_ACCEL_FIFO_Z[9] = AHRS_MPU6050_GYRO_Primitive_Z;


        for (i=0; i<10; i++)
            sum[0] += AHRS_MPU6050_GYRO_FIFO_X[i];
        for (i=0; i<10; i++)
            sum[1] += AHRS_MPU6050_GYRO_FIFO_Y[i];
        for (i=0; i<10; i++)
            sum[2] += AHRS_MPU6050_GYRO_FIFO_Z[i];
        for (i=0; i<10; i++)
            sum[3] += AHRS_MPU6050_TEMPER_FIFO_O[i];
        for (i=0; i<10; i++)
            sum[4] += AHRS_MPU6050_ACCEL_FIFO_X[i];
        for (i=0; i<10; i++)
            sum[5] += AHRS_MPU6050_ACCEL_FIFO_Y[i];
        for (i=0; i<10; i++)
            sum[6] += AHRS_MPU6050_ACCEL_FIFO_Z[i];

        AHRS_MPU6050_ACCEL_Last_X = sum[0]/10;
        AHRS_MPU6050_ACCEL_Last_Y = sum[1]/10;
        AHRS_MPU6050_ACCEL_Last_Z = sum[2]/10;

        AHRS_MPU6050_TEMPER_Last_O = sum[3]/10;

        AHRS_MPU6050_GYRO_Last_X = sum[4]/10-AHRS_MPU6050_GYRO_Offset_X;
        AHRS_MPU6050_GYRO_Last_Y = sum[5]/10-AHRS_MPU6050_GYRO_Offset_Y;
        AHRS_MPU6050_GYRO_Last_Z = sum[6]/10-AHRS_MPU6050_GYRO_Offset_Z;
        
    }
    else
    {
        AHRS_MPU6050_ACCEL_Last_X = AHRS_MPU6050_ACCEL_Primitive_X;
        AHRS_MPU6050_ACCEL_Last_Y = AHRS_MPU6050_ACCEL_Primitive_Y;
        AHRS_MPU6050_ACCEL_Last_Z = AHRS_MPU6050_ACCEL_Primitive_Z;

        AHRS_MPU6050_TEMPER_Last_O = AHRS_MPU6050_TEMPER_Primitive_O;

        AHRS_MPU6050_GYRO_Last_X = AHRS_MPU6050_GYRO_Primitive_X-AHRS_MPU6050_GYRO_Offset_X;
        AHRS_MPU6050_GYRO_Last_Y = AHRS_MPU6050_GYRO_Primitive_Y-AHRS_MPU6050_GYRO_Offset_Y;
        AHRS_MPU6050_GYRO_Last_Z = AHRS_MPU6050_GYRO_Primitive_Z-AHRS_MPU6050_GYRO_Offset_Z;
    }
}


