#ifndef __AHRS_IMU_H
#define __AHRS_IMU_H

#include "ahrs.h"

extern volatile uint8_t IMU_Mode;

extern float AHRS_IMU_Pitch;
extern float AHRS_IMU_Roll;
extern float AHRS_IMU_Yaw;

void AHRS_IMU_Init(void);
void AHRS_IMU_Get_Angle(void);

#endif
