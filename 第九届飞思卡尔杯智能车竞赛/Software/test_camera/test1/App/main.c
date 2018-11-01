/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火K60 平台主程序
 * @author     野火科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "math.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

uint8 image[CAMERA_W][CAMERA_H];

//函数声明
void sendimg(uint8 *imgaddr, uint32 imgsize);          //发送图像到上位机
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void PORTA_IRQHandler();
void DMA0_IRQHandler();

void turn_img()
{
  uint8 i=0,j=0;
  uint16 k=0;
  for(i=0;i<60;i++)
  {
    for(j=0;j<80;j++)
    {
      if(img[k]==255) image[j][i]=1;
      else image[j][i]=0;
      k++;
    }
  }
}

void  main(void)
{
    uint8 i=0,j=0;
    uint16 k=0;
    NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
    NVIC_SetPriority(PIT0_IRQn,1);          //配置优先级
    NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
    
//    gpio_init(PTD15,GPO,HIGH);
    //初始化摄像头
    camera_init(imgbuff);

    //配置中断复位函数
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断复位函数为 PORTA_IRQHandler

    while(1)
    {
//        gpio_set(PTD15,LOW);
        //获取图像
        camera_get_img();                                   //摄像头获取图像       
//        gpio_set(PTD15,HIGH);

//        gpio_set(PTD15,LOW);
        //解压图像
        img_extract(img, imgbuff,CAMERA_SIZE);       
//        gpio_set(PTD15,HIGH);

        turn_img();

        for(i=0;i<60;i++)
        {
          for(j=0;j<80;j++)
          {
            printf("%d",image[j][i]);
          }
          printf("\n");
        } 
  
  
/*        for(i=0;i<60;i++)
        {
          for(j=0;j<80;j++)
          {
//            printf("%d",img[k]);
            uart_putchar(FIRE_PORT,img[k]);
            k++;
          }
          printf("\n");
        }
*/
        
        //发送图像到上位机
//        sendimg(image, CAMERA_W * CAMERA_H);                  //发送到上位机
        
    }
}

/*!
 *  @brief      发送图像到eSmartCameraCar上位机显示
 *  @param      imgaddr         图像地址
 *  @param      imgsize         图像占用空间大小
 *  @since      v5.0
 *  @note       不同的上位机，不同的命令，这里使用 eSmartCameraCar软件，
                如果使用其他上位机，则需要修改代码。
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */
/*
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令

    uart_putbuff(FIRE_PORT, cmd, sizeof(cmd));    //先发送命令

    uart_putbuff(FIRE_PORT, imgaddr, imgsize); //再发送图像
}
*/

void sendimg(uint8 *imgaddr, uint32 imgsize)
{
//  uart_putbuff(FIRE_PORT, imgaddr, imgsize); //再发送图像 
}


/*!
 *  @brief      二值化图像解压（空间 换 时间 解压）
 *  @param      dst             图像解压目的地址
 *  @param      src             图像解压源地址
 *  @param      srclen          二值化图像的占用空间大小
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}