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
* File : includes.h
* By   : Ke Chao
*********************************************************************************************************
*/

#ifndef   INCLUDES_H
#define   INCLUDES_H


/* Aditional standard ANSI C types */
#ifndef int8_t
typedef signed char         int8_t;
#endif
#ifndef int16_t
typedef signed int          int16_t;
#endif
#ifndef int32_t
typedef signed long int     int32_t;
#endif

#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif
#ifndef uint16_t
typedef unsigned int        uint16_t;
#endif
#ifndef uint32_t
typedef unsigned long int   uint32_t;
#endif
#ifndef TPE_Float
typedef float               TPE_Float;
#endif
#ifndef char_t
typedef char                char_t;
#endif


/*
*********************************************************************************************************
* Description: No
*********************************************************************************************************
*/
#include  <hidef.h>
#include  <MC9S12XS128.h>
#pragma   LINK_INFO DERIVATIVE "MC9S12XS128"


/*
*********************************************************************************************************
* Description: No
*********************************************************************************************************
*/
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <ctype.h>


/*
*********************************************************************************************************
* Description: Includes
*********************************************************************************************************
*/
#include  "CPU.h"
#include  "SCI.h"
#include  "ADC.h"
#include  "PIT.h"
#include  "Events.h"
#include  "Vector.h"

#include  "user.h"


/*
*********************************************************************************************************
* Global variable
*********************************************************************************************************
*/
extern unsigned char Pixel[128];


#endif