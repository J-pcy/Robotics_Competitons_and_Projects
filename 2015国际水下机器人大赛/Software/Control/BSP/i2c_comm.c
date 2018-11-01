#include "i2c_comm.h"
#include "stm32f10x_rcc.h"
#include "bsp.h"


/******* specified by user ************/
//#define I2C_REMAP
//#define SLAVE_10BIT_ADDRESS
/* timeout for check target's internal write procedure --> set 1s as max */
#define CHECK_TIMEOUT   (1000000*one_us_unit) 

#define BUS_BUSY_TIMEOUT (i2c_10clk_us*one_us_unit)*100
#define SEND_START_TIMEOUT  (i2c_10clk_us*one_us_unit)*100
#define SEND_ADDR7_TIMEOUT  (i2c_10clk_us*one_us_unit)*100
#define SEND_ADDR_HEADER_TIMEOUT  (i2c_10clk_us*one_us_unit)*100
#define SEND_DATA_TIMEOUT (i2c_10clk_us*one_us_unit)*100
#define RECEIVE_DATA_TIMEOUT (i2c_10clk_us*one_us_unit)*100

#define WAIT_COMM_TIMEOUT 
#define RECEIVE_STOP_TIMEOUT (i2c_10clk_us*one_us_unit)/5
/******* specify by user ************/

 
static u32 one_us_unit=0;
static u32 i2c_10clk_us=0;
static volatile bool I2C_OT = false;
TIM_TimeBaseInitTypeDef TIMStruct;
void systick_isr()
{
  I2C_OT = true;
  //SysTick_CounterCmd(SysTick_Counter_Disable);
  TIM_Cmd(TIM4,DISABLE);
}

void  I2C_Comm_Init(I2C_TypeDef* I2Cx, u32 I2C_Speed, u16 I2C_Addr)
{
    
/******* GPIO configuration and clock enable *********/
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_ClocksTypeDef  rcc_clocks;
  I2C_InitTypeDef  I2C_InitStructure;
  u32 hclk;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
  if(I2Cx == I2C1)
  {
#ifdef I2C_REMAP
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
#else
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  }
  else if (I2Cx == I2C2)
  {
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10| GPIO_Pin_11;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  }
    
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
/*********** I2C periphral configuration **********/
  I2C_DeInit(I2Cx);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; /* fixed */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  /* fixed */
  I2C_InitStructure.I2C_OwnAddress1 = I2C_Addr;  /* user parameter */
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; /* fixed */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; /* user parameter */
  I2C_Cmd(I2Cx, ENABLE);
  I2C_Init(I2Cx, &I2C_InitStructure);
  
/********** SysTick for timeout configuration *****/
  RCC_GetClocksFreq(&rcc_clocks);
  hclk = rcc_clocks.HCLK_Frequency;
  
  
  
  //NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 0, 0);
  //SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  //one_us_unit = hclk/1000000;
  TIMStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIMStruct.TIM_Period = 71;
  TIMStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInit(TIM4,&TIMStruct);
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
  
  i2c_10clk_us = 1000000*10/I2C_Speed;
  
  TIM_SetCounter(TIM4,0);
  
  //TIM_Cmd(TIM
  //SysTick_SetReload(one_us_unit); /* initial timeout--> 1us! once systick enabled */
  //SysTick_ITConfig(ENABLE);
  //SysTick_CounterCmd(SysTick_Counter_Clear);
}


