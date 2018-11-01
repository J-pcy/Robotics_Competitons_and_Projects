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


#ifndef USER_H_
#define USER_H_


/* TSL1401_CLK -- PORTA0 */
#define TSL1401_CLK_DDR     DDRA_DDRA0
#define TSL1401_CLK         PORTA_PA0

/* TSL1401_SI -- PORTA1 */
#define TSL1401_SI_DDR      DDRA_DDRA1
#define TSL1401_SI          PORTA_PA1

/* LED1 */
#define LED1_DDR            DDRK_DDRK3
#define LED1                PORTK_PK3

/* LED2 */
#define LED2_DDR            DDRK_DDRK7
#define LED2                PORTK_PK7


/* CPU delay 1us at 40M bus clock */\
#define Cpu_Delay1us() \
    { __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
    }

/* CPU delay 200ns at 40M bus clock */\
#define Cpu_Delay200ns() \
    { __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
      __asm(nop); __asm(nop); __asm(nop); __asm(nop);\
    }

#define SamplingDelay() \
    { Cpu_Delay200ns()\
    }


void CalculateIntegrationTime(void);
unsigned char PixelAverage(unsigned char len, unsigned char *data);
void SendImageData(unsigned char * ImageData);
void StartIntegration(void);
void ImageCapture(unsigned char * ImageData);
void CCD_IO_Init(void);
void LED_Init(void);
void SendHex(unsigned char hex);


extern unsigned char IntegrationTime;


#endif /* #ifndef USER_H_ */
