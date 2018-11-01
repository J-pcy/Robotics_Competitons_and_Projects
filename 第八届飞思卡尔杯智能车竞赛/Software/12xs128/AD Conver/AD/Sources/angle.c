#include "derivative.h"      /* derivative-specific definitions */
#include "angle.h"
#define uint unsigned int


void angle_calculate()
{
  uint az,gy;
  float R_Z;
  R_Z = 180/(Z_Max - Z_Min);
  az=AD_Measure10(0);
  gy=AD_Measure10(1);
  
  Angle_Z = (az-C_Z)*R_Z;
  Angle_G = (gy-C_Gyro)*R_Gyro;
  Angle_AG = Angle_AG+(((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;
  Angle_GG = Angle_GG+Angle_G*0.005;
}