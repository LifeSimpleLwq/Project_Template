#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

	 
//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_OS		1		//����ϵͳ�ļ����Ƿ�֧��UCOS
	 
	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























