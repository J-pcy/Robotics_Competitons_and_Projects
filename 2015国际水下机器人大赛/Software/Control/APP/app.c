#include "app.h"

extern	OS_TCB	StartUp_TCB;					
static	OS_TCB	LED1_TCB;
static	CPU_STK	LED1_Stk[TASK_LED1_STK_SIZE];	
static	OS_TCB	LED2_TCB;
static	CPU_STK	LED2_Stk[TASK_LED2_STK_SIZE];	
static	OS_TCB	LED3_TCB;
static	CPU_STK	LED3_Stk[TASK_LED3_STK_SIZE];	
static	OS_TCB	AHRS_TCB;
static	CPU_STK	AHRS_Stk[TASK_AHRS_STK_SIZE];
/* 
 * ��������Task_Start
 * ����  : ��������
 *			   ���ȼ�Ϊ3��
 *		     ����LED1��LED2��LED3������
 * ����  ����
 * ���  : ��
 */
void Task_Start(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;	

	//��������LED1	
	OSTaskCreate((OS_TCB     *)&LED1_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"LED1",						            // ��������
               (OS_TASK_PTR )Task_LED1,					                // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_LED1_PRIO,				            // �������ȼ�
               (CPU_STK    *)&LED1_Stk[0],				                // �����ջ����ַ
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE/10,		                // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE,			            // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // ����ѡ��
               (OS_ERR     *)&err);						                // ����ֵ

    //��������LED2
	OSTaskCreate((OS_TCB     *)&LED2_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"LED2",						            // ��������
               (OS_TASK_PTR )Task_LED2,					                // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_LED2_PRIO,				            // �������ȼ�
               (CPU_STK    *)&LED2_Stk[0],				                // �����ջ����ַ
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE/10,		                // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE,			            // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // ����ѡ��
               (OS_ERR     *)&err);						                // ����ֵ
               
    //��������LED3
	OSTaskCreate((OS_TCB     *)&LED3_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"LED3",						            // ��������
               (OS_TASK_PTR )Task_LED3,					                // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_LED3_PRIO,				            // �������ȼ�
               (CPU_STK    *)&LED3_Stk[0],				                // �����ջ����ַ
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE/10,		                // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE,			            // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // ����ѡ��
               (OS_ERR     *)&err);						                // ����ֵ
               
   //��������AHRS
	OSTaskCreate((OS_TCB     *)&AHRS_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"AHRS",						            // ��������
               (OS_TASK_PTR )Task_AHRS,					                // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_AHRS_PRIO,				            // �������ȼ�
               (CPU_STK    *)&AHRS_Stk[0],				                // �����ջ����ַ
               (CPU_STK_SIZE)TASK_AHRS_STK_SIZE/10,		                // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_AHRS_STK_SIZE,			            // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // ����ѡ��
               (OS_ERR     *)&err);						                // ����ֵ
               
  //����ɾ���Լ�	
	OSTaskDel(&StartUp_TCB,&err);							 
}




