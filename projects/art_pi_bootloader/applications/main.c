/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
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
#include <drv_common.h>
#include "w35t51nwtbie.h"
#include "aps256xx.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

/* defined the LED0 pin: PB1 */
#define LED_RED GET_PIN(O, 5)
#define LED_BLUE GET_PIN(O, 1)

#define APPLICATION_ADDRESS XSPI2_BASE  //(uint32_t)0x70000000

XSPI_HandleTypeDef hxspi1;
XSPI_HandleTypeDef hxspi2;
ADC_HandleTypeDef hadc1;
/**
 * @brief XSPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_XSPI1_Init(void) // PSRAM
{

    /* USER CODE BEGIN XSPI1_Init 0 */
    uint32_t xspi1_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI1);
    /* USER CODE END XSPI1_Init 0 */

    XSPIM_CfgTypeDef sXspiManagerCfg = {0};

    /* USER CODE BEGIN XSPI1_Init 1 */

    /* USER CODE END XSPI1_Init 1 */
    /* XSPI1 parameter configuration*/
    hxspi1.Instance = XSPI1;
    hxspi1.Init.FifoThresholdByte = 1;
    hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
    hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM_16BITS;
    hxspi1.Init.MemorySize = HAL_XSPI_SIZE_32GB;
    hxspi1.Init.ChipSelectHighTimeCycle = 1;
    hxspi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
    hxspi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
    hxspi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;         // HAL_XSPI_WRAP_32_BYTES;
    hxspi1.Init.ClockPrescaler = 0;
    hxspi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;    // HAL_XSPI_SAMPLE_SHIFT_HALFCYCLE;
    hxspi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
    hxspi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_8KB;
    hxspi1.Init.MaxTran = 0;
    hxspi1.Init.Refresh = 0;        //((2U * (xspi1_clk / (hxspi1.Init.ClockPrescaler + 1U))) / 1000000U) - 4U;
    hxspi1.Init.MemorySelect = HAL_XSPI_CSSEL_NCS1;
    if (HAL_XSPI_Init(&hxspi1) != HAL_OK)
    {
        Error_Handler();
    }
    sXspiManagerCfg.nCSOverride = HAL_XSPI_CSSEL_OVR_NCS1;
    sXspiManagerCfg.IOPort = HAL_XSPIM_IOPORT_1;
    if (HAL_XSPIM_Config(&hxspi1, &sXspiManagerCfg, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN XSPI1_Init 2 */

    /* USER CODE END XSPI1_Init 2 */
}

/**
 * @brief XSPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_XSPI2_Init(void) // Octal Flash
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
    hxspi2.Init.ClockPrescaler = 0;
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

int JumpToApplication(void)
{
    uint32_t primask_bit;
    typedef void (*pFunction)(void);
    pFunction JumpToApp;
    uint32_t Application_vector;

    /* Suspend SysTick */
    HAL_SuspendTick();

    /* Disable I-Cache---------------------------------------------------------*/
//    SCB_DisableICache();  //TODO SCB_Disables Cache and jump success

    /* Disable D-Cache---------------------------------------------------------*/
//    SCB_DisableDCache();

    /* Initialize user application's Stack Pointer & Jump to user application  */
    primask_bit = __get_PRIMASK();
    __disable_irq();

    /* Apply offsets for image location and vector table offset */
    //  Application_vector += EXTMEM_XIP_IMAGE_OFFSET + EXTMEM_HEADER_OFFSET;
    Application_vector = APPLICATION_ADDRESS;
    SCB->VTOR = (uint32_t)Application_vector;
    JumpToApp = (pFunction)(*(__IO uint32_t *)(Application_vector + 4u));

#if ((defined(__ARM_ARCH_8M_MAIN__) && (__ARM_ARCH_8M_MAIN__ == 1)) ||     \
     (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ == 1)) || \
     (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ == 1)))
    /* on ARM v8m, set MSPLIM before setting MSP to avoid unwanted stack overflow faults */
    __set_MSPLIM(0x00000000);
#endif /* __ARM_ARCH_8M_MAIN__ or __ARM_ARCH_8M_BASE__ */

    __set_MSP(*(__IO uint32_t *)Application_vector);

    /* Re-enable the interrupts */
    __set_PRIMASK(primask_bit);

    JumpToApp();
    return 0;
}

