#include "BSP_Main.h"

static struct rt_thread flag1_thread;
ALIGN(RT_ALIGN_SIZE)
rt_uint8_t flag1_stack[512];


void flag1_thread_entry(void *parameter)
{
	while(1)
	{
		printf("hello work\r\n");
		rt_thread_delay(RT_TICK_PER_SECOND/10);
	}
}

void flag1_Init(void)
{
	rt_err_t err_t;
	
	err_t = rt_thread_init(&flag1_thread,
									"flag1",
									flag1_thread_entry,
									RT_NULL,
									&flag1_stack[0],
									sizeof(flag1_stack),
									20,
									5);
	if (err_t == RT_EOK)
	{
		rt_thread_startup(&flag1_thread);
	}		
}

/**
  * @brief  Ö÷º¯Êý
  * @param  void
  * @retval None
  */
void BSP_Main(void)
{
//	u32 i = 0x11111111,b;
	printf("STM32F103C8T6 HAL_Project\r\n");
	printf("By£ºSimple\r\n");
	printf("Date£º8/30/2017\r\n");
	
	flag1_Init();
	
//	BSP_FLASHWrite(Test,&i,1);
//	b = BSP_FLASHReadWord(Test);
//	printf("%x\r\n",b);
		
//	while(1)
//	{
//		printf("hello work\r\n");
//		BSP_Delay_us(8000000);
//	}
}




