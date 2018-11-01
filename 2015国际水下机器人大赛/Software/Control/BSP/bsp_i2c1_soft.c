#include "bsp_i2c1_soft.h"

void i2c_delayus(uint32_t us)
{
    uint32_t i;
    
    while(us--)
    {
        i=0;
        while(i--);
    }
}



void I2C_Soft_Init(uint8_t I2Cx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if (I2Cx == 1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else if (I2Cx == 2)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
}

void I2C_Soft_SDA_Set_OUT(uint8_t I2Cx)
{
    if (I2Cx == 1)
    {
        GPIOB->CRL&=0X0FFFFFFF;
        GPIOB->CRL|=0x30000000;
    }
    
    if (I2Cx == 2)
    {
        GPIOB->CRH&=0XFFFF0FFF;
        GPIOB->CRH|=0x00003000;
    }
}

void I2C_Soft_SDA_Set_IN(uint8_t I2Cx)
{
    if (I2Cx == 1)
    {
        GPIOB->CRL&=0X0FFFFFFF;
        GPIOB->CRL|=0x80000000;
    }
    
    if (I2Cx == 2)
    {
        GPIOB->CRH&=0XFFFF0FFF;
        GPIOB->CRH|=0x00008000;
    }
}

void I2C_Soft_SDA_OUT(uint8_t I2Cx,uint32_t data)
{
    if (I2Cx == 1)
        PBout(7) = data;
    if (I2Cx == 2)
        PBout(11) = data;
}

uint8_t I2C_Soft_SDA_IN(uint8_t I2Cx)
{
    if (I2Cx == 1)
        return PBin(7);
    if (I2Cx == 2)
        return PBin(11);
    return 0;
}

void I2C_Soft_SCL_OUT(uint8_t I2Cx, uint8_t data)
{
    if (I2Cx == 1)
        PBout(6) = data;
    else
        PBout(10) = data;
}

void I2C_Soft_Start(uint8_t I2Cx)
{
	I2C_Soft_SDA_Set_OUT(I2Cx);
	I2C_Soft_SDA_OUT(I2Cx,1);
	I2C_Soft_SCL_OUT(I2Cx,1);
	i2c_delayus(2);
 	I2C_Soft_SDA_OUT(I2Cx,0);
	i2c_delayus(2);
	I2C_Soft_SCL_OUT(I2Cx,0);
}

void I2C_Soft_Stop(uint8_t I2Cx)
{
    I2C_Soft_SDA_Set_OUT(I2Cx);
	I2C_Soft_SCL_OUT(I2Cx,0);
	I2C_Soft_SDA_OUT(I2Cx,0);
 	i2c_delayus(2);
	I2C_Soft_SCL_OUT(I2Cx,1);
	I2C_Soft_SDA_OUT(I2Cx,1);
	i2c_delayus(2);
}


uint8_t I2C_Soft_Wait_Ack(uint8_t I2Cx)
{
	uint8_t ucErrTime=0;
	I2C_Soft_SDA_Set_IN(I2Cx);
	I2C_Soft_SDA_OUT(I2Cx,1);i2c_delayus(1);
	I2C_Soft_SCL_OUT(I2Cx,1);i2c_delayus(1);
	while(I2C_Soft_SDA_IN(I2Cx))
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			I2C_Soft_Stop(I2Cx);
			return 1;
		}
	  i2c_delayus(1);
	}
	I2C_Soft_SCL_OUT(I2Cx,0);   
	return 0;  
} 

void I2C_Soft_Ack(uint8_t I2Cx)
{
	I2C_Soft_SCL_OUT(I2Cx,0);
	I2C_Soft_SDA_Set_OUT(I2Cx);
	I2C_Soft_SDA_OUT(I2Cx,0);
	i2c_delayus(1);
	I2C_Soft_SCL_OUT(I2Cx,1);
	i2c_delayus(1);
	I2C_Soft_SCL_OUT(I2Cx,0);
}

void I2C_Soft_NAck(uint8_t I2Cx)
{
	I2C_Soft_SCL_OUT(I2Cx,0);
	I2C_Soft_SDA_Set_OUT(I2Cx);
	I2C_Soft_SDA_OUT(I2Cx,1);
	i2c_delayus(1);
	I2C_Soft_SCL_OUT(I2Cx,1);
	i2c_delayus(1);
	I2C_Soft_SCL_OUT(I2Cx,0);
}

