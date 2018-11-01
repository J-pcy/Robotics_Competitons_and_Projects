/*!
 * program for freescale 
 *
 * @file       main.c
 * @brief      野火K60 平台
 * @author     J-pcy
 * @version    v1.0      直立+摄像头+图像处理准备
 * @date       2014-05-18
 */

#include "common.h"
#include "include.h"
#include "math.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

#define car_Angle_set 0
#define Car_speed_set 0
#define Car_Angle_Speed_set 0
#define Moto_Out_Max 1000
#define Moto_Out_Min -1000

#define SPEED_CONTROL_COUNT 20

#define Optical_encode_constant 100  //光电编码盘的刻槽数量
#define Speed_contro_period 100  //速度控制周期，ms
#define Car_speed_constant 1000.0/Speed_contro_period/Optical_encode_constant  //单位转化比例值


int g_nSpeedControlPeriod=0;
int g_nSpeedControlCount=0;                          

float C_Z = 30567;           //目前调速  318-慢
float C_Gyro =29584;
float Z_Min = 13753; 
float Z_Max = 44471;
float T_Z = 1;
float R_Z = 0.0058598;        //R_Z = 180/(Z_Max - Z_Min);
float R_Gyro = 0.021;
float Angle_Z=0,Angle_G=0,Angle_AG=0,Angle_GG;
float g_fCarSpeed=0,g_fSpeedControlIntegral=0,g_fSpeedControlOutOld=0,g_fSpeedControlOutNew=0;


float OutData[4]={0};//虚拟示波器4通道参数

/*********************************   读取AD值	**************************************/
static unsigned int AD_az_max,AD_az_min;
static unsigned int AD_gy_max,AD_gy_min;
static unsigned int AD_az_totle,AD_gy_totle,AD_az,AD_gy;

static float Angle_Control_P=0;                //18
static float Angle_Control_D=0;                //0.41
static float Speed_control_P=0;                //100
static float Speed_control_I=0;
static float DIR_CONTROL_P=0;                
static float DIR_CONTROL_D=0;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;

unsigned char PACNT_flag=0;
unsigned char PIT_flag0=0;
unsigned int M_PACNT_left=0; 
unsigned int M_PACNT_right=0;


//函数声明
extern void PIT0_IRQHandler(void);
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*********************************   GPIO初始化	**************************************/
void IO_init()
{
  gpio_init(PTD1,GPO,HIGH);
  gpio_init(PTE2,GPO,LOW);
  gpio_init(PTE4,GPO,LOW); 
  gpio_init(PTE6,GPO,LOW); 
  gpio_init(PTE7,GPO,LOW); 
}

/*********************************   AD初始化	**************************************/
void AD_init()
{
  adc_init(ADC1_SE16);              //ADC初始化
  adc_init(ADC0_SE16); 
}

/*********************************   PWM初始化	**************************************/
void PWM_init()
{
  FTM_PWM_init(FTM0, FTM_CH5,20*1000,0);    //初始化 FTM PWM ，使用 FTM0_CH3，频率为200k ，占空比为 30 / FTM0_PRECISON
  FTM_PWM_init(FTM0, FTM_CH7,20*1000,0);    //初始化 FTM PWM ，使用 FTM0_CH3，频率为200k ，占空比为 30 / FTM0_PRECISON
}

/*
void Uart_output(unsigned int data) 
{
  uart_putchar(FIRE_PORT,data/1000+48);
  uart_putchar(FIRE_PORT,data%1000/100+48);
  uart_putchar(FIRE_PORT,data%1000%100/10+48);
  uart_putchar(FIRE_PORT,data%1000%100%10+48);
  printf("\r\n");
}
*/

void MotoR(unsigned char com,unsigned char speed_m)//占空比调节范围为0-100
{
  if(com==0)		//左电机正转		 		
  {
    gpio_set(PTE2,HIGH);
    gpio_set(PTE6,LOW);
    FTM_PWM_Duty(FTM0, FTM_CH5,speed_m);
  }
  else if(com==1) //左电机反转
  {
    gpio_set(PTE2,LOW);
    gpio_set(PTE6,HIGH);
    FTM_PWM_Duty(FTM0, FTM_CH5,speed_m);
  }
} 


