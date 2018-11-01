//定义全局变量
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;

//#define acceler 0
//#define gyro 1
#define RunTime 7

#define Moto_R 0
#define Moto_RA 8
#define Moto_RB 9
#define Moto_L 1
#define Moto_LA 10
#define Moto_LB 11

//MPU 6050
int16_t ax, ay, az;
int16_t gx, gy, gz;

float C_Z = -2069.22;//Z轴零点偏移量
float C_Gyro = 42.19;//陀螺仪零点偏移量
float Z_Min = -19216.58;//最小极值
float Z_Max = 14116.15;//最大极值
float T_Z = 3;//Z轴角度补偿时间常数
float R_Z = 180/(Z_Max - Z_Min);//Z轴比例
float R_Gyro = 0.0281;//陀螺仪比例
unsigned long T_Now =0;//系统当前时间
unsigned long T_Last;//上次时间
float Angle_Z,Angle_G,Angle_AG,Angle_GG;
int i;
float GYRO;
 
bool blinkState = false;


float OutData[4]={0};//虚拟示波器4通道参数




long previousMillis = 0;//过去的时间
long interval = 1;//间隔

char timee=0;

//Kalman滤波

//-------------------------------------------------------
static  float Q_angle=0.001, Q_gyro=0.003, R_angle=0.5, dt=0.005;
			//注意：dt的取值为kalman滤波器采样时间;
static float Pk[2][2] = { {1, 0 }, {0, 1 }};
	
static float Pdot[4] ={0,0,0,0};

static char C_0 = 1;
static float angle, angle_dot; 		//外部需要引用的变量
static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------

/*********************************   读取AD值	**************************************/
static unsigned int AD_acceler_max,AD_acceler_min;
static unsigned int AD_gyro_max,AD_gyro_min;
static unsigned int AD_acceler_totle,AD_gyro_totle,AD_acceler,AD_gyro;
//卡尔曼滤波，就算倾角和角加速度
static float gyro_val;
static float acceler_val;
static int offset=0;//-5
static int off_gy=0;
static int K_gy=1.0;



//-------------------------------------------------------
//计算PWM输出值
//车辆直径：42mm; 27*128pulse/rev; 1m=26192pulses;
//-------------------------------------------------------	
//static int speed_diff,speed_diff_all,speed_diff_adjust;
//static float K_speed_P,K_speed_I;
static float K_voltage,K_angle,K_angle_dot,K_position,K_position_dot;
static float K_angle_AD,K_angle_dot_AD,K_position_AD,K_position_dot_AD;
static float position_R,position_R_dot,position_L,position_L_dot;
static float position_R_dot_filter,position_L_dot_filter;
static float PWM_R,PWM_L;
static float speed_output_LH,speed_output_RH;
static float Speed_R_Need,Speed_L_Need;
static float Moto_PWM_R_New,Moto_PWM_R_Old,Moto_PWM_L_New,Moto_PWM_L_Old;

int R_Speed_need=0;
int L_Speed_need=0;

static int speed_real_LH=0;//左轮电机计数
static int speed_real_RH=0;//右轮电机计数

static float PWM_R_Output,PWM_L_Output;


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









void Kalman_Filter(float angle_m,float gyro_m)			
{
	angle+=(gyro_m-q_bias) * dt;


	Pdot[0]=Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1]=- Pk[1][1];
	Pdot[2]=- Pk[1][1];
	Pdot[3]=Q_gyro;
	
	Pk[0][0] += Pdot[0] * dt;
	Pk[0][1] += Pdot[1] * dt;
	Pk[1][0] += Pdot[2] * dt;
	Pk[1][1] += Pdot[3] * dt;
	
	
	angle_err = angle_m - angle;
	
	
	PCt_0 = C_0 * Pk[0][0];
	PCt_1 = C_0 * Pk[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * Pk[0][1];

	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	
	
	angle	+= K_0 * angle_err;
	q_bias	+= K_1 * angle_err;
	angle_dot = gyro_m-q_bias;
}










