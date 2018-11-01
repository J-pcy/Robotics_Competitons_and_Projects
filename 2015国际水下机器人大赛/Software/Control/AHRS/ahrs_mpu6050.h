#ifndef __AHRS_MPU6050_H
#define __AHRS_MPU6050_H

#include "ahrs.h"

extern int16_t AHRS_MPU6050_ACCEL_Last_X;//加速度仪 融合最终 X轴 数据
extern int16_t AHRS_MPU6050_ACCEL_Last_Y;//加速度仪 融合最终 Y轴 数据
extern int16_t AHRS_MPU6050_ACCEL_Last_Z;//加速度仪 融合最终 Z轴 数据

extern int16_t AHRS_MPU6050_TEMPER_Last_O;//内部温度仪 融合最终  数据

extern int16_t AHRS_MPU6050_GYRO_Last_X;//陀螺仪 融合最终 X轴 数据
extern int16_t AHRS_MPU6050_GYRO_Last_Y;//陀螺仪 融合最终 Y轴 数据
extern int16_t AHRS_MPU6050_GYRO_Last_Z;//陀螺仪 融合最终 Z轴 数据

void AHRS_MPU6050_Init(void);//传感器初始化

void AHRS_MPU6050_GetFilterData(void);//固定窗口滑动滤波后的传感器值

#endif
