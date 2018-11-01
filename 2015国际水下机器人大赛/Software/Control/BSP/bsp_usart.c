#include "bsp_usart.h"
#include "string.h"



    

extern float AHRS_IMU_Pitch;
extern float AHRS_IMU_Roll;
extern float AHRS_IMU_Yaw;

extern int16_t AHRS_MPU6050_ACCEL_Last_X;//加速度仪 融合最终 X轴 数据
extern int16_t AHRS_MPU6050_ACCEL_Last_Y;//加速度仪 融合最终 Y轴 数据
extern int16_t AHRS_MPU6050_ACCEL_Last_Z;//加速度仪 融合最终 Z轴 数据

extern int16_t AHRS_MPU6050_GYRO_Last_X;//陀螺仪 融合最终 X轴 数据
extern int16_t AHRS_MPU6050_GYRO_Last_Y;//陀螺仪 融合最终 Y轴 数据
extern int16_t AHRS_MPU6050_GYRO_Last_Z;//陀螺仪 融合最终 Z轴 数据


extern float AHRS_HMC5883L_MAG_Last_X;//电子罗盘 融合最终 X轴 数据
extern float AHRS_HMC5883L_MAG_Last_Y;//电子罗盘 融合最终 Y轴 数据
extern float AHRS_HMC5883L_MAG_Last_Z;//电子罗盘 融合最终 Z轴 数据


/*
    float Roll
    float Yaw
    float Pitch
    
    int16_t AX
    int16_t AY
    int16_t AZ
   
    int16_t GX
    int16_t GY
    int16_t GZ
    
    int16_t MX
    int16_t MY
    int16_t MZ
    
    
    
    
    
*/



//=====================================================================
//                      匿名调试信息
//=====================================================================

#define BSP_USART1_SendBuffer_ANO_SIZE 64

uint8_t BSP_USART1_SendBuffer_ANO[BSP_USART1_SendBuffer_ANO_SIZE];


void Data_Send_Status(void)
{
	
}


void Data_Send_Senser(void)
{
	
}





void DMA_DataUpdate(void)
{
    uint8_t data_to_send[32];
    
    u8 datacount = 0;
    vs16 _temp;
    vs32 _temp2 = 0;

    u8 i;
    u8 sum = 0;
    u8 _cnt=0;
    
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(AHRS_IMU_Roll*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(AHRS_IMU_Pitch*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	//_temp = (int)(Q_ANGLE.YAW*100);
	_temp = (int)(AHRS_IMU_Yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = 0;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
		
	data_to_send[_cnt++]=0xA1;
	
	data_to_send[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;

	//Uart1_Put_Buf(data_to_send,_cnt);
    
    memcpy(BSP_USART1_SendBuffer_ANO,&data_to_send[0], _cnt+4);
    
    
    datacount = _cnt+4;
    
    
    
    _cnt = 0;
    

	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_ACCEL_Last_X);  //?8?
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_ACCEL_Last_X);  //?8?
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_ACCEL_Last_Y);
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_ACCEL_Last_Y);
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_ACCEL_Last_Z);
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_ACCEL_Last_Z);
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_GYRO_Last_X);
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_GYRO_Last_X);
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_GYRO_Last_Y);
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_GYRO_Last_Y);
	data_to_send[_cnt++]=BYTE1(AHRS_MPU6050_GYRO_Last_Z);
	data_to_send[_cnt++]=BYTE0(AHRS_MPU6050_GYRO_Last_Z);
	data_to_send[_cnt++]=BYTE1(AHRS_HMC5883L_MAG_Last_X);
	data_to_send[_cnt++]=BYTE0(AHRS_HMC5883L_MAG_Last_X);
	data_to_send[_cnt++]=BYTE1(AHRS_HMC5883L_MAG_Last_Y);
	data_to_send[_cnt++]=BYTE0(AHRS_HMC5883L_MAG_Last_Y);
	data_to_send[_cnt++]=BYTE1(AHRS_HMC5883L_MAG_Last_Z);
	data_to_send[_cnt++]=BYTE0(AHRS_HMC5883L_MAG_Last_Z);
	
	data_to_send[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	//Uart1_Put_Buf(data_to_send,_cnt);
    
     memcpy(&BSP_USART1_SendBuffer_ANO[0 + datacount],&(data_to_send[0]), _cnt+4);
    
}




//=====================================================================
//                      调试信息输出
//=====================================================================


uint8_t UploadData_Buffer[43];

//帧头0xAA 0x55
//帧尾0x55 0xAA
//功能帧



//typedef struct//25
//{
//    float _0_Pitch;//4
//    float _1_Roll;//4
//    float _2_Yaw;//4
//    
//    
//}agle8_t;




