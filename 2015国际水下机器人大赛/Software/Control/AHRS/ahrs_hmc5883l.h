#ifndef __AHRS_HMC5883L_H
#define __AHRS_HMC5883L_H

#include "ahrs.h"

#include "stdbool.h"


extern int16_t AHRS_HMC5883L_MAG_Temp_X;//�������� �ں����� X�� ����
extern int16_t AHRS_HMC5883L_MAG_Temp_Y;//�������� �ں����� Y�� ����
extern int16_t AHRS_HMC5883L_MAG_Temp_Z;//�������� �ں����� Z�� ����

extern float AHRS_HMC5883L_MAG_Last_X;//�������� �ں����� X�� ����
extern float AHRS_HMC5883L_MAG_Last_Y;//�������� �ں����� Y�� ����
extern float AHRS_HMC5883L_MAG_Last_Z;//�������� �ں����� Z�� ����

extern bool AHRS_HMC5883L_MAG_Calibrate;//У׼��ʶ

extern bool AHRS_HMC5883L_MAG_CalibrateSaving;//У׼��ʶ

void AHRS_HMC5883L_Init(void);
void AHRS_HMC5883L_GetFilterData(void);

void AHRS_HMC5883L_Calibrate_Saving(void);
void AHRS_HMC5883L_Calibrate_Read(void);

#endif
