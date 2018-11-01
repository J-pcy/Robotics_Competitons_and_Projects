/**********************************************************************************
********************* (C) COPYRIGHT 2012 蓝宙电子科技有限公司 *********************
***********************************************************************************
 * 描述         ：线性CCD测试程序
 *
 * 实验平台     ：蓝宙电子XS128系统板 + TSL1401线性CCD传感器
 *
 * 作者         ：蓝宙电子科技有限公司
 * 淘宝店       ：http://landzo.taobao.com
 * 网站         ：http://www.landzo.cn
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

    for(;;) {

        /* 20ms Task */
        if(TimerFlag20ms == 1) {
            TimerFlag20ms = 0;

            /* Flash LED */
            LED1 = ~LED1;

            /* Sampling CCD data */
            ImageCapture(Pixel);

            /* Calculate Integration Time */
            CalculateIntegrationTime();

            /* Send data to CCDView every 100ms */
            if(++send_data_cnt >= 5) {
                send_data_cnt = 0;
                SendImageData(Pixel);
            }

        }

    } /* End of for(;;); loop */

} /* End of main() */