void read_AD()
{
    unsigned char i;
    for(i=0;i<8;i++)
    {
              if(i==0)
		{
                        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//                        Serial.print("a/g:\t");
//                        Serial.print(ax/16384); Serial.print("\t");    
//                        Serial.println(gy/131);   		
                        AD_acceler_totle = az/16384;
			AD_gyro_totle    = gy/131;
			AD_acceler_max   = AD_acceler_totle;
			AD_acceler_min   = AD_acceler_totle;
			AD_gyro_max	     = AD_gyro_totle;
			AD_gyro_min	     = AD_gyro_totle;
		}
		else
		{
                        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);	
			AD_acceler = az/16384;
			AD_gyro    = gy/131;

			AD_acceler_totle += AD_acceler;
			AD_gyro_totle    += AD_gyro;

			if(AD_acceler>AD_acceler_max) AD_acceler_max=AD_acceler;
			else if(AD_acceler<AD_acceler_min) AD_acceler_min=AD_acceler;

			if(AD_gyro>AD_gyro_max) AD_gyro_max=AD_gyro;
			else if(AD_gyro<AD_gyro_min) AD_gyro_min=AD_gyro;
		}
	}

	AD_acceler=	(AD_acceler_totle-AD_acceler_max-AD_acceler_min)/6;
	AD_gyro	  = (AD_gyro_totle-AD_gyro_max-AD_gyro_min)/6;
