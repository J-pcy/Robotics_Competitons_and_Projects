/********************************************
Target : MC9S12XS128
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz    
 
ʹ��˵����
OLED��Դʹ��3.3V��   
----------------
G    ��Դ��
3.3V ��3.3V��Դ
D0   PORTA_PA0  
D1	 PORTA_PA1
RST  PORTA_PA2 
DC   PORTA_PA3
CS   �ѽӵأ����ý�
============================================
OLED��Դʹ��5V��   
----------------
*/
#include "derivative.h"   
#include "LQ12864.h"
uint M_PACNT=0; 
uchar PIT_flag=0;   
void PLL_init_80M(void)
{
//Fpllclk = 2 * Foscclk * (SYNR+1)/(REFDV+1)
  CLKSEL=0X00;
  PLLCTL_PLLON=1;
  SYNR=0xc0|0x04; //1100   000100  //����һЩ����
  REFDV=0x80|0x01; //10 000001
  _asm(nop);
  _asm(nop);
  while(CRGFLG_LOCK!=1);
  CLKSEL=0x80; 
}

void  PAC_Init(void)
{
  PACTL=0x40; //������� �¼�����ģʽ �½�����Ч 
  PACNT=0x00; //�����ĳ�ֵΪ0
}
  
  
void PIT_Init()
{
  
  PITCFLMT=0x00; //��ֹPITģ��
  PITCE_PCE0=1;  // ����PCE0�Ķ�ʱ����
  PITMUX=0x00;//ʹ��΢��ʱ0
//time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS
//1ms
  PITMTLD0 = 199;             //PIT Micro Timer Load Register
  PITLD0 = 1999;  
  PITINTE=0x01;//�����ж�����
  PITCFLMT=0x80;//����PITģ��
}
  
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<6600;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
} 
//============================MAIN()===========================
/*********************������************************************/
void main(void) 
{
 
  PLL_init_80M(); 
  DDRA=0XFF; 
  LCD_Init();
  PAC_Init();
  PIT_Init();  
  EnableInterrupts;
  Draw_BMP(16,0,112,7,longqiu96x64);  //ͼƬ��ʾ
  Dly_ms(1000);
  LCD_CLS();
  LCD_Print(20,0,"�Ͼ�����ѧԺ");  //�����ַ�����ʾ
  LCD_Print(20,4,"��˼�������ܳ�");   //�����ַ�����ʾ
  Dly_ms(1000);
  LCD_CLS();
  for(;;) 
  {
      //LCD_Fill(0xff);//����  
      //LCD_Fill(0x00);//����
      LCD_Print(0,0,"������:");   //�����ַ�����ʾ
      //DisplayNum(50,0,-123);
      LCD_Print(0,2,"���ٶȼ�:");   //�����ַ�����ʾ
      DisplayNum(60,2,2345);
      LCD_Print(0,4,"�Ƕ�:");   //�����ַ�����ʾ
      DisplayNum(60,4,678);
      if(PIT_flag==1)
      {
        PIT_flag=0;
        DisplayNum(50,0,M_PACNT*10);
      }
      //LCD_P8x16Str(48,4,"free scale");          //Ӣ���ַ�����ʾ
      //LCD_P6x8Str(16,6,"free scale");//Ӣ���ַ�����ʾ 
     //        
      }
  /* please make sure that you never leave main */
}


#pragma CODE_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void  timer_inte0(void) 
{
  PITTF=0x01;//�����ʱ����־λ
  M_PACNT=PACNT;
  PACNT=0;
  PIT_flag=1;
 
}
#pragma CODE_SEG DEFAULT
