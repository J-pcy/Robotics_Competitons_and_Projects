#include "user.h"


extern volatile uint32_t AHRS_Time_Counter;//AHRS��ʱ

void SysTick_Handler(void)
{
	OSIntEnter(); 	 //����ͳ���жϵ�Ƕ�ײ�������Ƕ�ײ���+1
	OSTimeTick();	   //ͳ��ʱ�䣬�������񣬶���ʱ�����ʱ��1
	OSIntExit();	   //��Ƕ�ײ�����1�����˳��ж�ǰ�����������
    
    //AHRS_Time_Counter++;//AHRS��ʱ
    
    //CPAL_I2C_TIMEOUT_Manager();
}

//void TIM4_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
//    {
//        
//        //systick_isr();
//        
//        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
//    }
//    
//}


extern uint32_t countpertime;
extern uint32_t counterpers;
extern volatile float sampleFreq;
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
    {
        
        
        counterpers = countpertime;
        
        countpertime = 0;
        
        sampleFreq = counterpers;
        
        if (sampleFreq == 0)
            sampleFreq = 1;
        //systick_isr();
        
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    }
    
}

uint8_t count = 0;
uint8_t pdata[6];
void USART1_IRQHandler(void)
{
    static bool head1 = false;
    static bool head2 = false;
    static bool func = false;
    uint8_t data;
    
    
    uint16_t crc_value;
    
    
    if (USART_GetITStatus(USART1,USART_IT_RXNE))
    {
        
        data = USART_ReceiveData(USART1);
        
        usart1head:
        
        
        if (head1 == false)
        {
            if (data == 0xAA)
            {
                head1 =true;
            }
            goto endofirquart;
        }
        else if (head2 == false)
        {
            if (data == 0x55)
            {
                head2 =true;
                count = 0;
            }
            else
            {
                head1 = false;
                
                if (data == 0xAA)
                    goto usart1head;
            }
            goto endofirquart;
        }
        else// if (func == false)
        {
            
            if (count <5)
            {
                pdata[count] = data;
                count++;
            }
            else
            {
                crc_value = cal_CRC(pdata,2);
                if (pdata[4] == 0x55)
                {
                    if (data == 0xAA)
                    {
                        if (crc_value/256 == pdata[2])
                        {
                            if (crc_value %256 == pdata[3])
                            {
                                if (pdata[0] == 0x01)
                                    switch (pdata[1])
                                    {
                                        case 0x01: NVIC_SystemReset();
                                        case 0x02: IMU_Mode = 1;break;
                                        case 0x03: IMU_Mode = 2;break;
                                        case 0x04: IMU_Mode = 3;break;
                                        case 0x05: break;//У׼����
                                        case 0x06: break;//У׼�Ӽ�
                                        case 0x07: AHRS_HMC5883L_MAG_Calibrate = true;
                                        case 0x08: AHRS_HMC5883L_MAG_CalibrateSaving = true;
                                    }

                            }
                            else
                            {
                                goto usart1else;
                            }
                        }
                        else
                        {
                            goto usart1else;
                        }
                    }
                    else
                    {
                        goto usart1else;
                    }
                            
                }
                else
                {
                    
                    usart1else:
                    head1 = false;
                    head2 = false;
                    //func = false;
                    
                    goto endofirquart;
                }
            }
            
//            if (data == 0x01)//command
//            {
//                func = true; 
//                count = 0;
//            }
//            else
//            {
//                head1 = false;
//                head2 = false;
//                if (data == 0xAA)
//                    goto usart1head;
//            }
//            goto endofirquart;
        }
//        else
//        {
//            
//                
//        }
            
        
        
            endofirquart:
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}


