#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#include "ADC.h"
#include "PWM.h"
#include "PLL.h"
#define uint unsigned int
#define uchar unsigned char

#define car_Angle_set 0
#define Car_Angle_Speed_set 0
#define Moto_Out_Max 100
#define Moto_Out_Min -100

float C_Z = 303.604;
float C_Gyro = 347.999;
float Z_Min = 147.816; 
float Z_Max = 474.205;
float T_Z = 3;
float R_Z = 0.5515;
float R_Gyro = 4.3;
float Angle_Z,Angle_G,Angle_AG,Angle_GG;

float OutData[4]={0};//虚拟示波器4通道参数

static float Angle_Control_P=11;
static float Angle_Control_D=0.3;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

char timee=0;


void IO_init(void)
{
  DDRH=0x0f;  //开启PH口的0123位
}
   
    

void MotoR(unsigned char com,unsigned char speed_m)//占空比调节范围为0-100
{
  if(com==0)		//左电机正转		 		
	{
	PTH_PTH0=1;
	PTH_PTH2=0;
	PWMOUT_R(speed_m);
	}
	else if(com==1) //左电机反转
	{
   PTH_PTH0=0;
	 PTH_PTH2=1;
   PWMOUT_R(speed_m);
	}
}


void MotoL(unsigned char com,unsigned char speed_m)
{


	if(com==0)		//右电机正转		 		
	{ 
  PTH_PTH1=0;
	PTH_PTH3=1;
  PWMOUT_L(speed_m);
	}
	else if(com==1) //右电机反转
 {
  PTH_PTH1=1;
	PTH_PTH3=0;
	PWMOUT_L(speed_m);
	}
}

/*unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++)
    {      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++)
         {
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
  Uart_putchar(databuf[i]);
} */



void angle_calculate()
{
  uint az,gy;
//  float R_Z = 180/(Z_Max - Z_Min);
  az=AD_Measure10(7);
  gy=AD_Measure10(6);
  
  Angle_Z = (az-C_Z)*R_Z;
  Angle_G = (gy-C_Gyro)*R_Gyro;
  Angle_AG = Angle_AG+(((Angle_Z-Angle_AG)*(1/T_Z))+Angle_G)*0.005;
//  Angle_GG = Angle_GG+Angle_G*0.005;
  Uart_output(Angle_AG);
//  OutData[0]=Angle_Z;
//  OutData[1]=Angle_GG;
//  OutData[2]=Angle_AG;
//  OutPut_Data();  
}

void AngleControl(void)
{
  float fValue;
  fValue = (car_Angle_set - Angle_AG) * Angle_Control_P + (Car_Angle_Speed_set - Angle_G) * Angle_Control_D;

//输出限幅
//  if(f_Value > Angle_Control_Out_Max)
//   f_Value = Angle_Control_Out_Max;
//  if(f_Value < Angle_Control_Out_Min)
//    f_Value = Angle_Control_Out_Min;

  g_fAngleControlOut = -fValue;
}

void MotorSpeedOut(void)
{
  float fLeftVal,fRightVal;
  fLeftVal = g_fLeftMotorOut;
  fRightVal = g_fRightMotorOut;
  
//死区常数
/*  if(fLeftVal > 0)
    fLeftVal += Motor_Out_Dead_Val;
  else if(fLeftVal < 0)
    fLeftVal -= Motor_Out_Dead_Val;

  if(fRightVal > 0)
    fRightVal += Motor_Out_Dead_Val;
  else if(fRightVal < 0)
    fRightVal -= Motor_Out_Dead_Val;*/
 
//输出限幅    
  if(fLeftVal > Moto_Out_Max)
    fLeftVal = Moto_Out_Max;
  if(fLeftVal < Moto_Out_Min)
    fLeftVal = Moto_Out_Min;   
  if(fRightVal > Moto_Out_Max)
    fRightVal = Moto_Out_Max;
  if(fRightVal < Moto_Out_Min)
    fRightVal = Moto_Out_Min;
    
  if(fLeftVal>0)
    MotoL(0,(int)(fLeftVal));
  else if(fLeftVal<0)
  {
    fLeftVal=fLeftVal*(-1);
    MotoL(1,(int)(fLeftVal));	
  }


  if(fRightVal>0)
    MotoR(0,(int)(fRightVal));
  else if(fRightVal<0)
  {
    fRightVal=fRightVal*(-1);
    MotoR(1,(int)(fRightVal));	
  }

}

void MotorOutput()
{
  float fLeft,fRight;
  
  fLeft = g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
  fRight = g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
  
  g_fLeftMotorOut = fLeft;
  g_fRightMotorOut = fRight;
  MotorSpeedOut();
}

void main(void) 
{
  /* put your own code here */
  PLL_init_80M();//设置锁相环的时钟为48MHz 总线频率为24MHz
  IO_init();
  PWM_init();
  AD_Init();
  SCI0_init();
	EnableInterrupts;
  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    timee++;
//    PWM_output_line();
    MotorOutput();
    if(timee==5)
    {
      timee=0;
    }
  	else if(timee==1)//读取传感器值、取平均值
	  {	
//      digitalWrite(RunTime,HIGH);
      angle_calculate();
//      digitalWrite(RunTime,LOW);
  	}
	  else if(timee==4)//获取电机速度，计算电机控制量
  	{
//      digitalWrite(RunTime,HIGH);
      AngleControl();
//      digitalWrite(RunTime,LOW);
	  }  

  } /* loop forever */
  /* please make sure that you never leave main */
}

