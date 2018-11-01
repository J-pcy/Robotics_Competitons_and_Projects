#ifndef __AHRS_MPU6050_H
#define __AHRS_MPU6050_H

#include "ahrs.h"

extern int16_t AHRS_MPU6050_ACCEL_Last_X;//���ٶ��� �ں����� X�� ����
extern int16_t AHRS_MPU6050_ACCEL_Last_Y;//���ٶ��� �ں����� Y�� ����
extern int16_t AHRS_MPU6050_ACCEL_Last_Z;//���ٶ��� �ں����� Z�� ����

extern int16_t AHRS_MPU6050_TEMPER_Last_O;//�ڲ��¶��� �ں�����  ����

extern int16_t AHRS_MPU6050_GYRO_Last_X;//������ �ں����� X�� ����
extern int16_t AHRS_MPU6050_GYRO_Last_Y;//������ �ں����� Y�� ����
extern int16_t AHRS_MPU6050_GYRO_Last_Z;//������ �ں����� Z�� ����

void AHRS_MPU6050_Init(void);//��������ʼ��

void AHRS_MPU6050_GetFilterData(void);//�̶����ڻ����˲���Ĵ�����ֵ

#endif
