/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "math.h"

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 img[CAMERA_W*CAMERA_H];                           //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��

uint8 image[CAMERA_W][CAMERA_H];

//��������
void sendimg(uint8 *imgaddr, uint32 imgsize);          //����ͼ����λ��
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
    NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�
    NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�
    NVIC_SetPriority(PIT0_IRQn,1);          //�������ȼ�
    NVIC_SetPriority(DMA0_IRQn,2);          //�������ȼ�
    
//    gpio_init(PTD15,GPO,HIGH);
    //��ʼ������ͷ
    camera_init(imgbuff);

    //�����жϸ�λ����
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler

    while(1)
    {
//        gpio_set(PTD15,LOW);
        //��ȡͼ��
        camera_get_img();                                   //����ͷ��ȡͼ��       
//        gpio_set(PTD15,HIGH);

//        gpio_set(PTD15,LOW);
        //��ѹͼ��
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
        
        //����ͼ����λ��
//        sendimg(image, CAMERA_W * CAMERA_H);                  //���͵���λ��
        
    }
}

/*!
 *  @brief      ����ͼ��eSmartCameraCar��λ����ʾ
 *  @param      imgaddr         ͼ���ַ
 *  @param      imgsize         ͼ��ռ�ÿռ��С
 *  @since      v5.0
 *  @note       ��ͬ����λ������ͬ���������ʹ�� eSmartCameraCar�����
                ���ʹ��������λ��������Ҫ�޸Ĵ��롣
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //���͵���λ��
 */
/*
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmd[4] = {0, 255, 1, 0 };    //yy_����ͷ���ڵ��� ʹ�õ�����

    uart_putbuff(FIRE_PORT, cmd, sizeof(cmd));    //�ȷ�������

    uart_putbuff(FIRE_PORT, imgaddr, imgsize); //�ٷ���ͼ��
}
*/

void sendimg(uint8 *imgaddr, uint32 imgsize)
{
//  uart_putbuff(FIRE_PORT, imgaddr, imgsize); //�ٷ���ͼ�� 
}


/*!
 *  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
 *  @param      dst             ͼ���ѹĿ�ĵ�ַ
 *  @param      src             ͼ���ѹԴ��ַ
 *  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //���͵���λ��
 */
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��Ұ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
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
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if 0             //ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}