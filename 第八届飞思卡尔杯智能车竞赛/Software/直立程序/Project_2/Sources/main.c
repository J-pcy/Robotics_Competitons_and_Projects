#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "LQ12864.h"
#include "Uart0.h"
#include "ADC.h"
#include "PWM.h"
#include "PLL.h"


#define uint unsigned int
#define uchar unsigned char

#define car_Angle_set 0
#define Car_speed_set 0
#define Car_Angle_Speed_set 0
#define Moto_Out_Max 100
#define Moto_Out_Min -100

#define Optical_encode_constant 100  //光电编码盘的刻槽数量
#define Speed_contro_period 100  //速度控制周期，ms
#define Car_speed_constant 1000.0/Speed_contro_period/Optical_encode_constant  //单位转化比例值

uint az,gx;
int g_nSpeedControlPeriod=100;

float C_Z = 303.604;
float C_Gyro = 413;
float Z_Min = 147.816; 
float Z_Max = 474.205;
float T_Z = 3;
float R_Z = 0.5515;
float R_Gyro = 4.8;
float Angle_Z,Angle_G,Angle_AG,Angle_GG;
float g_fCarSpeed,g_fSpeedControlIntegral,g_fSpeedControlOutOld,g_fSpeedControlOutNew;

float OutData[4]={0};//虚拟示波器4通道参数

static float Angle_Control_P=11;
static float Angle_Control_D=0.4;
static float Speed_control_P=0;
static float Speed_control_I=0;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

char timee=0;
uint M_PACNT_left=0; 
uint M_PACNT_right=0;
uchar PIT_flag=0;   


void IO_init(void)
{
  DDRH=0x0f;  //开启PH口的0123位
}

void  PAC_Init(void)
{
  PACTL=0x40; //允许计数 事件计数模式 下降沿有效 
  PACNT=0x00; //计数的初值为0
}

void PIT_Init()
{
  
  PITCFLMT=0x00; //禁止PIT模块
  PITCE_PCE0=1;  // 开启PCE0的定时功能
  PITMUX=0x00;//使用微定时0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
  PITMTLD0 = 199;             //PIT Micro Timer Load Register
  PITLD0 = 1999;  
  PITINTE=0x01;//开启中断请求
  PITCFLMT=0x80;//开启PIT模块
}

void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<6600;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
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

void angle_calculate()
{
//  uint az,gx;
  az=AD_Measure10(7);
  gx=AD_Measure10(6);
  
  Angle_Z = (az-C_Z)*R_Z;
  Angle_G = (gx-C_Gyro)*R_Gyro;
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
  fRight = g_fAngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;
  
  g_fLeftMotorOut = fLeft;
  g_fRightMotorOut = fRight;
  MotorSpeedOut();
}

void SpeedControl(void)
{
  float fDelta;
  float fP,fI;
  
  g_fCarSpeed =(M_PACNT_left+M_PACNT_right)/2;
  M_PACNT_left=M_PACNT_right=0; 
  g_fCarSpeed*=Car_speed_constant;
  
  fDelta = Car_speed_set-g_fCarSpeed;
  fP=fDelta*Speed_control_P;
  fI=fDelta*Speed_control_I;
  
  g_fSpeedControlIntegral += fI;
  
  g_fSpeedControlOutOld = g_fSpeedControlOutNew;
  g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;
}

void SpeedControlOutput(void) 
{
  float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;
  g_fSpeedControlOut=0; //= fValue * (g_nSpeedControlPeriod + 1) /Speed_contro_period + g_fSpeedControlOutOld;
}


void main(void) 
{
  /* put your own code here */
  PLL_init_80M();//设置锁相环的时钟为48MHz 总线频率为24MHz
  IO_init();
  PWM_init();
  AD_Init();
  SCI0_init();
  
  DDRA=0XFF; 
  LCD_Init();
  PAC_Init();
  PIT_Init();  

  Draw_BMP(16,0,112,7,longqiu96x64);  //图片显示
  Dly_ms(1000);
  LCD_CLS();
  LCD_Print(20,0,"南京工程学院");  //汉字字符串显示
  LCD_Print(20,4,"飞思卡尔智能车");   //汉字字符串显示
  Dly_ms(1000);
  LCD_CLS();


	EnableInterrupts;


  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    //LCD_Fill(0xff);//黑屏  
    //LCD_Fill(0x00);//亮屏
    LCD_Print(0,0,"陀螺仪:");   //汉字字符串显示
//    DisplayNum(60,0,gx);
    LCD_Print(0,2,"加速度计:");   //汉字字符串显示
//    DisplayNum(60,2,az);
    LCD_Print(0,4,"角度:");   //汉字字符串显示
//    DisplayNum(60,4,Angle_AG);
    
    

  } /* loop forever */
  /* please make sure that you never leave main */
}

#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//清除定时器标志位
//  M_PACNT=PACNT;
//  PACNT=0;
  PIT_flag++;
  MotorOutput();
  if(PIT_flag==5)
  {
    PIT_flag=0;
    timee++;
//      DisplayNum(50,0,M_PACNT*10);
  } 
  else if(PIT_flag==1)
  {
    angle_calculate();
  }
  else if(PIT_flag==3)
  {
    AngleControl();
  }
  
  if(timee==10)
  {
    M_PACNT_left=PACNT;
    PACNT=0;  
  }
  
  if(timee==20)
  {
    timee=0;
    M_PACNT_right=M_PACNT_left;
    PACNT=0; 
  }
 
}
#pragma CODE_SEG DEFAULT

