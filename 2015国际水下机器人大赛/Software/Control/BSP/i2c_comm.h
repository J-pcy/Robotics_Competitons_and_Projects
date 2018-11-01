#ifndef __I2C_COMM_H
#define __I2C_COMM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp.h"
/* Exported types ------------------------------------------------------------*/
typedef enum i2c_result
{
  NO_ERR  = 0,  
  TIMEOUT = 1,
  BUS_BUSY = 2,
  SEND_START_ERR = 3,
  ADDR_MATCH_ERR = 4,
  ADDR_HEADER_MATCH_ERR = 5,
  DATA_TIMEOUT = 6,
  WAIT_COMM = 7,
  STOP_TIMEOUT = 8,
  WRITE_FAILURE = 9,

}I2C_Result;

void  I2C_Comm_Init(I2C_TypeDef* I2Cx, u32 I2C_Speed, u16 I2C_Addr);
I2C_Result  I2C_Comm_MasterWrite(I2C_TypeDef* I2Cx, u16 slave_addr, u32 offset, u8* pBuffer, u32 length);
I2C_Result  I2C_Comm_SlaveRead(I2C_TypeDef* I2Cx, u8* pBuffer, u32 length);
I2C_Result  I2C_Comm_MasterRead(I2C_TypeDef* I2Cx, u16 slave_addr, u32 offset, u8* pBuffer, u32 length);
I2C_Result  I2C_Comm_SlaveWrite(I2C_TypeDef* I2Cx, u8* pBuffer, u32 length);

void systick_isr(void);




void I2C_Master_BufferRead2Byte(u8* pBuffer);
#endif
