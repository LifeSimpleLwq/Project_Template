#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "SYS.h"

#define STM32_FLASH_SIZE 64		//��ѡSTM32��FLASH������С(��λΪK)

#define STM32_FLASH_BASE 0x08000000			//FLASH��ʼ��ַ
#define Test				 0x08003800				  //����ʹ��

void BSP_FLASHWrite(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite);
void BSP_FLASHWrite_NoCheck(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite);
void BSP_FLASHWriteWord(u32 WriteAddr,u16 pBuffer);

void BSP_FLASHRead(u32 ReadAddr,u32 *pBuffer,u16 NumToRead);
void BSP_ReadByte(u32 ReadAddr,uint8_t *pBuffer);
u32 BSP_FLASHReadWord(u32 faddr);

	
#endif

