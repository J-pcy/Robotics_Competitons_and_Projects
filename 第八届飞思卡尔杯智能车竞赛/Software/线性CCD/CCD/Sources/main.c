#include "includes.h"

unsigned char TimerFlag20ms = 0;
unsigned char send_data_cnt =0;
unsigned char TimerCnt20ms = 0;
unsigned char integration_piont;

unsigned char Pixel[128];
unsigned char Pixel1[128];//二值化后的数组
unsigned char Limit=100;


void Binarization(void)//二值化的程序
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




void main(void) 
{


//    unsigned char i;
//    unsigned char send_data_cnt = 0;
//    unsigned char *pixel_pt;
    
    PLL_init_80M();

    SCI0_init();

    PIT_Init();

    AD_Init();

    CCD_IO_Init();

//    pixel_pt = Pixel;
//    for(i=0; i<128+10; i++) 
//    {
//        *pixel_pt++ = 0;
//    }


    EnableInterrupts;

    for(;;) 
    {
      unsigned char k;
      /*for(k=0;k<128;k++)
      {
        Uart_putchar(Pixel[k]/100+48);
        Uart_putchar(Pixel[k]%100/10+48);
        Uart_putchar(Pixel[k]%100%10+48);
      }
      Uart_putchar('\r');
      Uart_putchar('\n');
    
    //Uart_putchar('\r');
    //Uart_putchar('\n');
    /*for(k=0;k<128;k++)
    {
     Uart_putchar(Pixel1[k]+48);
    }
    Uart_putchar('\r');
    Uart_putchar('\n');            
        /* 20ms Task */
       
     if(TimerFlag20ms == 1) 
        {
            TimerFlag20ms = 0;
            ImageCapture(Pixel);
            Binarization();//二值化的程序
            CalculateIntegrationTime();
            
/*            for(k=0;k<128;k++)
            {
              Uart_putchar(Pixel1[k]+48);
            }
            Uart_putchar('\r');
            Uart_putchar('\n');     */
            
            for(k=0;k<128;k++)
            {
              Uart_putchar(Pixel[k]/100+48);
              Uart_putchar(Pixel[k]%100/10+48);
              Uart_putchar(Pixel[k]%100%10+48);
            }
            Uart_putchar('\r');
            Uart_putchar('\n');            
        /* 20ms Task */
            
             //   if(++send_data_cnt >= 5) 
             //    {
             //          send_data_cnt = 0;
             //          SendImageData(Pixel);
             //    }
        } 

    } /* End of for(;;); loop */

} /* End of main() */





#pragma CODE_SEG __NEAR_SEG NON_BANKED

__interrupt VectorNumber_Vpit0 void Timer0(void) 
{

    static unsigned char TimerCnt20ms = 0;
    unsigned char integration_piont;

    PITTF_PTF0 = 1;

    TimerCnt20ms++;

    /* 根据曝光时间计算20ms周期内的曝光点 */
    integration_piont = 20 - IntegrationTime;
    if(integration_piont >= 2) {      /* 曝光点小于2(曝光时间大于18ms)则不进行再曝光 */
        if(integration_piont == TimerCnt20ms)
        StartIntegration();           /* 曝光开始 */
    }

     

    if(TimerCnt20ms >= 20)
     {
        TimerCnt20ms = 0;
        TimerFlag20ms = 1;
     }

}

#pragma CODE_SEG DEFAULT