//typedef union //32-7 = 25B
//{
//    agle8_t _0_Angle;
//    
//    uint8_t _8_Reserved[25];
//}updt8_t;



/******************************CRC_CCITT余式表*********************************/
uint16_t crc_tab[16]={
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    };

/*******************************************************************************                            
函数：uint16_t cal_CRC(uint8_t *ptr,uint8_t len)
形参：ptr:形参类型:unsigned char*；形参功能：待校验数据的首地址；
      len:形参类型:unsigned char;  形参功能：待校验数据的长度；
返回：返回类型：unsigned int;
功能：CRC_CCITT数据校验函数
*******************************************************************************/
uint16_t cal_CRC(uint8_t *ptr,uint8_t len)
{
    uint16_t crc;
    uint8_t  dat;   
    crc = 0x00;
    while(len--!=0)
    {
        dat   = ((uint8_t)((crc/256))/16 & 0x000f);  //暂存 CRC 的高四位
        crc   = (uint16_t)((crc<<4)&0xfff0);         //CRC 右移 4 位，相当于取 CRC 的低 12 位）
        crc  ^= crc_tab[dat^(*ptr/16)];              //CRC 的高 4 位和本字节的前半字节相加后查表计算 CRC，然后加上上一次 CRC 的余数
        dat   = ((uint8_t)((crc/256))/16 & 0x000f);  //暂存 CRC 的低 4 位
        crc   = (uint16_t)((crc<<4)&0xfff0);         //CRC 右移 4 位，相当于 CRC 的低 12 位）
        crc  ^= crc_tab[dat^(*ptr&0x0f)];            //CRC 的高 4 位和本字节的后半字节相加后查表计算 CRC，然后再加上上一次 CRC 的余数
        ptr++;
    }
    return(crc);
}



UploadData_Type UploadData;
UploadData_Type *pUploadData = &UploadData;

extern volatile float sampleFreq;

void DMA_Update_UploadData(void)
{
    uint8_t data[39];
    uint16_t crc_value;
    bool status[3];
    
    if (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)) status[0] = true; else status[0] = false;
    if (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14)) status[1] = true; else status[1] = false;
    if (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15)) status[2] = true; else status[2] = false;
    
    UploadData._00_DataHead1 = 0xAA;
    UploadData._01_DataHead2 = 0x55;
    UploadData._02_Function  = 0x01;
    UploadData._03_Pitch     = AHRS_IMU_Pitch;
    UploadData._04_Roll      = AHRS_IMU_Roll;
    UploadData._05_Yaw       = AHRS_IMU_Yaw;
    UploadData._06_Ax        = AHRS_MPU6050_ACCEL_Last_X;
    UploadData._07_Ay        = AHRS_MPU6050_ACCEL_Last_Y;
    UploadData._08_Az        = AHRS_MPU6050_ACCEL_Last_Z;
    UploadData._09_Gx        = AHRS_MPU6050_GYRO_Last_X;
    UploadData._10_Gy        = AHRS_MPU6050_GYRO_Last_Y;
    UploadData._11_Gz        = AHRS_MPU6050_GYRO_Last_Z;
    UploadData._12_Mx        = AHRS_HMC5883L_MAG_Temp_X;
    UploadData._13_My        = AHRS_HMC5883L_MAG_Temp_Y;
    UploadData._14_Mz        = AHRS_HMC5883L_MAG_Temp_Z;
    UploadData._15_Speed     = sampleFreq;
    UploadData._16_LED1      = status[0];
    UploadData._17_LED2      = status[1];
    UploadData._18_LED3      = status[2];
    
    memcpy(data,&UploadData,39);
    crc_value = cal_CRC(&data[3],36);
    
    UploadData._19_CRC1      = crc_value/256;
    UploadData._20_CRC2      = crc_value%256;
    UploadData._21_DataTail1 = 0x55;
    UploadData._22_DataTail2 = 0xAA;
        
        
    memcpy(&UploadData_Buffer,&UploadData,43);

}














void BSP_USART1_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
		
	
	
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 500000;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		USART_Cmd(USART1, ENABLE);
        
        USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_Init(&NVIC_InitStructure);
        
        
	
		/*开启DMA时钟*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		/*设置DMA源：串口数据寄存器地址*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;	   

		/*内存地址(要传输的变量的指针)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)UploadData_Buffer;

		/*方向：从内存到外设*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = 43;

		/*外设地址不增*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*内存地址自增*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*外设数据单位*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*内存数据单位 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMA模式：不断循环*/
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Circular;	 

		/*优先级：中*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*禁止内存到内存的传输	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*配置DMA1的4通道*/		   
		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
		
		/*使能DMA*/
		DMA_Cmd (DMA1_Channel4,ENABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断
        
        USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}








































