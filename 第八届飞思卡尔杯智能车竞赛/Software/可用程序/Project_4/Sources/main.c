#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
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

#define SPEED_CONTROL_COUNT 20

#define Optical_encode_constant 100  //�������̵Ŀ̲�����
#define Speed_contro_period 100  //�ٶȿ������ڣ�ms
#define Car_speed_constant 1000.0/Speed_contro_period/Optical_encode_constant  //��λת������ֵ

int g_nSpeedControlPeriod=0;
int g_nSpeedControlCount=0;                          

float C_Z = 312;
float C_Gyro = 407.5;
float Z_Min = 147.816; 
float Z_Max = 474.205;
float T_Z = 1;
float R_Z = 0.55149;
float R_Gyro = 2.25;
float Angle_Z=0,Angle_G=0,Angle_AG=0,Angle_GG;
float g_fCarSpeed=0,g_fSpeedControlIntegral=0,g_fSpeedControlOutOld=0,g_fSpeedControlOutNew=0;

float OutData[4]={0};//����ʾ����4ͨ������

/*********************************   ��ȡADֵ	**************************************/
static unsigned int AD_az_max,AD_az_min;
static unsigned int AD_gx_max,AD_gx_min;
static unsigned int AD_az_totle,AD_gx_totle,AD_az,AD_gx;


static float Angle_Control_P=10;
static float Angle_Control_D=0.19;
static float Speed_control_P=0;
static float Speed_control_I=0;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

uchar timee=0;
uchar PIT_flag0=0;
uint M_PACNT_left=0; 
uint M_PACNT_right=0;


void IO_init(void)
{
  DDRH=0x0f;  //����PH�ڵ�0123λ
  DDRB=0x03;
}
   
void  PAC_Init(void)
{
  PACTL=0x40; //������� �¼�����ģʽ �½�����Ч 
  PACNT=0x00; //�����ĳ�ֵΪ0
}

void PIT_Init()
{
  
  PITCFLMT=0x00; //��ֹPITģ��
  PITCE_PCE0=1;  // ����PCE0�Ķ�ʱ����
  PITMUX=0x00;//ʹ��΢��ʱ0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
  PITMTLD0 = 19;             //PIT Micro Timer Load Register
  PITLD0 = 1999;  
  PITINTE=0x01;//�����ж�����
  PITCFLMT=0x80;//����PITģ��
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
    

void MotoR(unsigned char com,unsigned char speed_m)//ռ�ձȵ��ڷ�ΧΪ0-100
{
  if(com==0)		//������ת		 		
	{
	PTH_PTH0=1;
	PTH_PTH2=0;
	PWMOUT_R(speed_m);
	}
	else if(com==1) //������ת
	{
   PTH_PTH0=0;
	 PTH_PTH2=1;
   PWMOUT_R(speed_m);
	}
}


void MotoL(unsigned char com,unsigned char speed_m)
{


	if(com==0)		//�ҵ����ת		 		
	{ 
  PTH_PTH1=0;
	PTH_PTH3=1;
  PWMOUT_L(speed_m);
	}
	else if(com==1) //�ҵ����ת
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
  for(i=0;i<8;i++)
  {
    if(i==0)
	  {
			AD_az_totle = AD_Measure10(5);
			AD_gx_totle = AD_Measure10(6);
			AD_az_max   = AD_az_totle;
			AD_az_min   = AD_az_totle;
			AD_gx_max	  = AD_gx_totle;
			AD_gx_min	  = AD_gx_totle;
		}
		else
		{
			AD_az = AD_Measure10(5);
			AD_gx = AD_Measure10(6);

			AD_az_totle += AD_az;
			AD_gx_totle += AD_gx;

			if(AD_az>AD_az_max) AD_az_max=AD_az;
			else if(AD_az<AD_az_min) AD_az_min=AD_az;

			if(AD_gx>AD_gx_max) AD_gx_max=AD_gx;
			else if(AD_gx<AD_gx_min) AD_gx_min=AD_gx;
		}
	}

	AD_az   =	(AD_az_totle-AD_az_max-AD_az_min)/6;
	AD_gx	  = (AD_gx_totle-AD_gx_max-AD_gx_min)/6;
}

void angle_calculate()
{
//  uint az,gx;
//  float R_Z = 180/(Z_Max - Z_Min);

  
  Angle_Z = (AD_az-C_Z)*R_Z;
  Angle_G = (AD_gx-C_Gyro)*R_Gyro;
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

//����޷�
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
  
//��������
/*  if(fLeftVal > 0)
    fLeftVal += Motor_Out_Dead_Val;
  else if(fLeftVal < 0)
    fLeftVal -= Motor_Out_Dead_Val;

  if(fRightVal > 0)
    fRightVal += Motor_Out_Dead_Val;
  else if(fRightVal < 0)
    fRightVal -= Motor_Out_Dead_Val;*/
 
//����޷�    
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

void main(void) 
{
  /* put your own code here */
  PLL_init_80M();//�������໷��ʱ��Ϊ80MHz ����Ƶ��Ϊ40MHz
  IO_init();
  PWM_init();
  AD_Init();
  SCI0_init();
  
//  DDRA=0XFF; 
//  LCD_Init();
  PAC_Init();
  PIT_Init(); 
	EnableInterrupts;

//  for(;;) 
//  {
//  }
  while(1)
  {
  //  Uart_output(g_fSpeedControlOut);
  
    Uart_output(AD_gx);
  //  OutData[0]=Angle_Z;
  //  OutData[1]=Angle_GG;
  //  OutData[2]=Angle_AG;
  //  OutPut_Data();  
  }
}

#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//�����ʱ����־λ
//  M_PACNT=PACNT;
//  PACNT=0;
  PIT_flag0++;
  g_nSpeedControlPeriod ++;
  SpeedControlOutput();
  if(PIT_flag0==5)
  {
    PIT_flag0=0;
    timee++;
    if(timee==20)
    {
      M_PACNT_left=PACNT;
      //Uart_output(M_PACNT_left);
      PACNT=0;
      PORTB=0x00;
    }
    if(timee==40)
    {
      M_PACNT_right=PACNT;
      //Uart_output(M_PACNT_right);
      timee==0;
      PACNT=0;
      PORTB=0x01;
    }
//    PACNT=0;
  } 
  else if(PIT_flag0==1)
  {
//    PORTB=0x02;
    read_AD();
//    PORTB=0x00;
  }
  else if(PIT_flag0==2)
  {
//    PORTB=0x02;
    angle_calculate();
//    PORTB=0x00;
  }
  else if(PIT_flag0==3)
  {
//    PORTB=0x02;
    AngleControl();
    MotorOutput();
//    PORTB=0x00;
  } 
  else if(PIT_flag0==4)
  {
    g_nSpeedControlCount ++;
    if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
    {
      SpeedControl();
      g_nSpeedControlCount = 0;
      g_nSpeedControlPeriod = 0;
    }
  }
 
}
#pragma CODE_SEG DEFAULT   


/*#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//�����ʱ����־λ
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
#pragma CODE_SEG DEFAULT   */  

/*#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//�����ʱ����־λ
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




