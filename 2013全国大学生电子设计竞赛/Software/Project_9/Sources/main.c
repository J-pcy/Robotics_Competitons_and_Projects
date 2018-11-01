#include "includes.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#include "ADC.h"
#include "PLL.h"
#include "5110.h"
#define uint unsigned int
#define uchar unsigned char

#define Moto_Out_Max 100
#define Moto_Out_Min -100

#define car_Angle_set 0
#define Car_Angle_Speed_set 0

#define Optical_encode_constant 100  //光电编码盘的刻槽数量
#define Speed_contro_period 100  //速度控制周期，ms
#define Car_speed_constant 1000.0/Speed_contro_period/Optical_encode_constant  //单位转化比例值
#define Motor_Out_Dead_Val 60


float OutData[4]={0};//虚拟示波器4通道参数

/*********************************   读取AD值	**************************************/
static unsigned int AD_az_max,AD_az_min;
static unsigned int AD_gx_max,AD_gx_min;
static unsigned int AD_az_totle,AD_gx_totle,AD_az,AD_gx;

float C_Z = 525; 
float C_Gyro_x = 206;       
float Z_Min = 245; 
float Z_Max = 816;
float T_Z = 1;
float R_Z = 0.31524;
float R_Gyro = 2.16;
float Angle_Z=0,Angle_G=0,Angle_AG=0,Angle_GG;

float Angle=0;

static float Angle_Control_P=3;         //20           /3             /9
static float Angle_Control_D=0.05;         //0.41        /0.11          /0.18

static float g_fAngleControlOut;

uint timee=0;
uint PIT_flag0=0;
uint M_PACNT=0; 

uchar key1_flag=0,key2_flag=0,key3_flag=0,key4_flag=0;


void IO_init(void)
{
  DDRH=0x0f; //开启PH口的0123位
//  DDRB=0x01; //开启PB0
//  DDRA=0xff;
//  PORTA=0x00;//PA0置低
}
   
void  PAC_Init(void)
{
  PACTL=0x40; //允许计数 事件计数模式 下降沿有效 
  PACNT=0x00; //计数的初值为0
}

void Port_Init(void)
{  
    DDRH = 0X00; // PORTH作为输入
    PPSH_PPSH0 =1; // Port h0 下降沿触发中断   
    PIEH_PIEH0 = 1; // PORT h0 中断使能
    PERH = 0xff; //
}

void PIT_Init()
{  
  PITCFLMT=0x00; //禁止PIT模块
  PITCE_PCE0=1;  // 开启PCE0的定时功能
//  PITCE_PCE1=1;  // 开启PCE1的定时功能
  PITMUX=0x00;//使用微定时0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
  PITMTLD0 = 19;             //PIT Micro Timer Load Register
  PITLD0 = 1999; 
//  PITMTLD1 = 19;             //PIT Micro Timer Load Register
//  PITLD1 = 1999;  
  PITINTE=0x01;//开启中断请求
  PITCFLMT=0x80;//开启PIT模块
}

void Delay(int ss)
{
   int ii,jj;
   if (ss<1) ss=1;
   for(ii=0;ii<ss;ii++)
     for(jj=0;jj<3300;jj++);
     //for(jj=0;jj<6600;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
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

void Moto(unsigned char com,unsigned char speed_m)//占空比调节范围为0-100
{
  if(com==0)		//右电机正转		 		
	{
	PTH_PTH1=1;
	PTH_PTH3=0;
	PWMOUT(speed_m);
	}
	else if(com==1) //右电机反转
	{
   PTH_PTH1=0;
	 PTH_PTH3=1;
   PWMOUT(speed_m);
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
			AD_az_totle = AD_Measure10(4);
			AD_gx_totle = AD_Measure10(6);
			AD_az_max   = AD_az_totle;
			AD_az_min   = AD_az_totle;
			AD_gx_max	  = AD_gx_totle;
			AD_gx_min	  = AD_gx_totle;
		}
		else
		{
			AD_az = AD_Measure10(4);
			AD_gx = AD_Measure10(6);

			AD_az_totle += AD_az;
			AD_gx_totle += AD_gx;

			if(AD_az>AD_az_max) AD_az_max=AD_az;
			else if(AD_az<AD_az_min) AD_az_min=AD_az;

			if(AD_gx>AD_gx_max) AD_gx_max=AD_gx;
			else if(AD_gx<AD_gx_min) AD_gx_min=AD_gx;
			

		}
	}

	AD_az   =	(AD_az_totle-AD_az_max-AD_az_min)/4;
	AD_gx	  = (AD_gx_totle-AD_gx_max-AD_gx_min)/4; 

}

