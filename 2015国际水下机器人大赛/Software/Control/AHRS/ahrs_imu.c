#include "ahrs_imu.h"
#include "math.h"

#define AHRS_IMU_KP 2.00f
#define AHRS_IMU_KI 0.01f

#define AHRS_IMU_PI  ((float)3.1415926535)


//================================================================
/*
        IMU �㷨ģʽ��~
        
                = 1 :   ��ͨ
                = 2 :   �����˲�
                = 3 :   �ݶ��½���


*/
//================================================================
volatile uint8_t IMU_Mode = 3;



volatile float exInt;//X ������
volatile float eyInt;//Y ������
volatile float ezInt;//Z ������

volatile float q0;// ȫ����Ԫ��
volatile float q1;// ȫ����Ԫ��
volatile float q2;// ȫ����Ԫ��
volatile float q3;// ȫ����Ԫ��
volatile float q[4]; //����Ԫ��

volatile float integralFBhand;
volatile float handdiff;

volatile uint32_t lastUpdate; // �������ڼ��� ��λ us
volatile uint32_t now; // �������ڼ��� ��λ us
volatile uint32_t AHRS_Time_Counter;//AHRS��ʱ

float AHRS_IMU_Pitch;
float AHRS_IMU_Roll;
float AHRS_IMU_Yaw;



// ���� �����˲��㷨 �� �ݶ��½��� ================================����������
volatile float sampleFreq	= 338.0f ;///600.0f;//600.0f//600.0f		// sample frequency in Hz
#define betaDef		0.1f		// 2 * proportional gain
volatile float beta = betaDef;								// 2 * proportional gain (Kp)


//===============================================================
//												�����˲�
//===============================================================


#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain


volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)

volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki








float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void AHRS_IMU_Init(void)
{
    
    AHRS_HMC5883L_Calibrate_Read();
    
    
    q0 = 1.0f;  //��ʼ����Ԫ��
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
  	exInt = 0.0;
  	eyInt = 0.0;
  	ezInt = 0.0;
    //AHRS_Time_Counter = 0;//���ֵ
  	lastUpdate = micros();//AHRS_Time_Counter;//����ʱ��
  	now = micros();//AHRS_Time_Counter;
}

float AHRS_IMU_ACCEL_Last_X;//���ٶ��� �ں����� X�� ����
float AHRS_IMU_ACCEL_Last_Y;//���ٶ��� �ں����� Y�� ����
float AHRS_IMU_ACCEL_Last_Z;//���ٶ��� �ں����� Z�� ����

float AHRS_IMU_TEMPER_Last_O;//�ڲ��¶��� �ں�����  ����

float AHRS_IMU_GYRO_Last_X;//������ �ں����� X�� ����
float AHRS_IMU_GYRO_Last_Y;//������ �ں����� Y�� ����
float AHRS_IMU_GYRO_Last_Z;//������ �ں����� Z�� ����

float AHRS_IMU_MAG_Last_X;//�������� �ں����� X�� ����
float AHRS_IMU_MAG_Last_Y;//�������� �ں����� Y�� ����
float AHRS_IMU_MAG_Last_Z;//�������� �ں����� Z�� ����

void AHRS_IMU_GetSensor(void)
{
    AHRS_IMU_ACCEL_Last_X = (float) AHRS_MPU6050_ACCEL_Last_X;
    AHRS_IMU_ACCEL_Last_Y = (float) AHRS_MPU6050_ACCEL_Last_Y;
    AHRS_IMU_ACCEL_Last_Z = (float) AHRS_MPU6050_ACCEL_Last_Z;
    
    AHRS_IMU_GYRO_Last_X = ((float) AHRS_MPU6050_GYRO_Last_X)/32.8f;//ת�ɶ�ÿ��
    AHRS_IMU_GYRO_Last_Y = ((float) AHRS_MPU6050_GYRO_Last_Y)/32.8f;//ת�ɶ�ÿ��
    AHRS_IMU_GYRO_Last_Z = ((float) AHRS_MPU6050_GYRO_Last_Z)/32.8f;//ת�ɶ�ÿ��

    AHRS_IMU_MAG_Last_X = AHRS_HMC5883L_MAG_Last_X;
    AHRS_IMU_MAG_Last_Y = AHRS_HMC5883L_MAG_Last_Y;
    AHRS_IMU_MAG_Last_Z = AHRS_HMC5883L_MAG_Last_Z;
}




