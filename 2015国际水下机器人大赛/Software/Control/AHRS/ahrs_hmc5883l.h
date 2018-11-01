#ifndef __AHRS_HMC5883L_H
#define __AHRS_HMC5883L_H

#include "ahrs.h"

#include "stdbool.h"


extern int16_t AHRS_HMC5883L_MAG_Temp_X;//电子罗盘 融合最终 X轴 数据
extern int16_t AHRS_HMC5883L_MAG_Temp_Y;//电子罗盘 融合最终 Y轴 数据
extern int16_t AHRS_HMC5883L_MAG_Temp_Z;//电子罗盘 融合最终 Z轴 数据

extern float AHRS_HMC5883L_MAG_Last_X;//电子罗盘 融合最终 X轴 数据
extern float AHRS_HMC5883L_MAG_Last_Y;//电子罗盘 融合最终 Y轴 数据
extern float AHRS_HMC5883L_MAG_Last_Z;//电子罗盘 融合最终 Z轴 数据

extern bool AHRS_HMC5883L_MAG_Calibrate;//校准标识

extern bool AHRS_HMC5883L_MAG_CalibrateSaving;//校准标识

void AHRS_HMC5883L_Init(void);
void AHRS_HMC5883L_GetFilterData(void);

void AHRS_HMC5883L_Calibrate_Saving(void);
void AHRS_HMC5883L_Calibrate_Read(void);

#endif
