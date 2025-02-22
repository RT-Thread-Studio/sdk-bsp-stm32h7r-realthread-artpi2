/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-15     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <extmem_manager.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define LED_RED GET_PIN(O, 5)
#define LED_BLUE GET_PIN(O, 1)

#define APPLICATION_ADDRESS XSPI2_BASE  //(uint32_t)0x70000000

int JumpToApplication(void)
{
    uint32_t primask_bit;
    typedef void (*pFunction)(void);
    pFunction JumpToApp;
    uint32_t Application_vector;

    /* Suspend SysTick */
    SysTick->CTRL = 0;

    /* Disable I-Cache---------------------------------------------------------*/
    SCB_DisableICache();  //TODO SCB_Disables Cache and jump success

    /* Disable D-Cache---------------------------------------------------------*/
    SCB_DisableDCache();

    /* Apply offsets for image location and vector table offset */
    //  Application_vector += EXTMEM_XIP_IMAGE_OFFSET + EXTMEM_HEADER_OFFSET;
    Application_vector = APPLICATION_ADDRESS;
    SCB->VTOR = (uint32_t)Application_vector;
    JumpToApp = (pFunction)(*(__IO uint32_t *)(Application_vector + 4u));

    __set_MSP(*(__IO uint32_t *)Application_vector);
    __set_CONTROL(0);

    JumpToApp();
    return 0;
}

/**
  * @brief FLASH Initialization Function
  * @param None
  * @retval None
  */
static void MX_FLASH_Init(void)
{

    FLASH_OBProgramInitTypeDef pOBInit = {0};

    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_FLASH_OB_Unlock() != HAL_OK)
    {
        Error_Handler();
    }
    pOBInit.OptionType = OPTIONBYTE_USER;
    pOBInit.USERType = OB_USER_IWDG_SW | OB_USER_NRST_STOP
                       | OB_USER_NRST_STDBY | OB_USER_VDDIO_HSLV
                       | OB_USER_IWDG_STOP | OB_USER_IWDG_STDBY
                       | OB_USER_XSPI1_HSLV | OB_USER_XSPI2_HSLV
                       | OB_USER_I2C_NI3C;
    pOBInit.USERConfig1 = OB_IWDG_SW | OB_STOP_NORST
                          | OB_STANDBY_NORST | OB_VDDIO_HSLV_DISABLE
                          | OB_IWDG_STOP_RUN | OB_IWDG_STDBY_RUN
                          | OB_XSPI1_HSLV_ENABLE | OB_XSPI2_HSLV_ENABLE;
    pOBInit.USERConfig2 = OB_I2C_NI3C_I2C;
    if (HAL_FLASHEx_OBProgram(&pOBInit) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_FLASH_OB_Lock() != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_FLASH_Lock() != HAL_OK)
    {
        Error_Handler();
    }

}

int main(void)
{
    MX_FLASH_Init();
    EXTMEM_Init();

    EXTMEM_Flash_Probe();
    EXTMEM_PSRAM_Probe();
    EXTMEM_Flash_EnterXIP();
    EXTMEM_PSRAM_EnterXIP();

    rt_kprintf("\nJump to APP...\n");

    rt_hw_interrupt_disable();

    JumpToApplication();

    return RT_EOK;
}