void I2C_Soft_Send_Byte(uint8_t I2Cx, uint8_t txd)
{
    u8 t;   
	I2C_Soft_SDA_Set_OUT(I2Cx); 	    
    I2C_Soft_SCL_OUT(I2Cx,0);
    for(t=0;t<8;t++)
    {              
        I2C_Soft_SDA_OUT(I2Cx,(txd&0x80)>>7);
        txd<<=1; 	  
		i2c_delayus(1);   
		I2C_Soft_SCL_OUT(I2Cx,1);
		i2c_delayus(1); 
		I2C_Soft_SCL_OUT(I2Cx,0);
		i2c_delayus(1);
    }	 
} 	 

uint8_t I2C_Soft_Read_Byte(uint8_t I2Cx,unsigned char ack)
{
	unsigned char i,receive=0;
	I2C_Soft_SDA_Set_IN(I2Cx);
    for(i=0;i<8;i++ )
	{
        I2C_Soft_SCL_OUT(I2Cx,0);
        i2c_delayus(1);
		I2C_Soft_SCL_OUT(I2Cx,1);
        receive<<=1;
        if(I2C_Soft_SDA_IN(I2Cx))receive++;   
		i2c_delayus(1); 
    }					 
    if (ack)
        I2C_Soft_Ack(I2Cx);
    else
        I2C_Soft_NAck(I2Cx);
    return receive;
}

unsigned char I2C_Soft_ReadOneByte(uint8_t I2Cx,unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;

	I2C_Soft_Start(I2Cx);	
	I2C_Soft_Send_Byte(I2Cx,I2C_Addr);
	res++;
	I2C_Soft_Wait_Ack(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,addr); res++;
	I2C_Soft_Wait_Ack(I2Cx);	  
	//IIC_Stop();
	I2C_Soft_Start(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,I2C_Addr+1); res++;
	I2C_Soft_Wait_Ack(I2Cx);
	res=I2C_Soft_Read_Byte(I2Cx,0);	   
    I2C_Soft_Stop(I2Cx);

	return res;
}


uint8_t I2C_Soft_ReadBytes(uint8_t I2Cx,u8 dev, u8 reg, u8 length, u8 *data)
{
    u8 count = 0;

	I2C_Soft_Start(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,dev);
	I2C_Soft_Wait_Ack(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,reg);
    I2C_Soft_Wait_Ack(I2Cx);	  
	I2C_Soft_Start(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,dev+1);
	I2C_Soft_Wait_Ack(I2Cx);

    for(count=0;count<length;count++){

		 if(count!=length-1)data[count]=I2C_Soft_Read_Byte(I2Cx,1);
		 	else  data[count]=I2C_Soft_Read_Byte(I2Cx,0);
	}
    I2C_Soft_Stop(I2Cx);
    return count;
}

uint8_t I2C_Soft_WriteBytes(uint8_t I2Cx,u8 dev, u8 reg, u8 length, u8* data)
{
  
 	u8 count = 0;
	I2C_Soft_Start(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,dev);
	I2C_Soft_Wait_Ack(I2Cx);
	I2C_Soft_Send_Byte(I2Cx,reg);
    I2C_Soft_Wait_Ack(I2Cx);	  
	for(count=0;count<length;count++){
		I2C_Soft_Send_Byte(I2Cx,data[count]); 
		I2C_Soft_Wait_Ack(I2Cx); 
	 }
	I2C_Soft_Stop(I2Cx);

    return 1;
}


uint8_t I2C_Soft_ReadByte(uint8_t I2Cx,u8 dev, u8 reg, u8 *data){
	*data=I2C_Soft_ReadOneByte(I2Cx,dev, reg);
    return 1;
}


unsigned char I2C_Soft_WriteByte(uint8_t I2Cx,unsigned char dev, unsigned char reg, unsigned char data){
    return I2C_Soft_WriteBytes(I2Cx,dev, reg, 1, &data);
}

uint8_t I2C_Soft_WriteBits(uint8_t I2Cx,u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b;
    if (I2C_Soft_ReadByte(I2Cx,dev, reg, &b) != 0) {
        u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return I2C_Soft_WriteByte(I2Cx,dev, reg, b);
    } else {
        return 0;
    }
}


uint8_t I2C_Soft_WriteBit(uint8_t I2Cx,u8 dev, u8 reg, u8 bitNum, u8 data)
{
    u8 b;
    I2C_Soft_ReadByte(I2Cx,dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return I2C_Soft_WriteByte(I2Cx,dev, reg, b);
}


