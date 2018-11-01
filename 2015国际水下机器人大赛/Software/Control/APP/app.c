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
 * 函数名：Task_Start
 * 描述  : 启动任务，
 *			   优先级为3，
 *		     创建LED1、LED2和LED3的任务
 * 输入  ：无
 * 输出  : 无
 */
void Task_Start(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;	

	//创建任务LED1	
	OSTaskCreate((OS_TCB     *)&LED1_TCB,					            // 任务控制块指针          
               (CPU_CHAR   *)"LED1",						            // 任务名称
               (OS_TASK_PTR )Task_LED1,					                // 任务代码指针
               (void       *)0,							                // 传递给任务的参数parg
               (OS_PRIO     )TASK_LED1_PRIO,				            // 任务优先级
               (CPU_STK    *)&LED1_Stk[0],				                // 任务堆栈基地址
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE/10,		                // 堆栈剩余警戒线
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE,			            // 堆栈大小
               (OS_MSG_QTY  )0,							                // 可接收的最大消息队列数
               (OS_TICK     )0,							                // 时间片轮转时间
               (void       *)0,							                // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // 任务选项
               (OS_ERR     *)&err);						                // 返回值

    //创建任务LED2
	OSTaskCreate((OS_TCB     *)&LED2_TCB,					            // 任务控制块指针          
               (CPU_CHAR   *)"LED2",						            // 任务名称
               (OS_TASK_PTR )Task_LED2,					                // 任务代码指针
               (void       *)0,							                // 传递给任务的参数parg
               (OS_PRIO     )TASK_LED2_PRIO,				            // 任务优先级
               (CPU_STK    *)&LED2_Stk[0],				                // 任务堆栈基地址
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE/10,		                // 堆栈剩余警戒线
               (CPU_STK_SIZE)TASK_LED2_STK_SIZE,			            // 堆栈大小
               (OS_MSG_QTY  )0,							                // 可接收的最大消息队列数
               (OS_TICK     )0,							                // 时间片轮转时间
               (void       *)0,							                // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // 任务选项
               (OS_ERR     *)&err);						                // 返回值
               
    //创建任务LED3
	OSTaskCreate((OS_TCB     *)&LED3_TCB,					            // 任务控制块指针          
               (CPU_CHAR   *)"LED3",						            // 任务名称
               (OS_TASK_PTR )Task_LED3,					                // 任务代码指针
               (void       *)0,							                // 传递给任务的参数parg
               (OS_PRIO     )TASK_LED3_PRIO,				            // 任务优先级
               (CPU_STK    *)&LED3_Stk[0],				                // 任务堆栈基地址
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE/10,		                // 堆栈剩余警戒线
               (CPU_STK_SIZE)TASK_LED3_STK_SIZE,			            // 堆栈大小
               (OS_MSG_QTY  )0,							                // 可接收的最大消息队列数
               (OS_TICK     )0,							                // 时间片轮转时间
               (void       *)0,							                // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // 任务选项
               (OS_ERR     *)&err);						                // 返回值
               
   //创建任务AHRS
	OSTaskCreate((OS_TCB     *)&AHRS_TCB,					            // 任务控制块指针          
               (CPU_CHAR   *)"AHRS",						            // 任务名称
               (OS_TASK_PTR )Task_AHRS,					                // 任务代码指针
               (void       *)0,							                // 传递给任务的参数parg
               (OS_PRIO     )TASK_AHRS_PRIO,				            // 任务优先级
               (CPU_STK    *)&AHRS_Stk[0],				                // 任务堆栈基地址
               (CPU_STK_SIZE)TASK_AHRS_STK_SIZE/10,		                // 堆栈剩余警戒线
               (CPU_STK_SIZE)TASK_AHRS_STK_SIZE,			            // 堆栈大小
               (OS_MSG_QTY  )0,							                // 可接收的最大消息队列数
               (OS_TICK     )0,							                // 时间片轮转时间
               (void       *)0,							                // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	                    // 任务选项
               (OS_ERR     *)&err);						                // 返回值
               
  //任务删除自己	
	OSTaskDel(&StartUp_TCB,&err);							 
}




