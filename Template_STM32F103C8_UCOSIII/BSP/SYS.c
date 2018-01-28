#include "SYS.h"



static u8  fac_us=9; // 滴答定时器的时钟频率，滴答定时器时钟为: HCLK/8

/**
  * @brief  延时nus
  * @param  nus为要延时的us数.
  * @retval None
  */
void BSP_Delay_us(u32 nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;
	//LOAD的值

	told=SysTick->VAL;
	//刚进入时的计数器值
	ticks=nus*fac_us;
	//需要的节拍数
	tcnt=0;

	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;
			//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;
			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}