int main(void)
{
    uint8_t device_id[6];
    XSPI_RegularCmdTypeDef sCommand = {0};
    int retr = 0;

    uint32_t ReadLatencyCode = 7;
    uint32_t WriteLatencyCode = 7;
    XSPI_HSCalTypeDef cal = {0};

    /* set LED0 pin mode to output */
    rt_pin_mode(LED_RED, PIN_MODE_OUTPUT);
    MX_XSPI1_Init();
    MX_XSPI2_Init();

    HAL_XSPI_GetDelayValue(&hxspi1, &cal);
    LOG_D("cal delay: 0x%.2x, fine: 0x%.2x, coarse 0x%.2x, max 0x%.2x", cal.DelayValueType, cal.FineCalibrationUnit, cal.CoarseCalibrationUnit, cal.MaxCalibration);
    if (W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2,
                                               W35T51NWTBIE_SPI_MODE,
                                               W35T51NWTBIE_STR_TRANSFER,
                                               device_id, W35T51NWTBIE_3BYTES_SIZE))

    {
        LOG_E("Read ID Fail");
    }
    else
    {
        LOG_D("Read ID success:%.2x %.2x %.2x", device_id[0], device_id[1], device_id[2]);
        retr = W35T51NWTBIE_EnterOctal_DTR_Mode(&hxspi2);
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

    if (W35T51NWTBIE_OK != W35T51NWTBIE_EnableMemoryMappedModeDTR(&hxspi2, RT_NULL))
    {
        LOG_E("XIP octal failed");
        return -1;
    }

    /* Configure the 16-bits Octal RAM memory ***************************/
    uint8_t reg[2];

    if (APS256XX_OK != APS256XX_Reset(&hxspi1))
    {
        LOG_E("PSRAM Reset error");
    }
    rt_thread_mdelay(20);

    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR0_ADDRESS, reg, 5))
    {
        LOG_D("PSRAM MR0: 0x%.2x, MR1: 0x%.2x", reg[0], reg[1]);
    }

    MODIFY_REG(reg[0], ((uint8_t)APS256XX_MR0_LATENCY_TYPE | (uint8_t)APS256XX_MR0_READ_LATENCY_CODE | (uint8_t)APS256XX_MR0_DRIVE_STRENGTH),
               ((uint8_t)APS256XX_MR0_LATENCY_TYPE_FIXED | (uint8_t)APS256XX_MR0_RLC_7 | (uint8_t)APS256XX_MR0_DS_HALF));
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR0_ADDRESS, reg[0]))
    {
        LOG_D("PSRAM MR0 SET");
        // return APS256XX_ERROR;
    }

    MODIFY_REG(reg[0], (uint8_t)(APS256XX_MR4_WRITE_LATENCY_CODE | APS256XX_MR4_RF_RATE | APS256XX_MR4_PASR),
               ((uint8_t)APS256XX_MR4_WLC_7 | APS256XX_MR4_RF_1X | APS256XX_MR4_PASR_FULL));
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR4_ADDRESS, reg[0]))
    {
        LOG_D("PSRAM MR4 SET");
        // return APS256XX_ERROR;
    }

    MODIFY_REG(reg[0], ((uint8_t)APS256XX_MR8_X8_X16 | (uint8_t)APS256XX_MR8_RBX | (uint8_t)APS256XX_MR8_BT | (uint8_t)APS256XX_MR8_BL),
               ((uint8_t)APS256XX_MR8_HYBRID_BURST_WRAP | (uint8_t)APS256XX_MR8_X16 | (uint8_t)APS256XX_MR8_BL_32_BYTES));
    if (APS256XX_OK == APS256XX_WriteReg(&hxspi1, APS256XX_MR8_ADDRESS, reg[0]))
    {
        LOG_D("PSRAM MR8 SET");
        // return APS256XX_ERROR;
    }

    HAL_XSPI_GetDelayValue(&hxspi1, &cal);
    LOG_D("cal delay: 0x%.2x, fine: 0x%.2x, coarse 0x%.2x, max 0x%.2x", cal.DelayValueType, cal.FineCalibrationUnit, cal.CoarseCalibrationUnit, cal.MaxCalibration);

    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR0_ADDRESS, reg, 7))
    {
        LOG_D("PSRAM MR0: 0x%.2x, MR1: 0x%.2x", reg[0], reg[1]);
    }
    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR4_ADDRESS, reg, 7))
    {
        LOG_D("PSRAM MR4: 0x%.2x, MR5: 0x%.2x", reg[0], reg[1]);
    }
    if (APS256XX_OK == APS256XX_ReadReg(&hxspi1, APS256XX_MR8_ADDRESS, reg, 7))
    {
        LOG_D("PSRAM MR8: 0x%.2x, MR1: 0x%.2x", reg[0], reg[1]);
    }

    HAL_XSPI_GetDelayValue(&hxspi1, &cal);
    LOG_D("cal delay: 0x%.2x, fine: 0x%.2x, coarse 0x%.2x, max 0x%.2x", cal.DelayValueType, cal.FineCalibrationUnit, cal.CoarseCalibrationUnit, cal.MaxCalibration);
    /*Configure Memory Mapped mode*/
    if (APS256XX_OK != APS256XX_EnableMemoryMappedMode(&hxspi1, ReadLatencyCode, WriteLatencyCode, HAL_XSPI_DATA_16_LINES, 0)) // Liner Burst
    {
        LOG_E("PSRAM Enter XIP Failed");
    }

    rt_kprintf("\nJump to APP...\n");
    rt_thread_mdelay(300);
    //    while (1)
    //    {
    //        rt_pin_write(LED_RED, PIN_HIGH);
    //        rt_thread_mdelay(500);
    //        rt_pin_write(LED_RED, PIN_LOW);
    //        rt_thread_mdelay(500);
    //    }

    JumpToApplication();

    return RT_EOK;
}