void MotoL(unsigned char com,unsigned char speed_m)
{
  if(com==0)		//右电机正转		 		
  { 
    gpio_set(PTE4,LOW);
    gpio_set(PTE7,HIGH);
    FTM_PWM_Duty(FTM0, FTM_CH7,speed_m);
  }
  else if(com==1) //右电机反转
  {
    gpio_set(PTE4,HIGH);
    gpio_set(PTE7,LOW);
    FTM_PWM_Duty(FTM0, FTM_CH7,speed_m);
  }
}


/*********************************   虚拟示波器	**************************************/
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
  uart_putchar(FIRE_PORT,databuf[i]);
} 


/*********************************   二值化图像解压	**************************************/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*********************************   边线提取	**************************************/
void find_edgeb()
{
  for(i=0;i<Front_Line ;i++)
  {
    for(j=COLUMN/2;j>0;j--) //前几行从中间向右边
    {
      if(Buffer[i][j]==1&&Buffer[i][j-1]==0) //找到右边界
      {
        right=j;
        right_flag++;
        break;
      }
    }
    if(j<=0)
    {
      right=1;
    }
    Line_Right[i]=right;
    for(j=COLUMN/2;j<COLUMN-1;j++) //从中间向左边
    {
      if(Buffer[i][j]==1&&Buffer[i][j+1]==0) //找到左边界
      {
        left=j;
        left_flag++;
        break;      
      }
    }
    if(j>=COLUMN-1)
    {
      left=COLUMN;
    }
    Line_Left[i]=left;
    Line_Center[i]=(Line_Left[i]+Line_Right[i])/2; 
    if((i>5)&&(Line_Left[i-1]+4>Line_Left[i])&&(Line_Left[i-1]<Line_Left[i]+5)&&(Line_Right[i-1]+5>Line_Right[i])&&(Line_Right[i-1]<Line_Right[i]+4)) //前几行中找到可靠行
    {
      neardot=i;
      break;
    }
  } 
  for(;i<ROW;i++)
  {
    left_flag_l=0,left_flag_r=0,right_flag_l=0,right_flag_r=0;
    for(j=right;j<smaller(right+Search_Length,COLUMN-1);j++)                        //从上一行右边界的对应下方向左边继续搜索，范围Search_Length
    {
      if(Buffer[i][j]==0&&Buffer[i][j+1]==1) //找到右边界
      {
        right_left=j;
        right_flag_l++;
        break;
      }
    }
    for(j=right;j>bigger(right-Search_Length,0);j--)//向右搜索
    {
      if(Buffer[i][j]==1&&Buffer[i][j-1]==0) //找到右边界
      {
        right_right=j;
        right_flag_r++;
        break;
      }
    }
    if(right_flag_l==0&&right_flag_r==0)
    {
      break;
    }
    if(right_flag_l==1&&right_flag_r==1)
    {
      if(right_left+right_right<2*right)                  //选离上一行黑点最近的点作为此行的边界
      {
        right=right_left;
      }
      else
      {
        right=right_right;
      }
    }
    if(right_flag_l==1&&right_flag_r==0){ right=right_left;}
    if(right_flag_r==1&&right_flag_l==0){ right=right_right;}
    Line_Right[i]=right;
    ////////搜索左边界/////////////
    for(j=left;j>bigger(left-Search_Length,0);j--) //向左边的右边搜索
    {
      if(Buffer[i][j-1]==1&&Buffer[i][j]==0) //找到左边界
      {
        left_right=j;
        left_flag_r++;
        break;
      }
    } 
    for(j=left;j<smaller(left+Search_Length,COLUMN-1);j++)                  //从上一行左边界的对应下方向左边继续搜索，范围Search_Length
    {
      if(Buffer[i][j]==0&&Buffer[i][j-1]==1) //找到左边界
      {
        left_left=j;
        left_flag_l++;
        break;
      }
    }
    if(left_flag_l==1&&left_flag_r==1)
    {
      if(left_left+left_right<2*left)
      { 
        left=left_left;
      }
      else
      {
        left=left_right;
      }
    }
    if(left_flag_l==1&&left_flag_r==0){left=left_left;}
    if(left_flag_l==0&&left_flag_r==1){left=left_right;}
    if(left_flag_l==0&&left_flag_r==0)
    break;
    Line_Left[i]=left;
    Line_Center[i]=(Line_Left[i]+Line_Right[i])/2;
  }
}


