#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;

#define RunTime 23

#define E1_A 31
#define E2_A 33
#define E1_B 35
#define E2_B 37
#define E1_C 39
#define E2_C 41
#define E1_D 43
#define E2_D 45

#define PWM1 4
#define PWM2 5
#define PWM3 6
#define PWM4 7

#define Angle_set_x 0
#define Angle_set_y 0
#define Angle_Speed_set 0

#define speed_set 125

#define Moto_Out_Max 255
#define Moto_Out_Min 0

int16_t ax, ay, az;
int16_t gx, gy, gz;

float A_Offset_x = 547.0693;//加速度计X轴零点偏移量
float A_Offset_y = -280.259;//加速度计Y轴零点偏移量

float G_Offset_x = 149.661;//陀螺仪X轴零点偏移量
float G_Offset_y = 371.7781;//陀螺仪Y轴零点偏移量

float A_X_Min = -15637.2;//加速度计X轴最小极值
float A_X_Max = 17077.02;//加速度计X轴最大极值
float A_Y_Min = -16362.7;//加速度计Y轴最小极值
float A_Y_Max = 16367.89;//加速度计Y轴最大极值
float T_A = 3;//加速度计X.Y轴角度补偿时间常数
float R_A_X = 180/(A_X_Max - A_X_Min);//加速度计X轴比例
float R_A_Y = 180/(A_Y_Max - A_Y_Min);//加速度计Y轴比例

float R_G_X = 0.281;//陀螺仪X轴比例
float R_G_Y = 0.281;//陀螺仪Y轴比例

unsigned long T_Now =0;//系统当前时间
unsigned long T_Last;//上次时间

float Angle_A_X,Angle_A_Y,Samp_G_X,Samp_G_Y,Angle_G_X,Angle_G_Y,Angle_AG_F,Angle_AG_H;

bool blinkState = false;

static float xAngle_Control_P=20;
static float xAngle_Control_D=2;
static float yAngle_Control_P=20;
static float yAngle_Control_D=2;
static float g_fAngleControlOut_x;
static float g_fAngleControlOut_y;
static float FrontB_MotorOut;
static float LeftR_MotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

float OutData[4]={0};//虚拟示波器4通道参数

long previousMillis = 0;//过去的时间
long interval = 1;//间隔

char timee=0;








unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
    Serial.write(databuf[i]);
}

void read_AD()
{

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//    Serial.print("a/g:\t");
//    Serial.print(ax/16384); Serial.print("\t");  
//    Serial.print(ay/16384); Serial.print("\t"); 
//    Serial.println(gx/131); Serial.print("\t"); 
//    Serial.println(gy/131);

    Angle_A_X = (ax-A_Offset_x)*R_A_X;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°
    Angle_A_Y = (ay-A_Offset_y)*R_A_Y;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°
    Samp_G_X = (gx-G_Offset_x)*R_G_X;//陀螺仪采样 （采样值-偏移量）*比例 单位：°/s
    Samp_G_Y = (G_Offset_y-gy)*R_G_Y;//陀螺仪采样 （采样值-偏移量）*比例 单位：°/s
    Angle_AG_F = Angle_AG_F + (((Angle_A_X-Angle_AG_F)*1/T_A)+Samp_G_Y)*0.005;//滤波
    Angle_AG_H = Angle_AG_H + (((Angle_A_Y-Angle_AG_H)*1/T_A)+Samp_G_X)*0.005;//滤波
    
    Angle_G_X = Angle_G_X + Samp_G_X*0.005;//陀螺仪对X轴积分 得出角度。
    Angle_G_Y = Angle_G_Y + Samp_G_Y*0.005;//陀螺仪对X轴积分 得出角度。
    
       OutData[0]=Angle_A_X;
       OutData[1]=Angle_G_Y;
       OutData[2]=Angle_AG_F;
       OutPut_Data();
       
//       OutData[0]=Angle_A_Y;
//       OutData[1]=Angle_G_X;
//       OutData[2]=Angle_AG_H;
//      OutPut_Data();
}

void AngleControl(void)
{
  float fValue_x,fValue_y;
  fValue_x = (Angle_set_x - Angle_AG_F) * xAngle_Control_P + (Angle_Speed_set - Samp_G_Y) * xAngle_Control_D;
  fValue_y = (Angle_set_y - Angle_AG_H) * yAngle_Control_P + (Angle_Speed_set - Samp_G_X) * yAngle_Control_D;
//输出限幅
//  if(f_Value > Angle_Control_Out_Max)
//   f_Value = Angle_Control_Out_Max;
//  if(f_Value < Angle_Control_Out_Min)
//    f_Value = Angle_Control_Out_Min;

  g_fAngleControlOut_x = -fValue_x;
  g_fAngleControlOut_y = -fValue_y;
//  Serial.print(g_fAngleControlOut_x);Serial.print("\t");
//  Serial.println(g_fAngleControlOut_y);
}

void MotorOutput()
{ 
  FrontB_MotorOut = g_fAngleControlOut_x;
  LeftR_MotorOut = g_fAngleControlOut_y;
  MotorSpeedOut();
//  Serial.println(g_fAngleControlOut);
}