//       OutData[0]=AD_acceler;
//       OutData[1]=AD_gyro;
//       OutPut_Data();

    Angle_Z = (az-C_Z)*R_Z;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°
    Angle_G = (gy-C_Gyro)*R_Gyro;//陀螺仪采样 （采样值-偏移量）*比例 单位：°/s
    Angle_AG = Angle_AG + (((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;//滤波
    Angle_GG = Angle_GG + Angle_G*0.005;//陀螺仪对X轴积分 得出角度。
       OutData[0]=Angle_Z;
       OutData[1]=Angle_GG;
       OutData[2]=Angle_AG;
       OutPut_Data();
}





void AD_calculate(void)
{
	acceler_val=AD_acceler;
	gyro_val=AD_gyro;
//        Serial.print("a/g:\t");
//        Serial.print(acceler_val); Serial.print("\t");
//        Serial.println(gyro_val);
//        OutData[2]=acceler_val;
	AD_acceler=0;
	AD_gyro=0;

//	if(acceler_val<-19216.58+offset)acceler_val=-19216.58+offset;
//	if(acceler_val>14116.15+offset)acceler_val=14116.15+offset;

	acceler_val-=(301+offset);			//前倾，值小，后倒，值大

	acceler_val*=0.0054001;		//系数换算：（5.03/1024）/(0.8);0.0061401
	acceler_val=asin(acceler_val);

	gyro_val-=(347+off_gy);				//前倾，值增大，后倒，减小
	gyro_val=gyro_val*(-1);
	gyro_val*=(0.0255788/K_gy);			//角速度系数：(3.14/180)* （5.03/1024/(0.00068*A)）=0；A=5

//        OutData[0]=acceler_val*100;
//        OutData[1]=gyro_val*100;
 	Kalman_Filter(acceler_val,gyro_val);            //将参数送入卡尔曼滤波器  
//        OutData[2]=angle*100;
//        OutData[3]=angle_dot*100;
//        OutPut_Data();
//        Serial.print("a/g:\t");
//        Serial.print(acceler_val); Serial.print("\t");
//        Serial.println(gyro_val);


}

void PWM_calculate()
{
	K_angle_AD=133;	   
	K_angle_dot_AD=2.13;
	K_position_AD=0;
	K_position_dot_AD=0;

//	K_angle_AD=133;	   
//	K_angle_dot_AD=2.13;
//	K_position_AD=0.3;
//	K_position_dot_AD=27.3;

	position_R_dot=(speed_real_RH-R_Speed_need);
	position_L_dot=(speed_real_LH-L_Speed_need);
	
	position_R_dot_filter*=0.85;		//车轮速度滤波
	position_R_dot_filter+=position_R_dot*0.15;	
	position_L_dot_filter*=0.85;		//车轮速度滤波
	position_L_dot_filter+=position_L_dot*0.15;	
	
	
	if (position_R<-3456)		//防止位置误差过大导致的不稳定
	{
		position_R=-3456;
	}
	else if  (position_L>3456)
	{
		position_L=3456;
	}
	
	PWM_R =(K_angle*angle*K_angle_AD
		+ K_angle_dot*angle_dot *K_angle_dot_AD	
		- (position_R_dot_filter *K_position_dot_AD) );

	PWM_L =(K_angle*angle*K_angle_AD
		+ K_angle_dot*angle_dot *K_angle_dot_AD	
		- (position_L_dot_filter *K_position_dot_AD) );


		
	Moto_PWM_R_Old=Moto_PWM_R_New;
        Moto_PWM_R_New=PWM_R;
	Moto_PWM_L_Old=Moto_PWM_L_New;
        Moto_PWM_L_New=PWM_L;

	speed_output_RH = PWM_R;
	speed_output_LH = PWM_L;
}



/*********************************电机正反转控制*****************************************
********说明
********com  =  0   正转
********com  =  1   反转 
********MotoR       右电机
********MotoL       左电机
********speed      0---Moto_PWM(慢---快）
*****************************************************************************************/ 
void MotoR(unsigned char com,unsigned int speed_m)
{


	if(com==0)		//右电机正转		 		
	{ 
	digitalWrite (Moto_RA,0);
        digitalWrite (Moto_RB,1);
        analogWrite (Moto_R,speed_m);
	}
	else if(com==1) //右电机反转
	{ 
         digitalWrite (Moto_RB,0); 
	 digitalWrite (Moto_RA,1);
         analogWrite (Moto_R,speed_m);

	}


}
void MotoL(unsigned char com,unsigned int speed_m)
{


	if(com==0)		//左电机正转		 		
	{
	digitalWrite (Moto_LA,0);
        digitalWrite (Moto_LB,1);
        analogWrite (Moto_L,speed_m);
	}
	else if(com==1) //左电机反转
	{
         digitalWrite (Moto_LB,0); 
	 digitalWrite (Moto_LA,1);
         analogWrite (Moto_L,speed_m);
	}

}















void PWM_output(float speed_RH,float speed_LH)
{
	if(speed_LH>0)speed_LH+=0;
	if(speed_LH<0)speed_LH-=0;

	if(speed_RH>0)speed_RH+=0;
	if(speed_RH<0)speed_RH-=0;	

	if(speed_RH>255)speed_RH=255;
	if(speed_RH<-255)speed_RH=-255;

	if(speed_LH>255)speed_LH=255;
	if(speed_LH<-255)speed_LH=-255;

	if(speed_LH>0)
		MotoL(1,(int)(speed_LH));
	else if(speed_LH<0)
	{
	 	speed_LH=speed_LH*(-1);
		MotoL(0,(int)(speed_LH));	
	}


	if(speed_RH>0)
		MotoR(1,(int)(speed_RH));
	else if(speed_RH<0)
	{
	 	speed_RH=speed_RH*(-1);
		MotoR(0,(int)(speed_RH));	
	}
	
}

void PWM_output_line(void)
{
	float delta_R,delta_L;
	delta_R=Moto_PWM_R_New-Moto_PWM_R_Old;
	delta_L=Moto_PWM_L_New-Moto_PWM_L_Old;

 	PWM_R_Output=delta_R*(timee+1)/5+Moto_PWM_R_Old;
	PWM_L_Output=delta_L*(timee+1)/5+Moto_PWM_L_Old;
	PWM_output(PWM_R_Output,PWM_L_Output);
}

void TMR0_IRQHandler(void)   //1毫秒定时程序
{
        timee++;
        PWM_output_line();
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
	else if(timee==3)//卡尔曼滤波，就算倾角和角加速度
	{
              digitalWrite(RunTime,HIGH);
              AD_calculate();
              digitalWrite(RunTime,LOW);


	}
	else if(timee==4)//获取电机速度，计算电机控制量
	{
              digitalWrite(RunTime,HIGH);
              PWM_calculate();
              speed_real_LH=0;
	      speed_real_RH=0;

//              OutData[0]=acceler_val*10;
//              OutData[1]=gyro_val*10;
//              OutData[2]=angle*10;
//              OutData[3]=analogRead(gyro);
        //      OutPut_Data();
              digitalWrite(RunTime,LOW);
	}

  
  
}


void setup()
{
  Wire.begin();
  Serial.begin(9600);
   
  accelgyro.initialize();

  pinMode(RunTime,OUTPUT);
  
  K_position=10;		//换算系数：(256/10) * (2*pi/(64*12))=0.20944；//256/10:电压换算至PWM，256对应10V；
  K_angle=57.3;		//换算系数：256/10 =25.6；
  K_position_dot=50;		//换算系数：(256/10) * (25*2*pi/(64*12))=20.944；
  K_angle_dot=57.3;		//换算系数：256/10 =25.6；
}
void loop()
{
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//    Serial.print("a/g:\t");
//    Serial.print(ax/16384); Serial.print("\t");
//    Serial.print(gy/131); Serial.print("\t");
    unsigned long currentMillis = millis();//读取当前时间值
    if(currentMillis - previousMillis >= interval) 
    {
        previousMillis = currentMillis;  
        TMR0_IRQHandler();
        
    }
    blinkState = !blinkState;
}
