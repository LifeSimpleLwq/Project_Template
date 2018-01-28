#include "BSP_Main.h"

#define START_TASK_PRIO 10
#define START_STK_SEZI 128
OS_STK START_TASK_STK[START_STK_SEZI];
// ������
void start_task(void *pdata);

#define LED0_TASK_PRIO 7
#define LED0_STK_SIZE 128
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
// led������
void led0_task(void *pdata);

#define UART_TASK_PRIO 8
#define UART_STK_SIZE 128
OS_STK UART_TASK_STK[UART_STK_SIZE];
// �����������
void uart_task(void *pdata);

/**
  * @brief  ������
  * @param  void
  * @retval None
  */
void BSP_Main(void)
{
	printf("STM32F103C8T6 HAL_Project\r\n");
	printf("By��Simple\r\n");
	printf("Date��9/3/2017\r\n");
	
	delay_init();
	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK*)&START_TASK_STK[START_STK_SEZI-1],START_TASK_PRIO);
	OSStart();

}

/**
  * @brief  ��ʼ����
  * @param  void
  * @retval None
  */
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //����ͳ������
	
	OS_ENTER_CRITICAL();  //�����ٽ���(�ر��ж�)

	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);//����LED0����
	OSTaskCreate(uart_task,(void*)0,(OS_STK*)&UART_TASK_STK[UART_STK_SIZE-1],UART_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);//����ʼ����
	OS_EXIT_CRITICAL();  //�˳��ٽ���(���ж�)
}

/**
  * @brief  LED0����
  * @param  void
  * @retval None
  */
void led0_task(void *pdata)
{
	while(1)
	{
		delay_ms(500);
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	}
}

/**
  * @brief  �����������
  * @param  void
  * @retval None
  */
void uart_task(void *pdata)
{
	while(1)
	{
		delay_ms(1000);
		printf("hello\r\n");
	}
}