void IMU1(void)
{
		float gx = AHRS_IMU_GYRO_Last_X * AHRS_IMU_PI /180;
    float gy = AHRS_IMU_GYRO_Last_Y * AHRS_IMU_PI /180;
    float gz = AHRS_IMU_GYRO_Last_Z * AHRS_IMU_PI /180;
    float ax = AHRS_IMU_ACCEL_Last_X;
    float ay = AHRS_IMU_ACCEL_Last_Y;
    float az = AHRS_IMU_ACCEL_Last_Z;
    float mx = AHRS_IMU_MAG_Last_X;
    float my = AHRS_IMU_MAG_Last_Y;
    float mz = AHRS_IMU_MAG_Last_Z;
    
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez,halfT;
    float tempq0,tempq1,tempq2,tempq3;

    // �Ȱ���Щ�õõ���ֵ���
    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;          

    now = micros();  //��ȡʱ��
    if(now<lastUpdate){ //��ʱ��������ˡ�
    halfT =  ((float)(now + (0xffff- lastUpdate)) / 2000000.0f);
    }
    else	{
    halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//����ʱ��

    //halfT =  ((float)(AHRS_Time_Counter - 0) / 2000000.0f);
    
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //�ѼӼƵ���ά����ת�ɵ�λ������

    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

    /*
    ���ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
    �������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
    ���������vx\y\z����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������������λ������
    */
    // compute reference direction of flux
    hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
    hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
    hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz;     

    // estimated direction of gravity and flux (v and w)
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
    wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  

    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    /*
    axyz�ǻ����������ϵ�ϣ����ٶȼƲ����������������Ҳ����ʵ�ʲ����������������
    axyz�ǲ����õ�������������vxyz�����ݻ��ֺ����̬����������������������Ƕ��ǻ����������ϵ�ϵ�����������
    ������֮�������������������ݻ��ֺ����̬�ͼӼƲ��������̬֮�����
    ������������������������Ҳ�������������ˣ�����ʾ��exyz�����������������Ĳ����
    �����������Ծ���λ�ڻ�������ϵ�ϵģ������ݻ������Ҳ���ڻ�������ϵ�����Ҳ���Ĵ�С�����ݻ����������ȣ����������������ݡ���������Լ��ö�������һ�£����������ǶԻ���ֱ�ӻ��֣����Զ����ݵľ�������ֱ�������ڶԻ�������ϵ�ľ�����
    */
    if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
    exInt = exInt + ex * AHRS_IMU_KI * halfT;
    eyInt = eyInt + ey * AHRS_IMU_KI * halfT;	
    ezInt = ezInt + ez * AHRS_IMU_KI * halfT;

    // �ò���������PI����������ƫ
    gx = gx + AHRS_IMU_KP*ex + exInt;
    gy = gy + AHRS_IMU_KP*ey + eyInt;
    gz = gz + AHRS_IMU_KP*ez + ezInt;

    }

    // ��Ԫ��΢�ַ���
    tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // ��Ԫ���淶��
    norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
    q[0] = q0 = tempq0 * norm;
    q[1] = q1 = tempq1 * norm;
    q[2] = q2 = tempq2 * norm;
    q[3] = q3 = tempq3 * norm;
}






void MahonyAHRSupdateIMU(void) 
{
	
	float gx = AHRS_IMU_GYRO_Last_X * AHRS_IMU_PI /180;
	float gy = AHRS_IMU_GYRO_Last_Y * AHRS_IMU_PI /180;
	float gz = AHRS_IMU_GYRO_Last_Z * AHRS_IMU_PI /180;
	float ax = AHRS_IMU_ACCEL_Last_X;
	float ay = AHRS_IMU_ACCEL_Last_Y;
	float az = AHRS_IMU_ACCEL_Last_Z;
	
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;        

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;
	
		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// apply integral feedback
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// prevent integral windup
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q[0] = q0 *= recipNorm;
	q[1] = q1 *= recipNorm;
	q[2] = q2 *= recipNorm;
	q[3] = q3 *= recipNorm;
}


