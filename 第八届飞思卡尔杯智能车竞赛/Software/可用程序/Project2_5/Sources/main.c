#include "includes.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#include "ADC.h"
#include "PWM.h"
#include "PLL.h"
#include "5110.h"
#define uint unsigned int
#define uchar unsigned char

#define car_Angle_set 0
//#define Car_speed_set 1
#define Car_Angle_Speed_set 0
#define Moto_Out_Max 100
#define Moto_Out_Min -100
#define Motor_Out_Dead_Val 17

#define SPEED_CONTROL_COUNT 20

#define DIRECTION_CONTROL_COUNT 2

#define Optical_encode_constant 100  //光电编码盘的刻槽数量
#define Speed_contro_period 100  //速度控制周期，ms
#define Car_speed_constant 1000.0/Speed_contro_period/Optical_encode_constant  //单位转化比例值

#define Direction_contro_period 10

#define Black_Limit 64;

float Car_speed_set=0;
float Car_speed_select=2;

int g_nSpeedControlPeriod=0;
int g_nSpeedControlCount=0; 

int g_nDirectionControlPeriod=0;
int g_nDirectionControlCount =0;                         

float C_Z = 312;        //值大前倾   
float C_Gyro_y = 291;
float C_Gyro_x = 302;
float Z_Min = 146; 
float Z_Max = 462;
float T_Z = 1;
float R_Z = 0.5696;
float R_Gyro = 2.16;
float Angle_Z=0,Angle_G=0,Angle_AG=0,Angle_GG;

float g_fCarSpeed=0,g_fSpeedControlIntegral=0,g_fSpeedControlOutOld=0,g_fSpeedControlOutNew=0;

float g_fDirectionControlOutOld=0,g_fDirectionControlOutNew=0;

float OutData[4]={0};//虚拟示波器4通道参数

/*********************************   读取AD值	**************************************/
static unsigned int AD_az_max,AD_az_min;
static unsigned int AD_gx_max,AD_gx_min;
static unsigned int AD_gy_max,AD_gy_min;
static unsigned int AD_az_totle,AD_gx_totle,AD_gy_totle,AD_az,AD_gy;
int A;


static float Angle_Control_P=18;
static float Angle_Control_D=0.41;
static float Speed_control_P=100;
static float Speed_control_I=0;
static float DIR_CONTROL_P=1.4;
static float DIR_CONTROL_D=0;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

uchar timee=0;
uint time_q=0;
uchar PIT_flag0=0;
uint M_PACNT_left=0; 
uint M_PACNT_right=0;

unsigned char TimerFlag20ms = 0;
unsigned char send_data_cnt =0;
unsigned char TimerCnt20ms = 0;
unsigned char integration_piont;

unsigned char Pixel[128];
unsigned char Pixel1[128];//二值化后的数组
unsigned char black_Limit=64;
static unsigned char black_right=0;
static unsigned char black_left=0;
static unsigned char black=0;
static unsigned char black_error=57;


unsigned char Limit=30;

extern unsigned char PixelAverageVoltage;
extern unsigned char PixelAverageValue;

void IO_init(void)
{
  DDRH=0x0f;  //开启PH口的0123位
  DDRB=0x01; //开启PB0
  PUCR=0xd2;
//  DDRA=0x01;
  PORTB=0x00;//PB0置低
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
  PITCE_PCE1=1;  // 开启PCE1的定时功能
  PITMUX=0x00;//使用微定时0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
  PITMTLD0 = 19;             //PIT Micro Timer Load Register
  PITLD0 = 1999; 
  PITMTLD1 = 19;             //PIT Micro Timer Load Register
  PITLD1 = 1999;  
  PITINTE=0x03;//开启中断请求
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
  if(com==0)		//右电机正转		 		
	{
	PTH_PTH0=1;
	PTH_PTH2=0;
	PWMOUT_R(speed_m);
	}
	else if(com==1) //右电机反转
	{
   PTH_PTH0=0;
	 PTH_PTH2=1;
   PWMOUT_R(speed_m);
	}
}


void MotoL(unsigned char com,unsigned char speed_m)
{


	if(com==0)		//左电机正转		 		
	{ 
  PTH_PTH1=0;
	PTH_PTH3=1;
  PWMOUT_L(speed_m);
	}
	else if(com==1) //左电机反转
 {
  PTH_PTH1=1;
	PTH_PTH3=0;
	PWMOUT_L(speed_m);
	}
}

unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
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
}  

