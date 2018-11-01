/* Including needed modules to compile this module/procedure */
#include "includes.h"



/* 128个像素点的平均AD值 */
unsigned char PixelAverageValue;
/* 128个像素点的平均电压值的10倍 */
unsigned char PixelAverageVoltage;

/* 设定目标平均电压值，实际电压的10倍 */
int TargetPixelAverageVoltage = 30;
/* 设定目标平均电压值与实际值的偏差，实际电压的10倍 */
int PixelAverageVoltageError = 0;
/* 设定目标平均电压值允许的偏差，实际电压的10倍 */
int TargetPixelAverageVoltageAllowError = 2;

/* 曝光时间，单位ms */
unsigned char IntegrationTime = 10;



void CalculateIntegrationTime(void) 
{

    /* 计算128个像素点的平均AD值 */
    PixelAverageValue = PixelAverage(128,Pixel);
    /* 计算128个像素点的平均电压值,实际值的10倍 */
    PixelAverageVoltage = (unsigned char)((int)PixelAverageValue * 25 / 128);

    PixelAverageVoltageError = TargetPixelAverageVoltage - PixelAverageVoltage;
    if(PixelAverageVoltageError < -TargetPixelAverageVoltageAllowError)
        IntegrationTime--;
    if(PixelAverageVoltageError > TargetPixelAverageVoltageAllowError)
        IntegrationTime++;
    if(IntegrationTime <= 1)
        IntegrationTime = 1;
    if(IntegrationTime >= 20)
        IntegrationTime = 20;
}


unsigned char PixelAverage(unsigned char len, unsigned char *data)
 {
  unsigned char i;
  unsigned int sum = 0;
  for(i = 0; i<len; i++) {
    sum = sum + *data++;
  }
  return ((unsigned char)(sum/len));
}


void SendImageData(unsigned char * ImageData) 
{

    unsigned char i;
    unsigned char crc = 0;

    /* Send Data */
    Uart_putchar('*');
    Uart_putchar('L');
    Uart_putchar('D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);

    for(i=0; i<128; i++) {
      SendHex(*ImageData++);
    }

    SendHex(crc);
    Uart_putchar('#');
}


void StartIntegration(void) 
{

    unsigned char i;

    TSL1401_SI = 1;         /* SI  = 1 */
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    TSL1401_SI = 0;         /* SI  = 0 */
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */

    for(i=0; i<127; i++) 
    {
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 1;    /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 0;    /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */
}

void ImageCapture(unsigned char * ImageData) {

    unsigned char i;
    unsigned int  temp_int;

    TSL1401_SI = 1;         /* SI  = 1 */
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    TSL1401_SI = 0;         /* SI  = 0 */
    SamplingDelay();

    //Delay 20us for sample the first pixel
    for(i = 0; i < 20; i++) 
    {
        Cpu_Delay1us();
    }

    //Sampling Pixel 1
    temp_int = AD_Measure12(0);
    *ImageData++ = (byte)(temp_int>>4);
    TSL1401_CLK = 0;        /* CLK = 0 */

    for(i=0; i<127; i++) 
    {
        SamplingDelay();
        SamplingDelay();
        TSL1401_CLK = 1;    /* CLK = 1 */
        SamplingDelay();
        SamplingDelay();
        //Sampling Pixel 2~128
        temp_int = AD_Measure12(0);
        *ImageData++ = (byte)(temp_int>>4);
        TSL1401_CLK = 0;    /* CLK = 0 */
    }
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 1;        /* CLK = 1 */
    SamplingDelay();
    SamplingDelay();
    TSL1401_CLK = 0;        /* CLK = 0 */
}


void CCD_IO_Init(void)
 {
    TSL1401_CLK_DDR = 1;
    TSL1401_SI_DDR  = 1;
    TSL1401_CLK = 0;
    TSL1401_SI  = 0;
}


void SendHex(unsigned char hex)
 {
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    Uart_putchar(temp + '0');
  } else {
    Uart_putchar(temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    Uart_putchar(temp + '0');
  } else {
    Uart_putchar(temp - 10 + 'A');
  }
}
