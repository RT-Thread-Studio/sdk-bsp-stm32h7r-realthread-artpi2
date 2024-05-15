/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-15     RT-Thread    first version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
    #include <rtdevice.h>
#endif /* RT_USING_NANO */
#include <drv_common.h>
#include "w35t51nwtbie.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(M, 3)

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x70000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;

XSPI_HandleTypeDef hxspi1;
XSPI_HandleTypeDef hxspi2;

/**
  * @brief XSPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_XSPI2_Init(void)
{

    XSPIM_CfgTypeDef sXspiManagerCfg = {0};

    /* USER CODE BEGIN XSPI2_Init 1 */

    /* USER CODE END XSPI2_Init 1 */
    /* XSPI2 parameter configuration*/
    hxspi2.Instance = XSPI2;
    hxspi2.Init.FifoThresholdByte = 4;
    hxspi2.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
    hxspi2.Init.MemoryType = HAL_XSPI_MEMTYPE_MICRON;
    hxspi2.Init.MemorySize = HAL_XSPI_SIZE_32GB;
    hxspi2.Init.ChipSelectHighTimeCycle = 2;
    hxspi2.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
    hxspi2.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
    hxspi2.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
    hxspi2.Init.ClockPrescaler = 1;
    hxspi2.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
    hxspi2.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
    hxspi2.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_NONE;
    hxspi2.Init.MaxTran = 0;
    hxspi2.Init.Refresh = 0;
    hxspi2.Init.MemorySelect = HAL_XSPI_CSSEL_NCS1;
    if (HAL_XSPI_Init(&hxspi2) != HAL_OK)
    {
        Error_Handler();
    }
    sXspiManagerCfg.nCSOverride = HAL_XSPI_CSSEL_OVR_NCS1;
    sXspiManagerCfg.IOPort = HAL_XSPIM_IOPORT_2;
    if (HAL_XSPIM_Config(&hxspi2, &sXspiManagerCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN XSPI2_Init 2 */

    /* USER CODE END XSPI2_Init 2 */

}

/**
  * @brief XSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_XSPI1_Init(void)
{
    /* XSPI initialization */
    hxspi1.Instance = XSPI1;

    hxspi1.Init.FifoThresholdByte          = 2U;
    hxspi1.Init.MemoryMode                 = HAL_XSPI_SINGLE_MEM;
    hxspi1.Init.MemoryType                 = HAL_XSPI_MEMTYPE_APMEM_16BITS;
    hxspi1.Init.MemorySize                 = HAL_XSPI_SIZE_32GB;
    hxspi1.Init.ChipSelectHighTimeCycle    = 1U;
    hxspi1.Init.FreeRunningClock           = HAL_XSPI_FREERUNCLK_DISABLE;
    hxspi1.Init.ClockMode                  = HAL_XSPI_CLOCK_MODE_0;
    hxspi1.Init.WrapSize                   = HAL_XSPI_WRAP_32_BYTES;
    hxspi1.Init.ClockPrescaler             = 0U;
    hxspi1.Init.SampleShifting             = HAL_XSPI_SAMPLE_SHIFT_NONE;
    hxspi1.Init.DelayHoldQuarterCycle      = HAL_XSPI_DHQC_ENABLE;
    hxspi1.Init.ChipSelectBoundary         = HAL_XSPI_BONDARYOF_8KB;
    hxspi2.Init.MaxTran                    = 0U;
    hxspi2.Init.Refresh                    = 0U;
    hxspi2.Init.MemorySelect               = HAL_XSPI_CSSEL_NCS1;

    /* Initialize XSPI */
    if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

int main(void)
{
    uint8_t device_id[6];
    int retr = 0;
    /* Enable the power of PORT_M */
    PWR->CSR2 |= PWR_CSR2_USB33DEN;
    /* Wait the detection of the USB33 power */
    uint32_t tickstart = HAL_GetTick();
    while ((PWR->CSR2 & PWR_CSR2_USB33RDY) == 0U)
    {
    }
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    MX_XSPI1_Init();
    MX_XSPI2_Init();
    HAL_RCCEx_EnableClockProtection(RCC_CLOCKPROTECT_XSPI);

    if (W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2,
            W35T51NWTBIE_SPI_MODE,
            W35T51NWTBIE_STR_TRANSFER,
            device_id, W35T51NWTBIE_3BYTES_SIZE))

    {
        LOG_E("Read ID Fail");
    }
    else
    {
        LOG_I("Read ID success:%.2x %.2x %.2x", device_id[0], device_id[1], device_id[2]);
        retr =  W35T51NWTBIE_EnterOctal_DTR_Mode(&hxspi2);
        LOG_D("Enter octal:%d", retr);
        if (W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2,
                W35T51NWTBIE_OPI_MODE,
                W35T51NWTBIE_DTR_TRANSFER,
                device_id, W35T51NWTBIE_4BYTES_SIZE))
        {
            LOG_E("Enter octal failed");

        }
        else
        {
            LOG_I("Enter Octal DTR and Read ID success:%.2x %.2x %.2x", device_id[0], device_id[1], device_id[2]);
        }
    }

    /* enable the region corresponding to the memory */
    {
        /* Enter critical section to lock the system and avoid any issue around MPU mechanism */
        MPU_Region_InitTypeDef default_config = {0};
        uint32_t primask_bit = __get_PRIMASK();
        __disable_irq();

        /* disable the MPU */
        HAL_MPU_Disable();

        /* Create a region associated with memory address 0x70000000 */
        /* TEX=1, Normal memory type, code execution allowed */
        default_config.Enable = MPU_REGION_ENABLE;
        default_config.Number = 1;
        default_config.BaseAddress = 0x70000000;
        default_config.Size = MPU_REGION_SIZE_128MB;
        default_config.SubRegionDisable = 0;
        default_config.TypeExtField = MPU_TEX_LEVEL1;
        default_config.AccessPermission = MPU_REGION_FULL_ACCESS;
        default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
        default_config.IsShareable = MPU_ACCESS_SHAREABLE;
        default_config.IsCacheable = MPU_ACCESS_CACHEABLE;
        default_config.IsBufferable = MPU_ACCESS_BUFFERABLE;
        HAL_MPU_ConfigRegion(&default_config);

        /* enable the MPU */
        HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

        /* Exit critical section to lock the system and avoid any issue around MPU mechanisme */
        __set_PRIMASK(primask_bit);
    }

    W35T51NWTBIE_EnableMemoryMappedModeDTR(&hxspi2, RT_NULL);

    rt_thread_mdelay(300);

    SysTick->CTRL = 0;

    JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    JumpToApplication();

    return RT_EOK;
}