void IMU2(void)
{
	
	float gx = AHRS_IMU_GYRO_Last_X * AHRS_IMU_PI /180;
	float gy = AHRS_IMU_GYRO_Last_Y * AHRS_IMU_PI /180;
	float gz = AHRS_IMU_GYRO_Last_Z * AHRS_IMU_PI /180;
	float ax = AHRS_IMU_ACCEL_Last_X;
	float ay = AHRS_IMU_ACCEL_Last_Y;
	float az = AHRS_IMU_ACCEL_Last_Z;
	float mx = AHRS_IMU_MAG_Last_X;
	float my = AHRS_IMU_MAG_Last_Y;
	float mz = AHRS_IMU_MAG_Last_Z;
	
	float recipNorm;
  float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;  
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MahonyAHRSupdateIMU();
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;     

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;   

        // Auxiliary variables to avoid repeated arithmetic
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;   

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);  
	
		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		// Compute and apply integral feedback if enabled
		if(twoKi > 0.0f) {
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);
			gx += integralFBx;	// apply integral feedback
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	// prevent integral windup
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	qa = q0;
	qb = q1;
	qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx); 
	
	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q[0] = q0 *= recipNorm;
	q[1] = q1 *= recipNorm;
	q[2] = q2 *= recipNorm;
	q[3] = q3 *= recipNorm;
}


void MadgwickAHRSupdateIMU(void)
{
	float gx = AHRS_IMU_GYRO_Last_X * AHRS_IMU_PI /180;
	float gy = AHRS_IMU_GYRO_Last_Y * AHRS_IMU_PI /180;
	float gz = AHRS_IMU_GYRO_Last_Z * AHRS_IMU_PI /180;
	float ax = AHRS_IMU_ACCEL_Last_X;
	float ay = AHRS_IMU_ACCEL_Last_Y;
	float az = AHRS_IMU_ACCEL_Last_Z;


	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q[0] = q0 *= recipNorm;
	q[1] = q1 *= recipNorm;
	q[2] = q2 *= recipNorm;
	q[3] = q3 *= recipNorm;
}

void IMU3(void)
{
	
	float gx = AHRS_IMU_GYRO_Last_X * AHRS_IMU_PI /180;
	float gy = AHRS_IMU_GYRO_Last_Y * AHRS_IMU_PI /180;
	float gz = AHRS_IMU_GYRO_Last_Z * AHRS_IMU_PI /180;
	float ax = AHRS_IMU_ACCEL_Last_X;
	float ay = AHRS_IMU_ACCEL_Last_Y;
	float az = AHRS_IMU_ACCEL_Last_Z;
	float mx = AHRS_IMU_MAG_Last_X;
	float my = AHRS_IMU_MAG_Last_Y;
	float mz = AHRS_IMU_MAG_Last_Z;
	
  float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MadgwickAHRSupdateIMU();
		return;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mx;
		_2q0my = 2.0f * q0 * my;
		_2q0mz = 2.0f * q0 * mz;
		_2q1mx = 2.0f * q1 * mx;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / (float)sampleFreq);
	q1 += qDot2 * (1.0f / (float)sampleFreq);
	q2 += qDot3 * (1.0f / (float)sampleFreq);
	q3 += qDot4 * (1.0f / (float)sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q[0] = q0 *= recipNorm;
	q[1] = q1 *= recipNorm;
	q[2] = q2 *= recipNorm;
	q[3] = q3 *= recipNorm;
}

void AHRS_IMU_Update(void)
{
    if (IMU_Mode == 1)
        IMU1();
    else if(IMU_Mode == 2)
        IMU2();
    else if (IMU_Mode == 3)
        IMU3();	
}





void AHRS_IMU_Get_Angle(void)
{
    AHRS_IMU_GetSensor();
    AHRS_IMU_Update();
    
    AHRS_IMU_Yaw   = -atan2( 2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2] * q[2] - 2 * q[3] * q[3] + 1) * 180 / AHRS_IMU_PI; // yaw
    AHRS_IMU_Pitch = -asin (-2 * q[1] * q[3] + 2 * q[0] * q[2]                                        ) * 180 / AHRS_IMU_PI; // pitch
    AHRS_IMU_Roll  =  atan2( 2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1) * 180 / AHRS_IMU_PI; // roll
    
}