void read_AD()
{
  unsigned char i;
  for(i=0;i<6;i++)
  {
    if(i==0)
	  {
			AD_az_totle = AD_Measure10(5);
			AD_gy_totle = AD_Measure10(6);
//			AD_gx_totle = AD_Measure10(7);
			AD_az_max   = AD_az_totle;
			AD_az_min   = AD_az_totle;
			AD_gy_max	  = AD_gy_totle;
			AD_gy_min	  = AD_gy_totle;
//		  AD_gx_max	  = AD_gx_totle;
//			AD_gx_min	  = AD_gx_totle;

		}
		else
		{
			AD_az = AD_Measure10(5);
			AD_gy = AD_Measure10(6);
//			AD_gx = AD_Measure10(7);

			AD_az_totle += AD_az;
			AD_gy_totle += AD_gy;
//			AD_gx_totle += AD_gx;

			if(AD_az>AD_az_max) AD_az_max=AD_az;
			else if(AD_az<AD_az_min) AD_az_min=AD_az;

			if(AD_gy>AD_gy_max) AD_gy_max=AD_gy;
			else if(AD_gy<AD_gy_min) AD_gy_min=AD_gy;
			
//			if(AD_gx>AD_gx_max) AD_gx_max=AD_gx;
//			else if(AD_gx<AD_gx_min) AD_gx_min=AD_gx;

		}
	}

	AD_az   =	(AD_az_totle-AD_az_max-AD_az_min)/4;
	AD_gy	  = (AD_gy_totle-AD_gy_max-AD_gy_min)/4; 
//	AD_gx	  = (AD_gx_totle-AD_gx_max-AD_gx_min)/4; 

}

void read_AD_x()
{
  unsigned char i;
  for(i=0;i<6;i++)
  {
    if(i==0)
    {
      AD_gx_totle = AD_Measure10(7);
		  AD_gx_max	  = AD_gx_totle;
			AD_gx_min	  = AD_gx_totle;
    }
    else
    {
			A = AD_Measure10(7);
			AD_gx_totle += A;
			if(A>AD_gx_max) AD_gx_max=A;
			else if(A<AD_gx_min) AD_gx_min=A;
    }
  }
	A	  = (AD_gx_totle-AD_gx_max-AD_gx_min)/4; 

}