/*******************************************************************************
* Function Name  : I2C_Comm_MasterWrite
* Description    : write a block of data to slave devices.
* Input          : - I2Cx : the I2C port from which mcu send data out.
*                  - slave_addr : target slave
*                  - offset : the internal memory of target slave to place data
*                     if special value : no internal memory space of slave
*                  - pBuffer : pointer to the data ready to send
*                  - length : number of bytes to send.
* Output         : None
* Return         : None
*******************************************************************************/
I2C_Result  I2C_Comm_MasterWrite(I2C_TypeDef* I2Cx, u16 slave_addr, u32 offset, u8* pBuffer, u32 length)
{
vu32 check_time = 0;

    /*wait 5us min for bus free time limitation for later transaction*/
    *(u32 *)0xe000e014 = one_us_unit*5;     //SysTick_SetReload(one_us_unit*5);
    *(u32 *)0xe000e018 = 0;                 //SysTick_CounterCmd(SysTick_Counter_Clear);
    *(u32 *)0xe000e010 |= 1;                //SysTick_CounterCmd(SysTick_Counter_Enable);
    while(!I2C_OT);
    *(u32 *)0xe000e010 &= 0xfffffffe;       //SysTick_CounterCmd(SysTick_Counter_Disable);
    I2C_OT = false;    
    
    
    /*wait bus free*/
    *(u32 *)0xe000e014 = BUS_BUSY_TIMEOUT;  //SysTick_SetReload(BUS_BUSY_TIMEOUT);
    *(u32 *)0xe000e018 = 0;                 //SysTick_CounterCmd(SysTick_Counter_Clear);
    *(u32 *)0xe000e010 |= 1;                //SysTick_CounterCmd(SysTick_Counter_Enable);
    while((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))&&(!I2C_OT));
    *(u32 *)0xe000e010 &= 0xfffffffe;       //SysTick_CounterCmd(SysTick_Counter_Disable);
    if (I2C_OT)
    {
      I2C_OT = false;
      return BUS_BUSY;
    } 
    
    
    /*send start and wait*/
    I2C_GenerateSTART(I2Cx, ENABLE);
    *(u32 *)0xe000e014 = SEND_START_TIMEOUT;  //SysTick_SetReload(SEND_START_TIMEOUT);
    *(u32 *)0xe000e018 = 0;                   //SysTick_CounterCmd(SysTick_Counter_Clear);
    *(u32 *)0xe000e010 |= 1;                  //SysTick_CounterCmd(SysTick_Counter_Enable);
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) || I2C_OT)); 
    *(u32 *)0xe000e010 &= 0xfffffffe;         //SysTick_CounterCmd(SysTick_Counter_Disable);
    if (I2C_OT)
    {
      I2C_OT = false;
      return SEND_START_ERR;
    }  
    
    
    /* send 7-bit slave address and wait */
    I2C_Send7bitAddress(I2Cx, (u8)(slave_addr & 0xFF), I2C_Direction_Transmitter);
    *(u32 *)0xe000e014 = SEND_ADDR7_TIMEOUT;  //SysTick_SetReload(SEND_ADDR7_TIMEOUT);
    *(u32 *)0xe000e018 = 0;                   //SysTick_CounterCmd(SysTick_Counter_Clear);
    *(u32 *)0xe000e010 |= 1;                  //SysTick_CounterCmd(SysTick_Counter_Enable);
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) || I2C_OT));  
    *(u32 *)0xe000e010 &= 0xfffffffe;         //SysTick_CounterCmd(SysTick_Counter_Disable);
    if (I2C_OT)
    {
      I2C_OT = false;
      I2C_GenerateSTOP(I2Cx, ENABLE);
      while ((I2C1->CR1 & 0x200) == 0x200);   //wait while stop bit not cleared 
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF))
        I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
      return ADDR_MATCH_ERR;
    }   
        
    /* send offset if needed */
    if (offset != 0xffffffff)
    {
      I2C_SendData(I2Cx, offset); 
      *(u32 *)0xe000e014 = SEND_DATA_TIMEOUT; //SysTick_SetReload(SEND_DATA_TIMEOUT);
      *(u32 *)0xe000e018 = 0;                 //SysTick_CounterCmd(SysTick_Counter_Clear);
      *(u32 *)0xe000e010 |= 1;                //SysTick_CounterCmd(SysTick_Counter_Enable);
      while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) || I2C_OT));
      *(u32 *)0xe000e010 &= 0xfffffffe;       //SysTick_CounterCmd(SysTick_Counter_Disable);
      if (I2C_OT)
      {
        I2C_OT = false;
        I2C_GenerateSTOP(I2Cx, ENABLE);
        while ((I2C1->CR1 & 0x200) == 0x200);   //wait while stop bit not cleared 
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) 
          I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
        return DATA_TIMEOUT;
      } 
    }
  

