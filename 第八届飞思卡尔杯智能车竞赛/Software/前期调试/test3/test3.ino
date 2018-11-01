#define RunTime 7

#define car_Angle_set 0
#define Car_Angle_Speed_set 0

#define Moto_Out_Max 255
#define Moto_Out_Min -255

//MPU 6050
int16_t az,gy;

float C_Z = 303.604;//Z轴零点偏移量
float C_Gyro = 413;//陀螺仪零点偏移量
float Z_Min = 147.816;//最小极值
float Z_Max = 474.205;//最大极值
float T_Z = 0.3;//Z轴角度补偿时间常数
float R_Z = 180/(Z_Max - Z_Min);//Z轴比例
float R_Gyro = 4.8;//陀螺仪比例
unsigned long T_Now =0;//系统当前时间
unsigned long T_Last;//上次时间
float Angle_Z,Angle_G,Angle_AG,Angle_GG;
int i;
float GYRO;

static float Angle_Control_P=20;
static float Angle_Control_D=1.5;
static float g_fAngleControlOut;
static float g_fLeftMotorOut;
static float g_fRightMotorOut;
static float g_fSpeedControlOut=0;
static float g_fDirectionControlOut=0;


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

    az = analogRead(0);
    gy = analogRead(1);

    Angle_Z = (az-C_Z)*R_Z;//加速度计 角度计算 （读取值-偏移量）*比例  单位：°
    Angle_G = (gy-C_Gyro)*R_Gyro;//陀螺仪采样 （采样值-偏移量）*比例 单位：°/s
    Angle_AG = Angle_AG + (((Angle_Z-Angle_AG)*1/T_Z)+Angle_G)*0.005;//滤波
    Angle_GG = Angle_GG + Angle_G*0.005;//陀螺仪对X轴积分 得出角度。
       OutData[0]=Angle_Z;
       OutData[1]=Angle_GG;
       OutData[2]=Angle_AG;
       OutPut_Data();
}

void TMR0_IRQHandler(void)   //1毫秒定时程序
{
        timee++;
        if(timee==5)
        {
          timee=0;
        }
	else //读取传感器值、取平均值
	{	
              digitalWrite(RunTime,HIGH);
              read_AD();
              digitalWrite(RunTime,LOW);
	}

}

void setup()
{
  Serial.begin(9600);

  pinMode(RunTime,OUTPUT);
  
}
void loop()
{

    unsigned long currentMillis = millis();//读取当前时间值
    if(currentMillis - previousMillis >= interval) 
    {
        previousMillis = currentMillis;  
        TMR0_IRQHandler();
        
    }
}

