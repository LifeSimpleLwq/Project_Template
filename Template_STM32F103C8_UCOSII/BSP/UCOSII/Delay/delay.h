#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

	 
//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持UCOS
	 
	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