//    while(length--)  
//    {
//      /* send data and wait */
//      I2C_SendData(I2Cx, *pBuffer++); 
//      *(u32 *)0xe000e014 = SEND_DATA_TIMEOUT; //SysTick_SetReload(SEND_DATA_TIMEOUT);
//      *(u32 *)0xe000e018 = 0;                 //SysTick_CounterCmd(SysTick_Counter_Clear);
//      *(u32 *)0xe000e010 |= 1;                //SysTick_CounterCmd(SysTick_Counter_Enable);
//      while (!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) || I2C_OT));
//      *(u32 *)0xe000e010 &= 0xfffffffe;       //SysTick_CounterCmd(SysTick_Counter_Disable);
//      if (I2C_OT)
//      {
//        I2C_OT = FALSE;
//        I2C_GenerateSTOP(I2Cx, ENABLE);
//        while ((I2C1->CR1 & 0x200) == 0x200); 
//        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) 
//          I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
//        return DATA_TIMEOUT;
//      }
//    }
    /* comment above send code */
    /* use another way to send by polling on BTF */
    I2C_SendData(I2Cx, *pBuffer++);
    length--;
    while (length--)
    {
        *(u32 *)0xe000e014 = SEND_DATA_TIMEOUT; 
        *(u32 *)0xe000e018 = 0;                 
        *(u32 *)0xe000e010 |= 1;               
        while (((I2C_GetLastEvent(I2Cx) & 0x04) != 0x04)&&(!I2C_OT));  
        *(u32 *)0xe000e010 &= 0xfffffffe;  
        if (I2C_OT)
        {
          I2C_OT = false;
          I2C_GenerateSTOP(I2Cx, ENABLE);
          while ((I2C1->CR1 & 0x200) == 0x200); 
          if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) 
            I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
          return DATA_TIMEOUT;
        }
        I2C_SendData(I2Cx, *pBuffer++);
    }
    *(u32 *)0xe000e014 = SEND_DATA_TIMEOUT; 
    *(u32 *)0xe000e018 = 0;                 
    *(u32 *)0xe000e010 |= 1; 
    while (!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) || I2C_OT));
    *(u32 *)0xe000e010 &= 0xfffffffe; 
    if (I2C_OT)
    {
      I2C_OT = false;
      I2C_GenerateSTOP(I2Cx, ENABLE);
      while ((I2C1->CR1 & 0x200) == 0x200); 
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)) 
        I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
      return DATA_TIMEOUT;
    }


    /* send stop to close communication */
    I2C_GenerateSTOP(I2Cx, ENABLE);
  
   
   
    /* launch check write status procedure */
    while(check_time != 1000000/5)
    {
      /*wait 5us min for bus free time limitation for later transaction : check phase*/
      *(u32 *)0xe000e014 = one_us_unit*5;     //SysTick_SetReload(one_us_unit*5);
      *(u32 *)0xe000e018 = 0;                 //SysTick_CounterCmd(SysTick_Counter_Clear);
      *(u32 *)0xe000e010 |= 1;                //SysTick_CounterCmd(SysTick_Counter_Enable);
      while(!I2C_OT);
      *(u32 *)0xe000e010 &= 0xfffffffe;       //SysTick_CounterCmd(SysTick_Counter_Disable);
      I2C_OT = false;  
    
      /* wait bus for free */
      *(u32 *)0xe000e014 = CHECK_TIMEOUT;
      *(u32 *)0xe000e018 = 0;
      *(u32 *)0xe000e010 |= 1;
      while((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))&&(!I2C_OT));
      *(u32 *)0xe000e010 &= 0xfffffffe; 
      I2C_OT = false;
      if (I2C_OT)
      {
        I2C_OT = false;
        /* during 1s, bus is occupied by other i2c comm */  
        /* after check_timeout(1s), target's internal must complete */
        return NO_ERR;
      }      
      

      I2C_GenerateSTART(I2Cx, ENABLE);
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
      I2C_Send7bitAddress(I2Cx, slave_addr, I2C_Direction_Transmitter);
      
       /* wait for 10 I2C clock pulse period and then check ADDR set or not*/
      *(u32 *)0xe000e014 = one_us_unit*i2c_10clk_us;
      *(u32 *)0xe000e018 = 0;
      *(u32 *)0xe000e010 |= 1;
      while(!I2C_OT);
      *(u32 *)0xe000e010 &= 0xfffffffe; 
      I2C_OT = false;

      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR))
      {
        break;
      }
      I2C_ClearFlag(I2Cx, I2C_FLAG_AF);   
      I2C_GenerateSTOP(I2Cx, ENABLE); 
      while ((I2C1->CR1 & 0x200) == 0x200);    
    
      check_time++;
    }

    if (check_time ==1000000/5)
      return WRITE_FAILURE;

    I2C_ClearFlag(I2Cx, I2C_FLAG_AF);   
    I2C_GenerateSTOP(I2Cx, ENABLE); 
    while ((I2C1->CR1 & 0x200) == 0x200);    
    
    return NO_ERR;
}


