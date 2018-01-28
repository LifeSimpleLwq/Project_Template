#include "BSP_Main.h"

#define START_TASK_PRIO 10
#define START_STK_SEZI 128
OS_STK START_TASK_STK[START_STK_SEZI];
// 任务函数
void start_task(void *pdata);

#define LED0_TASK_PRIO 7
#define LED0_STK_SIZE 128
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
// led任务函数
void led0_task(void *pdata);

#define UART_TASK_PRIO 8
#define UART_STK_SIZE 128
OS_STK UART_TASK_STK[UART_STK_SIZE];
// 串口输出函数
void uart_task(void *pdata);

/**
  * @brief  主函数
  * @param  void
  * @retval None
  */
void BSP_Main(void)
{
	printf("STM32F103C8T6 HAL_Project\r\n");
	printf("By：Simple\r\n");
	printf("Date：9/3/2017\r\n");
	
	delay_init();
	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK*)&START_TASK_STK[START_STK_SEZI-1],START_TASK_PRIO);
	OSStart();

}

/**
  * @brief  开始任务
  * @param  void
  * @retval None
  */
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //开启统计任务
	
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)

	OSTaskCreate(led0_task,(void*)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);//创建LED0任务
	OSTaskCreate(uart_task,(void*)0,(OS_STK*)&UART_TASK_STK[UART_STK_SIZE-1],UART_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL();  //退出临界区(开中断)
}

/**
  * @brief  LED0任务
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
  * @brief  串口输出函数
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

