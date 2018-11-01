#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

float A_Offset_x = 547.0693;//加速度计X轴零点偏移量
float A_Offset_y = -280.259;//加速度计Y轴零点偏移量

float A_X_Min = -15637.2;//加速度计X轴最小极值
float A_X_Max = 17077.02;//加速度计X轴最大极值
float A_Y_Min = -16362.7;//加速度计Y轴最小极值
float A_Y_Max = 16367.89;//加速度计Y轴最大极值

float R_A_X = 180/(A_X_Max - A_X_Min);//加速度计X轴比例
float R_A_Y = 180/(A_Y_Max - A_Y_Min);//加速度计Y轴比例


float Angle_A_X,Angle_A_Y,Samp_G_X,Samp_G_Y,Angle_G_X,Angle_G_Y,Angle_AG_F,Angle_AG_H;

bool blinkState = false;


void read_AD()
{

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    Angle_A_X = (ax-A_Offset_x)*R_A_X;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°
    Angle_A_Y = (ay-A_Offset_y)*R_A_Y;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°

      Serial.print(Angle_A_X); Serial.print("\t");
      Serial.println(Angle_A_Y);
}


void setup()
{
  Wire.begin();
  Serial.begin(9600);
   
  accelgyro.initialize();
 
}

void loop()
{
  
}




