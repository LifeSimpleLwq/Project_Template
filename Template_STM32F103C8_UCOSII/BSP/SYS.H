#ifndef __SYS_H
#define __SYS_H


#include "stm32f1xx_hal.h"
#include "usart.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

void BSP_Delay_us(u32 nus);

#endif 
