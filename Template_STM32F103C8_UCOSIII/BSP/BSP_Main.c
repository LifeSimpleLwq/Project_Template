#include "BSP_Main.h"
#include "includes.h"
#include "delay.h"
#include <string.h>


//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED0_TASK_PRIO		4
//�����ջ��С	
#define LED0_STK_SIZE 		128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);


//�������ȼ�
#define FLOAT_TASK_PRIO		6
//�����ջ��С
#define FLOAT_STK_SIZE		128
//������ƿ�
OS_TCB	FloatTaskTCB;
//�����ջ
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//������
void float_task(void *p_arg);


/**
  * @brief  ������
  * @param  void
  * @retval None
  */
void BSP_Main(void)
{
	OS_ERR err;
	
	CPU_SR_ALLOC();
	delay_init();       //��ʱ��ʼ��
	
	printf("STM32F103C8T6 HAL_Project\r\n");
	printf("By��Simple\r\n");
	printf("Date��8/30/2017\r\n");
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
	
//	while(1)
//	{
//		BSP_Delay_us(1000000);
//		printf("hello work\r\n");
//	}
}

OS_SEM MY_SEM;
//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED0����
	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
				 (CPU_CHAR	* )"led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK   * )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
			 
	//���������������
	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
				 (CPU_CHAR	* )"float test task", 		
                 (OS_TASK_PTR )float_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			

	OSSemCreate((OS_SEM *)&MY_SEM,
							(CPU_CHAR *)"MY_SEM",
							(OS_SEM_CTR)1,
							(OS_ERR*  )&err);
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}


u8 share_resource[30];
//led0������
void led0_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	
	u8 task1_str[]="frist task Running!";
	
	while(1)
	{
		printf("����1:\r\n");
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);
		memcpy(share_resource,task1_str,sizeof(task1_str));
		delay_ms(200);
		
		printf("%s\r\n",share_resource);
		OSSemPost(&MY_SEM,OS_OPT_POST_1,&err);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
	}
}



//�����������
void float_task(void *p_arg)
{
	OS_ERR err;
	
	CPU_SR_ALLOC();
	static float float_num=0.01;
	u8 task2_str[]="Second task Running!";
	
	while(1)
	{
		printf("����2:\r\n");
		OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);
		memcpy(share_resource,task2_str,sizeof(task2_str));
		delay_ms(200);

		printf("%s\r\n",share_resource);
		OSSemPost(&MY_SEM,OS_OPT_POST_1,&err);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
//		float_num+=0.01f;
//		OS_CRITICAL_ENTER();	//�����ٽ���
//		printf("float_num��ֵΪ: %.4f\r\n",float_num);
//		OS_CRITICAL_EXIT();		//�˳��ٽ���
//		delay_ms(500);			//��ʱ500ms
	}
}


