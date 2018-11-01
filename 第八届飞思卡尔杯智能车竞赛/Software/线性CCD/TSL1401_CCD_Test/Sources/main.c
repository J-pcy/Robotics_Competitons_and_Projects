/**********************************************************************************
********************* (C) COPYRIGHT 2012 ������ӿƼ����޹�˾ *********************
***********************************************************************************
 * ����         ������CCD���Գ���
 *
 * ʵ��ƽ̨     ���������XS128ϵͳ�� + TSL1401����CCD������
 *
 * ����         ��������ӿƼ����޹�˾
 * �Ա���       ��http://landzo.taobao.com
 * ��վ         ��http://www.landzo.cn
**********************************************************************************/
/*
*********************************************************************************************************
*
*                                      MCU: MC9S12XS128MAL - 112Pin
*                                      OSC: 16.000MHz
*                                      BUS: 40.0MHz
*
* File : main.c
* By   : Ke Chao
*********************************************************************************************************
*/

#include "includes.h"


/*
*********************************************************************************************************
* Description: Data
*********************************************************************************************************
*/
unsigned char Pixel[128];
unsigned char Pixel1[128];//��ֵ���������
unsigned char Limit=100;

void Binarization(void)//��ֵ���ĳ���
{
   unsigned int i;
   for(i=0;i<128;i++)
   {
   if(Pixel[i]<=Limit)
   {
   
      Pixel1[i]=1;
   }
   else
   {
      Pixel1[i]=0;     
   }
    }
 }

/*
*********************************************************************************************************
* Description: main
*********************************************************************************************************
*/
void main(void) {

    unsigned char i;
    unsigned char send_data_cnt = 0;
    unsigned char *pixel_pt;

    SCI0_Init();

    PIT_Init();

    AD_Init();

    CCD_IO_Init();

    LED_Init();
    

    /* Init Pixel Array */
    pixel_pt = Pixel;
    for(i=0; i<128+10; i++) {
        *pixel_pt++ = 0;
    }


    EnableInterrupts;

    for(;;) 
    {
    
    unsigned char k;
    /*
    SCI0_SendChar1('\r');
    SCI0_SendChar1('\n');
    for(k=0;k<128;k++)
    {
     SCI0_SendChar1(Pixel[k]/100+48);
     SCI0_SendChar1(Pixel[k]%100/10+48);
     SCI0_SendChar1(Pixel[k]%100%10+48);
     SCI0_SendChar1('\r');
     SCI0_SendChar1('\n');
    }*/
    SCI0_SendChar1('\r');
    SCI0_SendChar1('\n');
    for(k=0;k<128;k++)
    {
     SCI0_SendChar1(Pixel1[k]+48);
    }
     SCI0_SendChar1('\r');
     SCI0_SendChar1('\n');            
        /* 20ms Task */
          if(TimerFlag20ms == 1) 
        {
            TimerFlag20ms = 0;

            /* Flash LED */
            LED1 = ~LED1;

            /* Sampling CCD data */
            ImageCapture(Pixel);
            
            
            Binarization();//��ֵ���ĳ���

            /* Calculate Integration Time */
            CalculateIntegrationTime();

            /* Send data to CCDView every 100ms */
           // if(++send_data_cnt >= 5) 
          // {
         //       send_data_cnt = 0;
         //       SendImageData(Pixel);
         //   }

        }

    } /* End of for(;;); loop */

} /* End of main() */