void angle_calculate()
{
  
  Angle_Z = (AD_az-C_Z)*R_Z;
  Angle_G = (C_Gyro_x-AD_gx)*R_Gyro;
//  Angle_AG = Angle_AG+(((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;
//  Angle_GG = Angle_GG+Angle_G*0.005;
}

void AngleControl(void)
{
  float fValue;
  fValue = (car_Angle_set - Angle_Z) * Angle_Control_P + (Car_Angle_Speed_set - Angle_G) * Angle_Control_D;

//输出限幅
//  if(f_Value > Angle_Control_Out_Max)
//   f_Value = Angle_Control_Out_Max;
//  if(f_Value < Angle_Control_Out_Min)
//    f_Value = Angle_Control_Out_Min;


  if(fValue > Moto_Out_Max)
    fValue = Moto_Out_Max;
  if(fValue < Moto_Out_Min)
    fValue = Moto_Out_Min;
  
  if(fValue > 0)
    fValue += Motor_Out_Dead_Val;
  else if(fValue < 0)
    fValue -= Motor_Out_Dead_Val;

  
  if(fValue>0)
    Moto(1,(int)(fValue));
  else if(fValue<0)
  {
    fValue=fValue*(-1);
    Moto(0,(int)(fValue));	
  }
//  g_fAngleControlOut = fValue;
} 
  
void Moto_Out_A(void)
{
    if(Angle_G>0)
    {
      Moto(0,65);
    } else if(Angle_G<0)
    {
      Moto(1,65);  
    }  
}

void Moto_Out_B(void)
{
    if(Angle_G>0)
    {
      Moto(0,80);
    } else if(Angle_G<0)
    {
      Moto(1,80);  
    }  
}    

void main(void) 
{
  unsigned char i;
  unsigned char j=0;

  /* put your own code here */
  PLL_init_80M();//设置锁相环的时钟为80MHz 总线频率为40MHz
  IO_init();
  PWM_init();
  AD_Init();
  SCI0_init();
  
//  Port_Init();
  LCD_init();
  PAC_Init();
  PIT_Init();

/*  for(;;)
  {
  //  Uart_output(TimerFlag20ms);
  //  Uart_output(AD_az);
  //  OutData[0]=Angle_Z;
  //  OutData[1]=Angle_GG;
  //  OutData[2]=Angle_AG;
  //  OutPut_Data();
      
  } */
  Dly_ms(1000);
  
  C_Gyro_x= AD_Measure10(6) ;
  for(i=0;i<7;i++)
  {
    C_Gyro_x+=AD_Measure10(6);
  }
  C_Gyro_x=C_Gyro_x/8;
  
//	EnableInterrupts;
  
/*  Moto(0,85);
  Dly_ms(100);
  Moto(1,85);
  Dly_ms(100);
  Moto(0,85);
  Dly_ms(100);  
  Moto(1,85);
  Dly_ms(100);
  Moto(0,85);
  Dly_ms(100);
  Moto(1,85);
  Dly_ms(100);
  Moto(0,85);
  Dly_ms(100);  
  Moto(1,85);
  Dly_ms(100);  */
    
  while(1)
  {
        
/*    PORTA=0x80;
    Delay(1);
    PORTA=0x00;
    Delay(1);    */
    
//    Uart_output(Angle);
//    Uart_output(M_PACNT);
//    Uart_putchar('\n');
//  read_AD();
//  angle_calculate(); 

  Uart_output(AD_gx);
//  Moto(0,60);
  //  OutData[0]=Angle_Z;
  //  OutData[1]=Angle_GG;
  //  OutData[2]=Angle_AG;
  //  OutPut_Data();

    if(PORTB_PB0==0)
    {
      Dly_ms(10);
      if(PORTB_PB0==0)
      {
        j++;
      }     
    while(PORTB_PB0==0); 
    }
    
    if(j%4==0)
    {
      LCD_write_english_string(0,2,"mode1");
      key1_flag=1;
      key2_flag=0;
      key3_flag=0;
      key4_flag=0;
    }
    else if(j%4==1)
    {
      LCD_write_english_string(0,2,"mode2");
      key2_flag=1;
      key1_flag=0;
      key3_flag=0;
      key4_flag=0;
    }
    else if(j%4==2)
    {
      LCD_write_english_string(0,2,"mode3");
      key3_flag=1;
      key2_flag=0;
      key1_flag=0;
      key4_flag=0;
            
    }
    else if(j%4==3)
    {
      LCD_write_english_string(0,2,"mode4");
      key4_flag=1;
      key2_flag=0;
      key1_flag=0;
      key3_flag=0;
    }

    
    if(PORTB_PB1==0)
    {
      Dly_ms(10);
      if(PORTB_PB1==0)
      {
        C_Z++;
      }
      while(PORTB_PB1==0); 
    }
    
    if(PORTB_PB2==0)
    {
      Dly_ms(10);
      if(PORTB_PB2==0)
      {
        C_Z--;
      }
      while(PORTB_PB2==0); 
    }

    if(PORTB_PB3==0)
    {
      Dly_ms(10);
      if(PORTB_PB3==0)
      {
        EnableInterrupts;   
      }
      while(PORTB_PB3==0); 
    }  
    
    LCD_write_english_string(0,0,"A:");
    LCD_Show_Number (35,0,C_Z);
    LCD_write_english_string(0,1,"Angle");      
    LCD_Show_Number (35,1,Angle_Z);     
    
/*    if(PORTB_PB0==0)
    {
      Dly_ms(10);
      if(PORTB_PB0==0)
      {
        key1_flag=1;
        key2_flag=0;
        key3_flag=0;
      }     
    while(PORTB_PB0==0); 
    }
    
    if(PORTB_PB1==0)
    {
      Dly_ms(10);
      if(PORTB_PB1==0)
      {
        key2_flag=1;
        key1_flag=0;
        key3_flag=0;
      }     
    while(PORTB_PB1==0); 
    }
    
    if(PORTB_PB2==0)
    {
      Dly_ms(10);
      if(PORTB_PB2==0)
      {
        key3_flag=1;
        key2_flag=0;
        key1_flag=0;
      }     
    while(PORTB_PB2==0); 
    }
    
    if(PORTB_PB3==0)
    {
      Dly_ms(10);
      if(PORTB_PB3==0)
      {
        EnableInterrupts;   
      }
      while(PORTB_PB3==0); 
    }            */
    
/*    Moto(0,60);
    Dly_ms(500);
    Moto(1,60);
    Dly_ms(500); */   
       
  }
  
}