/*********************************   读取AD值	**************************************/
void read_AD()
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    if(i==0)
    {
      AD_az_totle = adc_once(ADC0_SE16, ADC_16bit);
      AD_gy_totle = adc_once(ADC1_SE16, ADC_16bit);
      AD_az_max   = AD_az_totle;
      AD_az_min   = AD_az_totle;
      AD_gy_max	  = AD_gy_totle;
      AD_gy_min	  = AD_gy_totle;
    }
    else
    {
      AD_az = adc_once(ADC0_SE16, ADC_16bit);
      AD_gy = adc_once(ADC1_SE16, ADC_16bit);
      
      AD_az_totle += AD_az;
      AD_gy_totle += AD_gy;
      
      if(AD_az>AD_az_max) AD_az_max=AD_az;
      else if(AD_az<AD_az_min) AD_az_min=AD_az;
      
      if(AD_gy>AD_gy_max) AD_gy_max=AD_gy;
      else if(AD_gy<AD_gy_min) AD_gy_min=AD_gy;
    }
  }
  
  AD_az   = (AD_az_totle-AD_az_max-AD_az_min)/6;
  AD_gy	  = (AD_gy_totle-AD_gy_max-AD_gy_min)/6; 
}


/*********************************   角度计算	**************************************/
void angle_calculate()
{
//  uint az,gy;
//  float R_Z = 180/(Z_Max - Z_Min);

  
  Angle_Z = (C_Z-AD_az)*R_Z;
  Angle_G = (AD_gy-C_Gyro)*R_Gyro;
  Angle_AG = Angle_AG+(((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;
  Angle_GG = Angle_GG+Angle_G*0.005;
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

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       野火 DMA 采集摄像头 实验
                注意，此例程 bus频率设为100MHz(50MHz bus频率会太慢而导致没法及时采集图像)
 */
void  main(void)
{
  DisableInterrupts;
  
  NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
  NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
  NVIC_SetPriority(PIT0_IRQn,1);          //配置优先级
  NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
  
  IO_init();
  AD_init();
  
  FTM_QUAD_Init(FTM1);                        //FTM1 正交解码初始化
  FTM_QUAD_Init(FTM2);                        //FTM2 正交解码初始化

  camera_init(imgbuff);
  
  //配置中断复位函数
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断复位函数为 PORTA_IRQHandler
  
  pit_init_us(PIT0, 1000);                                //初始化PIT0，定时时间为： 1000ms
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
  enable_irq (PIT0_IRQn);                                 //使能PIT0中断
  
  EnableInterrupts;
  
  while(1)
  {
//    read_AD();
//    angle_calculate();
//    Uart_output(AD_az);
    
    camera_get_img();                                   //摄像头获取图像
    img_extract(img, imgbuff,CAMERA_SIZE);               //解压图像

    OutData[0]=Angle_Z;
    OutData[1]=Angle_GG;
    OutData[2]=Angle_AG;
    OutPut_Data();

//    printf("%d\t%d\n",AD_az,AD_gy);
//    DELAY_MS(10);
  }
}


/*********************************   定时器中断 	**************************************/
void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);          //清除定时器标志位
  PIT_flag0++;
  g_nSpeedControlPeriod ++;
//  SpeedControlOutput();
  if(PIT_flag0==5)
  {
    PIT_flag0=0;
    PACNT_flag++;
    if(PACNT_flag==4)
    {
      M_PACNT_left=FTM_QUAD_get(FTM1);
      M_PACNT_right=FTM_QUAD_get(FTM2);
      FTM_QUAD_clean(FTM1);
      FTM_QUAD_clean(FTM2);
    }
    
  } 
  else if(PIT_flag0==1)
  {
//    gpio_set(PTD1,LOW);
    read_AD();
    angle_calculate();
//    AngleControl();
//    MotorOutput();
//    gpio_set(PTD1,HIGH);
  }
  else if(PIT_flag0==2)
  {
//    gpio_set(PTD1,LOW);
/*    g_nSpeedControlCount ++;
    if(g_nSpeedControlCount >= SPEED_CONTROL_COUNT)
    {
      SpeedControl();
      g_nSpeedControlCount = 0;
      g_nSpeedControlPeriod = 0;
    } */ 
//    gpio_set(PTD1,HIGH);
  }
  else if(PIT_flag0==3)
  {
//    gpio_set(PTD1,LOW);
//    gpio_set(PTD1,HIGH);
  } 
  else if(PIT_flag0==4)
  {
//    gpio_set(PTD1,LOW);
//    gpio_set(PTD1,HIGH); 
  }
}


/*********************************   行场中断	**************************************/
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}


/*********************************   DMA中断	**************************************/
void DMA0_IRQHandler()
{
    camera_dma();
}