/*******************************************************************************
* Function Name  : I2C_Comm_MasterRead
* Description    : read a block of data from slave devices.
* Input          : - I2Cx : the I2C port from which mcu send data out.
*                  - slave_addr : target slave
*                  - offset : the internal memory of target slave to place data
*                     if special value : no internal memory space of slave
*                  - pBuffer : pointer to the data ready to send
*                  - length : number of bytes to send, MUST >= 3 !!!
* Output         : None
* Return         : None
*******************************************************************************/
I2C_Result  I2C_Comm_MasterRead(I2C_TypeDef* I2Cx, u16 slave_addr, u32 offset, u8* pBuffer, u32 length)
{   
  /* wait 5us max for bus free time limitation for later transaction */
  *(u32 *)0xe000e014 = one_us_unit*5;
  *(u32 *)0xe000e018 = 0;
  *(u32 *)0xe000e010 |= 1;
  while(!I2C_OT);
  *(u32 *)0xe000e010 &= 0xfffffffe; 
  I2C_OT = false;  

  /* wait bus free */
  *(u32 *)0xe000e014 = BUS_BUSY_TIMEOUT;
  *(u32 *)0xe000e018 = 0;
  *(u32 *)0xe000e010 |= 1;
  while((I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))&&(!I2C_OT));
  *(u32 *)0xe000e010 &= 0xfffffffe; 
  if (I2C_OT)
  {
    I2C_OT = false;
    return BUS_BUSY;
  }
  
  /* send start and wait */
  I2C_GenerateSTART(I2Cx, ENABLE);
  *(u32 *)0xe000e014 = SEND_START_TIMEOUT;
  *(u32 *)0xe000e018 = 0;
  *(u32 *)0xe000e010 |= 1;
  while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) || I2C_OT)); 
  *(u32 *)0xe000e010 &= 0xfffffffe; 
  if (I2C_OT)
  {
    I2C_OT = false;
    return SEND_START_ERR;
  }
        
  /*
  if offset needed:
  (1) slave address with write direction
  (2) offset
  (3) re-start
  */
  if (offset!=0xffffffff)
  {
    /* (1) */
    I2C_Send7bitAddress(I2Cx, (u8)(slave_addr & 0xFF), I2C_Direction_Transmitter);
    *(u32 *)0xe000e014 = SEND_ADDR7_TIMEOUT;
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) || I2C_OT));  
    *(u32 *)0xe000e010 &= 0xfffffffe; 
    if (I2C_OT)
    {
      I2C_OT = false;
      I2C_GenerateSTOP(I2Cx, ENABLE);
      while ((I2C1->CR1 & 0x200) == 0x200);    
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF))
        I2C_ClearFlag(I2Cx, I2C_FLAG_AF); 
      return ADDR_MATCH_ERR;
    }
      
    
    /* (2) */
    I2C_SendData(I2Cx, offset); 
    *(u32 *)0xe000e014 = SEND_DATA_TIMEOUT;
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) || I2C_OT));
    *(u32 *)0xe000e010 &= 0xfffffffe; 
    if (I2C_OT)
    {
      I2C_OT = false;
      I2C_GenerateSTOP(I2Cx, ENABLE);
      while ((I2C1->CR1 & 0x200) == 0x200);    
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF))
        I2C_ClearFlag(I2Cx, I2C_FLAG_AF); 
      return DATA_TIMEOUT;
    }
      
    
    /* (3) */
    I2C_GenerateSTART(I2Cx, ENABLE);
    *(u32 *)0xe000e014 = SEND_START_TIMEOUT;
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) || I2C_OT)); 
    *(u32 *)0xe000e010 &= 0xfffffffe; 
    if (I2C_OT)
    {
      I2C_OT = false;
      return SEND_START_ERR;
    }   
  } 

  
  /* send addr with read direction */
  I2C_Send7bitAddress(I2Cx, (u8)(slave_addr & 0xFF), I2C_Direction_Receiver);
  
  
  /*
  different implementation of receive data 
  depends on communication length
  (1) length >=3
  (2) length = 2
  (3) length = 1
  */
  if (length >=3)
  {
    *(u32 *)0xe000e014 = SEND_ADDR7_TIMEOUT;  
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(!(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) || I2C_OT));  
    *(u32 *)0xe000e010 &= 0xfffffffe;    
    if (I2C_OT)
    {
      I2C_OT = false;
      I2C_GenerateSTOP(I2Cx, ENABLE);
      while ((I2C1->CR1 & 0x200) == 0x200);    
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF))
          I2C_ClearFlag(I2Cx, I2C_FLAG_AF); 
      return ADDR_MATCH_ERR;
    }    
    
    /* receive data */
    /* use another safe procedure to receive data */
    while(length)
    {
      if (length !=3)
      {
        *(u32 *)0xe000e014 = RECEIVE_DATA_TIMEOUT;   
        *(u32 *)0xe000e018 = 0;
        *(u32 *)0xe000e010 |= 1;
        while(((I2C_GetLastEvent(I2Cx)&0x04) != 0x04)&& !I2C_OT); // poll on BTF
        *(u32 *)0xe000e010 &= 0xfffffffe;     
        if (I2C_OT)
        {
          I2C_OT = false;
          I2C_GenerateSTOP(I2Cx, ENABLE);
          return DATA_TIMEOUT;
        }    
        *pBuffer++ = I2C_ReceiveData(I2Cx);
        length--;
      }
      
      if (length == 3)
      {
        /* D(n-2) in DR; D(n-1) in shift register */
        *(u32 *)0xe000e014 = RECEIVE_DATA_TIMEOUT;   
        *(u32 *)0xe000e018 = 0;
        *(u32 *)0xe000e010 |= 1;
        while(((I2C_GetLastEvent(I2Cx)&0x04) != 0x04)&& !I2C_OT); // poll on BTF
        *(u32 *)0xe000e010 &= 0xfffffffe;     
        if (I2C_OT)
        {
          I2C_OT = false;
          I2C_GenerateSTOP(I2Cx, ENABLE);
          return DATA_TIMEOUT;
        }        
        /* clear ACK */
        I2C_AcknowledgeConfig(I2Cx, DISABLE);     
        __disable_irq();
        /* read D(n-2) */
        *pBuffer++ = I2C_ReceiveData(I2Cx);      
        /* program stop */
        I2C_GenerateSTOP(I2Cx, ENABLE);    
        /* read D(n-1) */
        *pBuffer++ = I2C_ReceiveData(I2Cx);
        __enable_irq();
        
        *(u32 *)0xe000e014 = RECEIVE_DATA_TIMEOUT;   
        *(u32 *)0xe000e018 = 0;
        *(u32 *)0xe000e010 |= 1;
        while(((I2C_GetLastEvent(I2Cx)&0x40) != 0x40)&& !I2C_OT); // poll on RxNE
        *(u32 *)0xe000e010 &= 0xfffffffe;     
        if (I2C_OT)
        {
          I2C_OT = false;
          I2C_GenerateSTOP(I2Cx, ENABLE);
          return DATA_TIMEOUT;
        }
        *pBuffer = I2Cx->DR;
        length = 0;
      }
    }
    
    while ((I2C1->CR1 & 0x200) == 0x200); 
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
  }
  else if (length == 2)
  {
    vu32 tmp;
    /* ACK=1,POS=1 */
    I2Cx->CR1 = 0xc01; 
    
    *(u32 *)0xe000e014 = SEND_ADDR7_TIMEOUT;   
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while((!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR))&& !I2C_OT); //wait for ADDR event
    *(u32 *)0xe000e010 &= 0xfffffffe;     
    if (I2C_OT)
      {
        I2C_OT = false;
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return ADDR_MATCH_ERR;
      } 
    __disable_irq();
    /* clear ADDR */
    tmp = I2Cx->SR2;    
    /* disable ACK */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);

    __enable_irq();     
    
    *(u32 *)0xe000e014 = RECEIVE_DATA_TIMEOUT;   
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(((I2C_GetLastEvent(I2Cx)&0x0004) != 0x0004)&& !I2C_OT); // poll on BTF
    *(u32 *)0xe000e010 &= 0xfffffffe;     
    if (I2C_OT)
      {
        I2C_OT = false;
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return DATA_TIMEOUT;
      } 
    __disable_irq();   
    I2C_GenerateSTOP(I2Cx, ENABLE); 
    *pBuffer++ = I2Cx->DR;
    *pBuffer = I2Cx->DR;
    __enable_irq();
    length = 0;
    /* POS=0; ACK=PE=1 */
    I2Cx->CR1 = 0x0401; 
  }
  else if (length == 1)
  {
    vu32 tmp;
    
    *(u32 *)0xe000e014 = SEND_ADDR7_TIMEOUT;   
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while((!I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR))&& !I2C_OT); //wait for ADDR event
    *(u32 *)0xe000e010 &= 0xfffffffe;     
    if (I2C_OT)
      {
        I2C_OT = false;
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return ADDR_MATCH_ERR;
      }   
    /* disable ACK */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);

    __disable_irq();
    /* clear ADDR */
    tmp = I2Cx->SR2;   
    I2C_GenerateSTOP(I2Cx, ENABLE);
    __enable_irq();
    
    
    
    *(u32 *)0xe000e014 = RECEIVE_DATA_TIMEOUT;   
    *(u32 *)0xe000e018 = 0;
    *(u32 *)0xe000e010 |= 1;
    while(((I2C_GetLastEvent(I2Cx)&0x40) != 0x40)&& !I2C_OT); // poll on RxNE
    *(u32 *)0xe000e010 &= 0xfffffffe;     
    if (I2C_OT)
      {
        I2C_OT = false;
        I2C_GenerateSTOP(I2Cx, ENABLE);
        return DATA_TIMEOUT;
      } 
    
    *pBuffer = I2Cx->DR;
    length = 0;
    while ((I2C1->CR1 & 0x200) == 0x200); 
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
  }
      
  return NO_ERR;
}
