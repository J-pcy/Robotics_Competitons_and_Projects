#include "bsp_usart.h"
#include "string.h"



    

extern float AHRS_IMU_Pitch;
extern float AHRS_IMU_Roll;
extern float AHRS_IMU_Yaw;

extern int16_t AHRS_MPU6050_ACCEL_Last_X;//���ٶ��� �ں����� X�� ����
extern int16_t AHRS_MPU6050_ACCEL_Last_Y;//���ٶ��� �ں����� Y�� ����
extern int16_t AHRS_MPU6050_ACCEL_Last_Z;//���ٶ��� �ں����� Z�� ����

extern int16_t AHRS_MPU6050_GYRO_Last_X;//������ �ں����� X�� ����
extern int16_t AHRS_MPU6050_GYRO_Last_Y;//������ �ں����� Y�� ����
extern int16_t AHRS_MPU6050_GYRO_Last_Z;//������ �ں����� Z�� ����


extern float AHRS_HMC5883L_MAG_Last_X;//�������� �ں����� X�� ����
extern float AHRS_HMC5883L_MAG_Last_Y;//�������� �ں����� Y�� ����
extern float AHRS_HMC5883L_MAG_Last_Z;//�������� �ں����� Z�� ����


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
//                      ����������Ϣ
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
//                      ������Ϣ���
//=====================================================================


uint8_t UploadData_Buffer[43];

//֡ͷ0xAA 0x55
//֡β0x55 0xAA
//����֡



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



/******************************CRC_CCITT��ʽ��*********************************/
uint16_t crc_tab[16]={
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    };

/*******************************************************************************                            
������uint16_t cal_CRC(uint8_t *ptr,uint8_t len)
�βΣ�ptr:�β�����:unsigned char*���βι��ܣ���У�����ݵ��׵�ַ��
      len:�β�����:unsigned char;  �βι��ܣ���У�����ݵĳ��ȣ�
���أ��������ͣ�unsigned int;
���ܣ�CRC_CCITT����У�麯��
*******************************************************************************/
uint16_t cal_CRC(uint8_t *ptr,uint8_t len)
{
    uint16_t crc;
    uint8_t  dat;   
    crc = 0x00;
    while(len--!=0)
    {
        dat   = ((uint8_t)((crc/256))/16 & 0x000f);  //�ݴ� CRC �ĸ���λ
        crc   = (uint16_t)((crc<<4)&0xfff0);         //CRC ���� 4 λ���൱��ȡ CRC �ĵ� 12 λ��
        crc  ^= crc_tab[dat^(*ptr/16)];              //CRC �ĸ� 4 λ�ͱ��ֽڵ�ǰ���ֽ���Ӻ������ CRC��Ȼ�������һ�� CRC ������
        dat   = ((uint8_t)((crc/256))/16 & 0x000f);  //�ݴ� CRC �ĵ� 4 λ
        crc   = (uint16_t)((crc<<4)&0xfff0);         //CRC ���� 4 λ���൱�� CRC �ĵ� 12 λ��
        crc  ^= crc_tab[dat^(*ptr&0x0f)];            //CRC �ĸ� 4 λ�ͱ��ֽڵĺ���ֽ���Ӻ������ CRC��Ȼ���ټ�����һ�� CRC ������
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
        
        
	
		/*����DMAʱ��*/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

		/*����DMAԴ���������ݼĴ�����ַ*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;	   

		/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)UploadData_Buffer;

		/*���򣺴��ڴ浽����*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	

		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = 43;

		/*�����ַ����*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*�ڴ��ַ����*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*�������ݵ�λ*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*�ڴ����ݵ�λ 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMAģʽ������ѭ��*/
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Circular;	 

		/*���ȼ�����*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*����DMA1��4ͨ��*/		   
		DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
		
		/*ʹ��DMA*/
		DMA_Cmd (DMA1_Channel4,ENABLE);					
		//DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
        
        USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}








