#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//清除定时器标志位
  
  if(key1_flag==1)
  {
    PIT_flag0++;
    if(PIT_flag0==3)                                  
    {
      PIT_flag0=0;
    } 
    else if(PIT_flag0==1)
    {
      read_AD();
      angle_calculate(); 
    }
    else if(PIT_flag0==2)
    {
      Moto_Out_A();
    }
  }
  
  if(key2_flag==1)
  {
    PIT_flag0++;
    if(PIT_flag0==3)                                  
    {
      PIT_flag0=0;
    } 
    else if(PIT_flag0==1)
    {
      read_AD();
      angle_calculate(); 
    }
    else if(PIT_flag0==2)
    {
      Moto_Out_B();
    }
  }

  
  if(key3_flag==1)
  {
    PIT_flag0++;
    if(PIT_flag0==2)                                  
    {
      PIT_flag0=0;
      AngleControl();
    } 
    else if(PIT_flag0==1)
    {
      read_AD();
      angle_calculate(); 
    }
  }
} 
#pragma CODE_SEG DEFAULT 

/*#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 25 PORTH_ISR(void)
{         
  PIFH_PIFH0 = 1;
  PIEH_PIEH0 = 0; // PORT H0 中断使能
  PPSH_PPSH0 =~PPSH_PPSH0; // Port H0 上升沿沿触发中断
  PIEH_PIEH0 = 1; // PORT H0 中断使能
  
  M_PACNT=PACNT;
  PACNT=0;
  
//  PORTA_PA0=~PORTA_PA0; 
  if(PTH_PTH0)
  {
    Angle= Angle+M_PACNT*0.72; 
  } 
  else
  {
    Angle= Angle-M_PACNT*0.72; 
  }
}  */
 
/*#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//清除定时器标志位
  PIT_flag0++;
//  M_PACNT=PACNT;
//  PACNT=0;
//  PORTA=0x00;
  if(PIT_flag0==5)                                  
  {
    PIT_flag0=0;
    timee++;
    if(timee==4)
    {
      M_PACNT_left=PACNT;
      //Uart_output(M_PACNT_left);
      PACNT=0;
      PORTB=0x00;
    }
    else if(timee==8)
    {
      M_PACNT_right=PACNT;
      //Uart_output(M_PACNT_right);
      PACNT=0;
      PORTB=0x01;
      timee=0;
    } 
  } 
  else if(PIT_flag0==1)
  {
    read_AD();
    angle_calculate(); 
  }
  else if(PIT_flag0==2)
  {
  }
  else if(PIT_flag0==3)
  {
  } 
  else if(PIT_flag0==4)
  {
  }
}
#pragma CODE_SEG DEFAULT */   