void MotorSpeedOut(void)
{
  float speed_1_Val,speed_2_Val,speed_3_Val,speed_4_Val;
  
//  if(FrontB_MotorOut > Moto_Out_Max)
//    FrontB_MotorOut = Moto_Out_Max;
//  if(FrontB_MotorOut < Moto_Out_Min)
//    FrontB_MotorOut = Moto_Out_Min;   
//  if(LeftR_MotorOut > Moto_Out_Max)
//    LeftR_MotorOut = Moto_Out_Max;
//  if(LeftR_MotorOut < Moto_Out_Min)
//    LeftR_MotorOut = Moto_Out_Min;
    
  speed_1_Val = speed_set-FrontB_MotorOut-LeftR_MotorOut;
  speed_2_Val = speed_set-FrontB_MotorOut+LeftR_MotorOut;
  speed_3_Val = speed_set+FrontB_MotorOut-LeftR_MotorOut;
  speed_4_Val = speed_set+FrontB_MotorOut+LeftR_MotorOut;
  
//输出限幅    
  if(speed_1_Val > Moto_Out_Max)
    speed_1_Val = Moto_Out_Max;
  if(speed_1_Val < Moto_Out_Min)
    speed_1_Val = Moto_Out_Min;   
  if(speed_2_Val > Moto_Out_Max)
    speed_2_Val = Moto_Out_Max;
  if(speed_2_Val < Moto_Out_Min)
    speed_2_Val = Moto_Out_Min;
  if(speed_3_Val > Moto_Out_Max)
    speed_3_Val = Moto_Out_Max;
  if(speed_3_Val < Moto_Out_Min)
    speed_3_Val = Moto_Out_Min;   
  if(speed_4_Val > Moto_Out_Max)
    speed_4_Val = Moto_Out_Max;
  if(speed_4_Val < Moto_Out_Min)
    speed_4_Val = Moto_Out_Min;
  
//死区常数
/*  if(fLeftVal > 0)
    fLeftVal += Motor_Out_Dead_Val;
  else if(fLeftVal < 0)
    fLeftVal -= Motor_Out_Dead_Val;

  if(fRightVal > 0)
    fRightVal += Motor_Out_Dead_Val;
  else if(fRightVal < 0)
    fRightVal -= Motor_Out_Dead_Val;*/  
//    SetMotorVoltage(fLeftVal,fRightVal);

  Moto_out_1((int)(speed_1_Val));
  Moto_out_2((int)(speed_2_Val));
  Moto_out_3((int)(speed_3_Val));
  Moto_out_4((int)(speed_4_Val));
  
    Serial.print(speed_1_Val); Serial.print("\t");
    Serial.print(speed_2_Val); Serial.print("\t");
    Serial.print(speed_3_Val); Serial.print("\t");
    Serial.println(speed_4_Val);  
}

void Moto_out_1(unsigned int speed_m)
{
  digitalWrite (E1_A,0);
  digitalWrite (E2_A,1);
  analogWrite (PWM1,speed_m);  
}

void Moto_out_2(unsigned int speed_m)
{
  digitalWrite (E1_B,0);
  digitalWrite (E2_B,1);
  analogWrite (PWM2,speed_m);  
}

void Moto_out_3(unsigned int speed_m)
{
  digitalWrite (E1_C,0);
  digitalWrite (E2_C,1);
  analogWrite (PWM3,speed_m);  
}

void Moto_out_4(unsigned int speed_m)
{
  digitalWrite (E1_D,0);
  digitalWrite (E2_D,1);
  analogWrite (PWM4,speed_m);  
}

void TMR0_IRQHandler(void)   //1毫秒定时程序
{
        timee++;
//        PWM_output_line();
        MotorOutput();
        if(timee==5)
        {
          timee=0;
//          MotoR(1,90);
//          MotoL(0,90);
        }
	else if(timee==1)//读取传感器值、取平均值
	{	
              digitalWrite(RunTime,HIGH);
              read_AD();
              digitalWrite(RunTime,LOW);
	}
	else if(timee==3)//获取电机速度，计算电机控制量
	{
              digitalWrite(RunTime,HIGH);
              AngleControl();
              digitalWrite(RunTime,LOW);


	}
//	else if(timee==4)//获取电机速度，计算电机控制量
//	{
//              digitalWrite(RunTime,HIGH);
//              AngleControl();
//             digitalWrite(RunTime,LOW);
//	} 
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
   
  accelgyro.initialize();

  pinMode(RunTime,OUTPUT);
  pinMode(E1_A,OUTPUT);
  pinMode(E2_A,OUTPUT);
  pinMode(E1_B,OUTPUT);
  pinMode(E2_B,OUTPUT);
  pinMode(E1_C,OUTPUT);
  pinMode(E2_C,OUTPUT);
  pinMode(E1_D,OUTPUT);
  pinMode(E2_D,OUTPUT);  
}

void loop()
{

    unsigned long currentMillis = millis();//读取当前时间值
    if(currentMillis - previousMillis >= interval) 
    {
        previousMillis = currentMillis;  
        TMR0_IRQHandler();
        
    }
    blinkState = !blinkState;
}




