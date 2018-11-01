#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Uart0.h"
#include "ADC.h"
#include "angle.h"
#define uint unsigned int
#define uchar unsigned char

float C_Z = 303.604;
float C_Gyro = 347.999;
float Z_Min = 147.816; 
float Z_Max = 474.205;
float T_Z = 3;
//float R_Z = 180/(Z_Max - Z_Min);
float R_Gyro = 4.1;
float Angle_Z,Angle_G,Angle_AG,Angle_GG;

void main(void) 
{
  /* put your own code here */
  AD_Init();
  SCI0_init();
	EnableInterrupts;
  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
    Uart_output(Angle_AG);

  } /* loop forever */
  /* please make sure that you never leave main */
}

