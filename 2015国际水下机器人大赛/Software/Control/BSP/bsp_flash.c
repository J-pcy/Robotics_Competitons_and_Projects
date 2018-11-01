#include "bsp_flash.h"


Flash_Data_TypeDef Flash_Data;


typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x0800C000)

uint32_t NbrOfPage;
uint32_t EraseCounter;
uint32_t Address;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED;

bool BSP_FLASH_Write(uint32_t *pData, uint8_t Size)
{
    uint8_t count=0;

    
    //解锁 FLASH 的 Bank1
    FLASH_UnlockBank1();
    
    //计算要擦除的页面数额
    NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
    
    //清除标志位
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    
    //擦除 FLASH 的页面
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }
    
    //对 FLASH 的 Bank1 进行编程
    Address = BANK1_WRITE_START_ADDR;
    
    while((Address < BANK1_WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
        if (count < Size)
            FLASHStatus = FLASH_ProgramWord(Address, pData[count]);
        else
            FLASHStatus = FLASH_ProgramWord(Address, 0);
        
        Address = Address + 4;
        
        count++;
    }
    
    //锁定 FLASH 的 Bank1
    FLASH_LockBank1();
    
    //校验
    Address = BANK1_WRITE_START_ADDR;
    
    count = 0;
    
    while((Address < BANK1_WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
    {
        if (count < Size)
        {
            if((*(__IO uint32_t*) Address) != pData[count])
            {
                MemoryProgramStatus = FAILED;
            }
        }
        
        Address += 4;
        
        count ++;
    }
    
    if (MemoryProgramStatus == FAILED)
        return false;
    else
        return true;
}

void BSP_FLASH_Read(uint32_t *pData,uint8_t Size)
{
    uint8_t i;
    
    for (i=0;i<Size;i++)
    {
        *pData = *(vu32*)(BANK1_WRITE_START_ADDR + i*4);
        pData++;
    }


}


