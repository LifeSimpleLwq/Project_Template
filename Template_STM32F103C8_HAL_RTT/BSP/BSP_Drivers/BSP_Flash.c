#include "BSP_flash.h"

/***
	��ֲע���������MCU FLASH��ҳ��С�޸�STM_SECTOR_SIZE
*/

#define STM_SECTOR_SIZE 1024 // �ֽ�
#define Page_SIZE 	STM_SECTOR_SIZE		// FLASHһҳ�洢�ռ��С
#define Page_Num		Page_SIZE/4			// ��4���ֽ�Ϊ��λ��ע�⣬�˴���д������ݿ���й�

u32 BSP_FLASHBUF[STM_SECTOR_SIZE/4];

/**
  * @brief  ָ��λ��д����֣�16λ��
  * @param  WriteAddr :	д���ַ
  * @param  pBuffer ��д������	
  * @retval NONE
  */
void BSP_FLASHWriteWord(u32 WriteAddr,u16 pBuffer)
{
		u32 buff_u32;
		u16 buff_u16;
		
		buff_u16 = BSP_FLASHReadWord(WriteAddr);
	
		buff_u32 = buff_u16 << 16;
		buff_u32 += pBuffer;
	
		BSP_FLASHWrite(WriteAddr,&buff_u32,1);
}

/**
  * @brief  ��ȡָ����ַ����
  * @param  faddr :��ȡ�ĵ�ַ
					�õ�ַ������ 4 �ı���
  * @retval ��ȡ������
  */
u32 BSP_FLASHReadWord(u32 faddr)
{
	return *(__IO u32*)faddr; 
}

/**
  * @brief  ������д��
  * @param  WriteAddr: ��ʼ��ַ
  * @param  pBuffer: д�����ݵĻ�����
  * @param  NumToWrite: д�����ݵĴ�С(��λ: ��)
  * @retval None
  */
void BSP_FLASHWrite_NoCheck(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)   
{
	u16 i;
	
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(TYPEPROGRAM_WORD, WriteAddr, pBuffer[i]);
		WriteAddr+=4;
		// ��ַ����4.
	}  
}

/**
  * @brief  ����һҳ
  * @param  Page_Address :Ҫ������ҳ��ַ
  * @retval None
  */
void FLASH_ErasePage(u32 Page_Address)
{
	u32 PageError = 0;
	FLASH_EraseInitTypeDef f;

	f.TypeErase = FLASH_TYPEERASE_PAGES;
	f.PageAddress = Page_Address;
	f.NbPages = 1;

	HAL_FLASHEx_Erase(&f, &PageError);
}

/**
  * @brief  �� Flash д����(һҳд��256�ֽ�)
  * @param  WriteAddr: ��ʼ��ַ
  * @param  pBuffer: д�����ݵĻ�����  
  * @param  NumToWrite: д�����ݵĴ�С(��λ: ��)  
  * @retval None	  
  */
void BSP_FLASHWrite(u32 WriteAddr,u32 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;
	// ������ַ
	u16 secoff;
	// ������ƫ�Ƶ�ַ(32λ����)
	u16 secremain;
	// ������ʣ���ַ(32λ����)
 	u16 i;    
	u32 offaddr;
	u32 Numsize;

	if(WriteAddr < STM32_FLASH_BASE||(WriteAddr >= (STM32_FLASH_BASE+STM_SECTOR_SIZE*STM32_FLASH_SIZE)))
		return;
	// �Ƿ���ַ  

	HAL_FLASH_Unlock();
	// ����

	Numsize = NumToWrite;  // д���ֽ� 
	// ����д��ĳ���  
	offaddr = WriteAddr - STM32_FLASH_BASE;  
	// ʵ��ƫ�Ƶ�ַ. 
	secpos = offaddr/(Page_SIZE);    
	// ������ַ  0~63 for STM32F103C8
	secoff = (offaddr%(Page_SIZE))/4;   	
	// �������ڵ�ƫ��( 4���ֽ�Ϊ������λ. )
	secremain = Page_Num - secoff;		  
	// ����ʣ��ռ��С

	if(Numsize <= secremain)	secremain = Numsize;  // �����ڸ�������Χ
	
	while(1) 
	{
		BSP_FLASHRead (secpos*Page_SIZE+STM32_FLASH_BASE, BSP_FLASHBUF, Page_Num);		
		// ������������������

		for (i = 0; i < secremain; i++)
		// У������
		{
			if (BSP_FLASHBUF[secoff+i] != 0x00000000)
				break;
			// ��Ҫ����
		}

		if (i<secremain)
			// ��Ҫ����
		{
			FLASH_ErasePage (secpos*Page_SIZE+STM32_FLASH_BASE); 
			FLASH_WaitForLastOperation (500);
			// �����������
			for (i=0;i<secremain;i++)
			// ����
			{
				BSP_FLASHBUF[i+secoff] = pBuffer[i];
			}
			BSP_FLASHWrite_NoCheck (secpos*Page_SIZE+STM32_FLASH_BASE, BSP_FLASHBUF, Page_Num);   
			// д����������
			FLASH_WaitForLastOperation (500);
		}
		else
		{
			BSP_FLASHWrite_NoCheck (WriteAddr, pBuffer, secremain);
			FLASH_WaitForLastOperation (500);
		}

		// д�Ѿ������˵�,ֱ��д������ʣ������.
		if(Numsize == secremain)
			break;
		// д�������
		else
			// д��δ����
		{
			secpos++;  //  20
			// ������ַ��1
			secoff = 0;  
			// ƫ��λ��Ϊ0 	 
			pBuffer += secremain;
			// ָ��ƫ��
			WriteAddr += (secremain*4);
			// д��ַƫ��	   
			Numsize -= secremain;
			// �ֽ�(16λ)���ݼ�
			if(Numsize > (Page_Num))
				secremain = Page_Num;
			// ��һ����������д����
			else
				secremain = Numsize;
			// ��һ����������д����
		}	 
	}
	HAL_FLASH_Lock();
	// ����
}

/**
  * @brief  ��ָ����ַ��ʼ����ָ�����ȵ�����
  * @param  ReadAddr: ��ʼ��ַ
  * @param  pBuffer: �������ݵĻ�����
  * @param  NumToWrite: �������ݵĴ�С(��λ: ��)
  * @retval None
  */
void BSP_ReadByte(u32 ReadAddr,u8 *pBuffer)
{
	u32 tmp;

	BSP_FLASHRead(ReadAddr,&tmp,1);
	*pBuffer = (u8)tmp;
}

/**
  * @brief  ��ָ����ַ��ʼ����ָ�����ȵ�����
  * @param  ReadAddr: ��ʼ��ַ
  * @param  pBuffer: �������ݵĻ�����
  * @param  NumToWrite: �������ݵĴ�С(��λ: ��)
  * @retval None
  */
void BSP_FLASHRead(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)   	
{
	u16 i;

	for(i=0; i<NumToRead; i++)
	{
		pBuffer[i] = BSP_FLASHReadWord(ReadAddr);
		// ��ȡ 4 ���ֽ�
		ReadAddr += 4;
		// ƫ�� 4 ���ֽ�
	}
}