void angle_calculate()
{
//  uint az,gx;
//  float R_Z = 180/(Z_Max - Z_Min);

  
  Angle_Z = (AD_az-C_Z)*R_Z;
  Angle_G = (C_Gyro_y-AD_gy)*R_Gyro;
  Angle_AG = Angle_AG+(((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;
//  Angle_GG = Angle_GG+Angle_G*0.005;
//  Uart_output(az);
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

  g_fAngleControlOut = fValue;
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
    fRightVal -= Motor_Out_Dead_Val;  */
 
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
  
  fLeft = -g_fAngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
  fRight = -g_fAngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;
  
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
  g_fSpeedControlOut= fValue * (g_nSpeedControlPeriod + 1) /Speed_contro_period + g_fSpeedControlOutOld;
}

void Binarization(void)//二值化的程序
{
  unsigned int i;
  if(PixelAverageValue>=70)
  {
    Limit=PixelAverageValue-20;  
  }
  else if(PixelAverageValue>=40) 
  {
    Limit=PixelAverageValue-12;  
  }
  else 
  {
    Limit=PixelAverageValue-8;    
  }
  for(i=0;i<128;i++)
  {
    if(Pixel[i]<=Limit)
    {

      Pixel1[i]=1;
    }
    else
    {
      Pixel1[i]=0;     
    }
  }
}

void Edge_finish(void)    //边缘滤波
{
  unsigned int i;
  for(i=0;i<4;i++)
  {
    if(Pixel1[i]==1)
    {

      Pixel1[i]=0;
    }
  }
  for(i=124;i<128;i++)
  {
    if(Pixel1[i]==1)
    {

      Pixel1[i]=0;
    }
  }   
}

/*void black_extract(void)          //修改过的程序
{
  unsigned int i,j,k,flag=0;
  for(i=0;i<128;i++)
  {
    if(Pixel1[i]==1)
    {
      k=i;
      break;
    }
  }
  for(j=k;j<k+30;j++)
  {
    if(Pixel1[j]==1)
    {
      flag++;
    }
  }
  if(flag>=20)
  {
    black=k+12;  
  }

}  */

/*void black_extract(void)              //基于原错误程序
{
  unsigned int i,j,m,n,k1,k2,flag1=0,flag2=0;
  for(i=64;i>=0;i--)
  {
    if(Pixel1[i]==1)
    {
      k1=i;
      for(j=k1;j>k1-8;j--)
      {
        if(Pixel1[i]==1)
        {
          flag1++;
        }
      }
      if(flag1>=4)
      {
        black_left=k1-4;  
      }
      else
      {
        black_left=0;
      }
    }
  }
  for(m=64;m<=127;m++)
  {
    if(Pixel1[m]==1)
    {
      k2=m;
      for(n=k2;n>k2+8;n++)
      {
        if(Pixel1[m]==1)
        {
          flag2++;
        }
      }
      if(flag2>=4)
      {
        black_right=k2+4;  
      }
      else
      {
        black_right=0;
      }
    }
  }
  if(black_left!=0&&black_right!=0)
  {
    black=black_left+(black_right-black_right)/2;
  }
  else if(black_left==0&&black_right!=0)
  {
    black=black_right-black_error;
  }
  else if(black_right==0&&black_left!=0)
  {
    black=black_left-black_error;
  }
  else
  {
    black=Black_Limit;
  }
}*/ 

void black_extract(void)              
{
  unsigned int i,j,m,n,k1,k2,flag1=0,flag2=0;
  black_Limit=Black_Limit;
  for(i=64;i>=0;i--)
  {
    if(Pixel1[i]==1)
    {
      k1=i;
      break;
    }
  }
  for(j=k1;j>k1-6;j--)
  {
    if(Pixel1[j]==1)
    {
      flag1++;
    }
  }
  if(flag1>=3)
  {
    black_left=k1;
    k1=0;  
  }
  else
  {
    black_left=0;
  }
    
  
  for(m=64;m<=127;m++)
  {
    if(Pixel1[m]==1)
    {
      k2=m;
      break;
    }
  }
  for(n=k2;n<k2+6;n++)
  {
    if(Pixel1[n]==1)
    {
      flag2++;
    }
  }
  if(flag2>=3)
  {
    black_right=k2;
    k2=0;  
  }
  else
  {
    black_right=0;
  }

  if(black_left!=0&&black_right!=0)
  {
    black=black_left+(black_right-black_left)/2;
    Car_speed_set=Car_speed_set+0.01; 
  }
  else if(black_left==0&&black_right!=0)
  {
    black=black_right-black_error;
    black_Limit=Black_Limit+24;
  }
  else if(black_right==0&&black_left!=0)
  {
    black=black_left+black_error;
    black_Limit=Black_Limit-24;
  }
  else
  {
    black=black_Limit;
  }
  if(black<=Black_Limit+1&&black>=Black_Limit-1)
  {
    Car_speed_set=Car_speed_set+0.01;
    if(Car_speed_set>=Car_speed_select+0.5)
    {
      Car_speed_set=Car_speed_select+0.5;  
    }
  }
  else
  {
    Car_speed_set=Car_speed_set-0.01; 
    if(Car_speed_set<=0)
    {
      Car_speed_set=0;  
    }
  }
}  
 

/*void DirectionControl(void)
{
  float fValue;
  
//  g_fDirectionControlOutOld = g_fDirectionControlOutNew;
  
  if(black<=black_Limit&&black>black_Limit-16)
  {
    fValue=DIR_CONTROL_P*1.2*(black_Limit-black); 
  } 
  else if(black<=black_Limit-16&&black>black_Limit-32)
  {
    fValue=DIR_CONTROL_P*1.1*(black_Limit-black); 
  }
  else if(black<=black_Limit-32&&black>black_Limit-48)
  {
    fValue=DIR_CONTROL_P*1*(black_Limit-black); 
  }
  else if(black<=black_Limit-48)
  {
    fValue=DIR_CONTROL_P*0.9*(black_Limit-black); 
  }
  
  else if(black>=black_Limit&&black<black_Limit+16)
  {
    fValue=DIR_CONTROL_P*1.2*(black_Limit-black); 
  }
  else if(black>=black_Limit+16&&black<black_Limit+32)
  {
    fValue=DIR_CONTROL_P*1.1*(black_Limit-black); 
  }
  else if(black>=black_Limit+32&&black<black_Limit+48)
  {
    fValue=DIR_CONTROL_P*1*(black_Limit-black); 
  }
  else if(black>=black_Limit+48)
  {
    fValue=DIR_CONTROL_P*0.9*(black_Limit-black); 
  }
  
  g_fDirectionControlOut = fValue;
}        */

void DirectionControl(void)
{
  float fValue;
  float fDValue;  
  
//  g_fDirectionControlOutOld = g_fDirectionControlOutNew;
  
  fValue = DIR_CONTROL_P*(black_Limit-black);
  fDValue = C_Gyro_x-A;
  fDValue *= DIR_CONTROL_D;
  fValue += fDValue;
  g_fDirectionControlOut = fValue;  

} 

/*void DirectionControlOutput(void) 
{
  float fValue;
  fValue = g_fDirectionControlOutNew - g_fDirectionControlOutOld;
  g_fDirectionControlOut = fValue * (g_nDirectionControlPeriod + 1) /Direction_contro_period + g_fDirectionControlOutOld;    
} */

void main(void) 
{
  unsigned char k,i;
  /* put your own code here */
  PLL_init_80M();//设置锁相环的时钟为80MHz 总线频率为40MHz
  IO_init();
  PWM_init();
  AD_Init();
  SCI0_init();
  LCD_init();
  
//  DDRA=0XFF; 
//  LCD_Init();
  PAC_Init();
  PIT_Init();
  CCD_IO_Init();
  Dly_ms(1000);

	C_Gyro_y= AD_Measure10(6) ;
  for(i=0;i<7;i++)
  {
    C_Gyro_y+=AD_Measure10(6);
  }
  C_Gyro_y=C_Gyro_y/8;
//  while(PORTB_PB2==0);
	EnableInterrupts;

  while(1)
  {
//    MotoR(1,100);
//    MotoL(1,100); 
     
//    Uart_output(TimerFlag20ms);
  //  Uart_output(AD_az);
  //  OutData[0]=Angle_Z;
  //  OutData[1]=Angle_GG;
  //  OutData[2]=Angle_AG;
  //  OutPut_Data();
  //  unsigned char k;
//    if(TimerFlag20ms == 1) 
//    {
//      PORTA=0x01;
//      TimerFlag20ms = 0;
//      ImageCapture(Pixel);
//      Binarization();//二值化的程序
//      CalculateIntegrationTime();

/*      for(k=0;k<128;k++)
      {
        Uart_putchar(Pixel1[k]+48);
      }
      Uart_putchar('\r');
      Uart_putchar('\n'); 
      Uart_output(black);         
//      Uart_output(black_left);
//      Uart_output(black_right); 
       

//      Uart_output(g_fLeftMotorOut);
//      Uart_output(g_fRightMotorOut);
      
      Uart_output(PixelAverageValue);    */  
//      Uart_output(Limit); 
      
/*      for(k=0;k<128;k++)
      {
        Uart_putchar(Pixel[k]/100+48);
        Uart_putchar(Pixel[k]%100/10+48);
        Uart_putchar(Pixel[k]%100%10+48);
      }
      Uart_putchar('\r');
      Uart_putchar('\n');
//      Uart_output(PixelAverageVoltage);
//      Uart_putchar('\n');
      Uart_output(PixelAverageValue);   */ 
      
//    } 


    LCD_write_english_string(0,0,"AD_az:");
    LCD_Show_Number (35,0,AD_az);
    LCD_write_english_string(0,1,"AD_gy");
    LCD_Show_Number (35,1,AD_gy);     

  }
  
}

/*#pragma CODE_SEG __NEAR_SEG NON_BANKED

__interrupt VectorNumber_Vpit0 void Timer0(void) 
{
    unsigned char k;
    static unsigned char TimerCnt20ms = 0;
    unsigned char integration_piont;

    PITTF_PTF0 = 1;

    TimerCnt20ms++;

    /* 根据曝光时间计算20ms周期内的曝光点 */
//    integration_piont = 20 - IntegrationTime;
//    if(integration_piont >= 2) {      /* 曝光点小于2(曝光时间大于18ms)则不进行再曝光 */
//        if(integration_piont == TimerCnt20ms)
//        StartIntegration();           /* 曝光开始 */
//    }

     

/*    if(TimerCnt20ms >= 20)
    {
        TimerCnt20ms = 0;
//        TimerFlag20ms = 1;
      ImageCapture(Pixel);
      Binarization();//二值化的程序
      CalculateIntegrationTime();
      
      for(k=0;k<128;k++)
      {
        Uart_putchar(Pixel1[k]+48);
      }
      Uart_putchar('\r');
      Uart_putchar('\n');
    }

}

#pragma CODE_SEG DEFAULT */

 
#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  unsigned char k;
  //PORTB_PB0=~PORTB_PB0;
  PITTF=0x01;//清除定时器标志位
//  M_PACNT=PACNT;
//  PACNT=0;
//  PORTA=0x00;
  PIT_flag0++;
//  TimerCnt20ms++;
  g_nSpeedControlPeriod ++;
  SpeedControlOutput();
  
//  g_nDirectionControlPeriod ++; 
//  DirectionControlOutput();
  
  if(PIT_flag0==5)
  {
    PIT_flag0=0;
    time_q++;
    timee++;      
    if(timee==4)
    {
      M_PACNT_left=PACNT;
//      Uart_output(M_PACNT_left);
      PACNT=0;
      PORTB=0x00;
    }
    else if(timee==8)
    {
      M_PACNT_right=PACNT;
//      Uart_output(M_PACNT_right);
      PACNT=0;
      PORTB=0x01;
      timee=0;
    }
    if(time_q>=1000)
    {
      time_q=1000;
      Car_speed_set=Car_speed_set+0.005;
      if(Car_speed_set>=Car_speed_select)
      {
        Car_speed_set=Car_speed_select;
      }
    }
  } 
  else if(PIT_flag0==1)
  {
//    PORTA=0x01;
    read_AD();
    angle_calculate();
    AngleControl();
    MotorOutput();
//    PORTA=0x00;
  }
  else if(PIT_flag0==2)
  {
//    PORTB=0x02;
    g_nSpeedControlCount ++;
    if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
    {
      SpeedControl();
      g_nSpeedControlCount = 0;
      g_nSpeedControlPeriod = 0;
    }  
//    PORTB=0x00;
  }
  else if(PIT_flag0==3)
  {
//    PORTA=0x01;
    if(TimerFlag20ms == 1) 
    {
      ImageCapture(Pixel); 
    }
//    PORTA=0x00;
  }   
  else if(PIT_flag0==4)
  {
//    PORTA=0x01;
//    g_nDirectionControlCount ++;
    if(TimerFlag20ms == 1) 
    {
      Binarization();//二值化的程序
      CalculateIntegrationTime();
      Edge_finish();
      black_extract();
      read_AD_x();
      DirectionControl();
      TimerFlag20ms = 0;
    }
/*    if(g_nDirectionControlCount >= DIRECTION_CONTROL_COUNT) 
    {
      DirectionControl();
      g_nDirectionControlCount = 0;
      g_nDirectionControlPeriod = 0;
    }   */
//    PORTA=0x00;  
  } 
}
#pragma CODE_SEG DEFAULT      

#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit1 void  timer_inte1(void) 
{
  PITTF=0x02;//清除定时器标志位 
  TimerCnt20ms++; 
    /* 根据曝光时间计算20ms周期内的曝光点 */
  integration_piont = 20 - IntegrationTime;
  if(integration_piont >= 2) 
  {      /* 曝光点小于2(曝光时间大于18ms)则不进行再曝光 */
    if(integration_piont == TimerCnt20ms)
    {
//      PORTA=0x01;
      StartIntegration();           /* 曝光开始 */
//      PORTA=0x00; 
    }
  }
  if(TimerCnt20ms >= 20)
  {
//    PORTA=0x01;
    TimerCnt20ms = 0;
    TimerFlag20ms = 1;
  }
//  Uart_output(TimerFlag20ms);
}
#pragma CODE_SEG DEFAULT 

/*#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//清除定时器标志位
//  M_PACNT=PACNT;
//  PACNT=0;
  PIT_flag0++;
  MotorOutput();
  if(PIT_flag0==5)
  {
    PIT_flag0=0;
  } 
  else if(PIT_flag0==1)
  {
//    PORTB=0x02;
    read_AD();
//    PORTB=0x00;
  }
  else if(PIT_flag0==3)
  {
//    PORTB=0x02;
    angle_calculate();
//    PORTB=0x00;
  }
  else if(PIT_flag0==4)
  {
//    PORTB=0x02;
    AngleControl();    
//    PORTB=0x00;
  }   
 
}
#pragma CODE_SEG DEFAULT    */ 

/*#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//清除定时器标志位
//  M_PACNT=PACNT;
//  PACNT=0;
  PIT_flag0++;
  MotorOutput();
  if(PIT_flag0==50)
  {
    M_PACNT_left=PACNT;
    Uart_output(M_PACNT_left);
    PACNT=0;
    PORTB=0x00;
  }
  if(PIT_flag0==100)
  {
    M_PACNT_right=PACNT;
//    Uart_output(M_PACNT_right);
    timee==0;
    PACNT=0;
    PORTB=0x01;
  }
    
 
}
#pragma CODE_SEG DEFAULT  */ 




