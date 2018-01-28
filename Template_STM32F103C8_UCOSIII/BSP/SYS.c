#include "SYS.h"



static u8  fac_us=9; // �δ�ʱ����ʱ��Ƶ�ʣ��δ�ʱ��ʱ��Ϊ: HCLK/8

/**
  * @brief  ��ʱnus
  * @param  nusΪҪ��ʱ��us��.
  * @retval None
  */
void BSP_Delay_us(u32 nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;
	//LOAD��ֵ

	told=SysTick->VAL;
	//�ս���ʱ�ļ�����ֵ
	ticks=nus*fac_us;
	//��Ҫ�Ľ�����
	tcnt=0;

	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;
			//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;
			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}





