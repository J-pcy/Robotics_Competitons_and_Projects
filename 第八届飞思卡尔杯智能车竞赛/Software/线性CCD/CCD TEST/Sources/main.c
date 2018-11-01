#include "includes.h"

//unsigned char TimerFlag20ms = 0;
//unsigned char send_data_cnt =0;
//unsigned char TimerCnt20ms = 0;
//unsigned char integration_piont;

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


    unsigned char i;
    unsigned char send_data_cnt = 0;
    unsigned char *pixel_pt;
    
    PLL_init_80M();

    SCI0_init();

    PIT_Init();

    AD_Init();

    CCD_IO_Init();

    pixel_pt = Pixel;
    for(i=0; i<128+10; i++) 
    {
        *pixel_pt++ = 0;
    }


    EnableInterrupts;

    for(;;) 
    {
    unsigned char k;
    //for(k=0;k<128;k++)
   // {
    // Uart_putchar(Pixel[k]/100+48);
     //Uart_putchar(Pixel[k]%100/10+48);
    // Uart_putchar(Pixel[k]%100%10+48);
    //}
   // Uart_putchar('\r');
   // Uart_putchar('\n');
    
    Uart_putchar('\r');
    Uart_putchar('\n');
    for(k=0;k<128;k++)
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
            
             //   if(++send_data_cnt >= 5) 
             //    {
            //          send_data_cnt = 0;
            //          SendImageData(Pixel);
             //    }
        } 

    } /* End of for(;;); loop */

} /* End of main() */

