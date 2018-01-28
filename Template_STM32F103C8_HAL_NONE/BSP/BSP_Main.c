#include "BSP_Main.h"


/**
  * @brief  Ö÷º¯Êý
  * @param  void
  * @retval None
  */
void BSP_Main(void)
{
	u32 i = 0x11111111,b;
	printf("STM32F103C8T6 HAL_Project\r\n");
	printf("By£ºSimple\r\n");
	printf("Date£º8/30/2017\r\n");
	
	BSP_FLASHWrite(Test,&i,1);
	b = BSP_FLASHReadWord(Test);
	printf("%x\r\n",b);
	
	while(1)
	{
		printf("hello work\r\n");
		BSP_Delay_us(8000000);
	}
}




