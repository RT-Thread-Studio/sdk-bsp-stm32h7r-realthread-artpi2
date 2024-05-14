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
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* defined the LED0 pin: PB1 */
#define LED_RED    GET_PIN(O, 5)
#define LED_BLUE    GET_PIN(O, 1)
#define LCD_BACKLIGHT    GET_PIN(G, 15)

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x70000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;

XSPI_HandleTypeDef hxspi1;
XSPI_HandleTypeDef hxspi2;
ADC_HandleTypeDef hadc1;
/**
  * @brief XSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_XSPI1_Init(void)
{

  /* USER CODE BEGIN XSPI1_Init 0 */
  uint32_t xspi1_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_XSPI1);
  /* USER CODE END XSPI1_Init 0 */

  XSPIM_CfgTypeDef sXspiManagerCfg = {0};

  /* USER CODE BEGIN XSPI1_Init 1 */

  /* USER CODE END XSPI1_Init 1 */
  /* XSPI1 parameter configuration*/
  hxspi1.Instance = XSPI1;
  hxspi1.Init.FifoThresholdByte = 2U;
  hxspi1.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
  hxspi1.Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM_16BITS;
  hxspi1.Init.MemorySize = HAL_XSPI_SIZE_32GB;
  hxspi1.Init.ChipSelectHighTimeCycle = 1U;
  hxspi1.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi1.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
  hxspi1.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi1.Init.ClockPrescaler = 0;
  hxspi1.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
  hxspi1.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
  hxspi1.Init.ChipSelectBoundary = HAL_XSPI_BONDARYOF_8KB;
  hxspi1.Init.MaxTran = 0;
  hxspi1.Init.Refresh = ((2U * (xspi1_clk / (hxspi1.Init.ClockPrescaler + 1U))) / 1000000U) - 4U;;
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
  hxspi2.Init.MemorySize = HAL_XSPI_SIZE_1GB;
  hxspi2.Init.ChipSelectHighTimeCycle = 1;
  hxspi2.Init.FreeRunningClock = HAL_XSPI_FREERUNCLK_DISABLE;
  hxspi2.Init.ClockMode = HAL_XSPI_CLOCK_MODE_0;
  hxspi2.Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
  hxspi2.Init.ClockPrescaler = 1;
  hxspi2.Init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
  hxspi2.Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSign = ADC_OFFSET_SIGN_NEGATIVE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

  /* Size of buffers */
#define BUFFERSIZE                              10240
#define KByte                                   1024
/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFERSIZE]; 
__IO uint8_t *mem_addr;
void mem_bench()
{
  uint16_t errorBuffer = 0;
  uint32_t index, index_K;
  /*fill aTxBuffer */
  for (index_K = 0; index_K < 10; index_K++)
  {
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      aTxBuffer[index]=index + index_K;
    }
  }
  /* Writing Sequence ----------------------------------------------- */
  index_K=0;
  for (index_K = 0; index_K < 10; index_K++)
  {
    mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      *mem_addr = aTxBuffer[index];
      mem_addr++;
    }
    
    /* In memory-mapped mode, not possible to check if the memory is ready
    after the programming. So a delay corresponding to max page programming
    time is added */
    HAL_Delay(1);
  }
  
  /* Reading Sequence ----------------------------------------------- */
  index_K=0;
  for (index_K = 0; index_K < 2; index_K++)
  {
    mem_addr = (uint8_t *)(XSPI1_BASE + (index_K * KByte));
    for (index = (index_K  * KByte); index < ((index_K +1) * KByte); index++)
    {
      if (*mem_addr != aTxBuffer[index])
      {
        rt_pin_write(LED_RED, PIN_HIGH);
        errorBuffer++;
      }
      mem_addr++;
    }
 
     /* In memory-mapped mode, not possible to check if the memory is ready
    after the programming. So a delay corresponding to max page programming
    time is added */
    HAL_Delay(1);
  }
  if (errorBuffer == 0)
  {
    /* Turn GREEN on */
    rt_pin_write(LED_RED, PIN_LOW);
    rt_kprintf("PSRAM Test OK.\n");
  }
  else
  { 
      rt_kprintf("PSRAM Test fail %s times.\n", errorBuffer);
  }
  
  /* Abort XSPI driver to stop the memory-mapped mode ------------ */
//  if (HAL_XSPI_Abort(&hxspi1) != HAL_OK)
//  {
//    rt_kprintf("PSRAM Abort XSPI Fail..\n");
//    Error_Handler();
//  }
    
    
}

MSH_CMD_EXPORT(mem_bench,mem_bench)

XSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Read Operations */
#define READ_CMD                                0x00
#define READ_LINEAR_BURST_CMD                   0x20
#define READ_HYBRID_BURST_CMD                   0x3F
  
  /* Write Operations */
#define WRITE_CMD                               0x80
#define WRITE_LINEAR_BURST_CMD                  0xA0
#define WRITE_HYBRID_BURST_CMD                  0xBF
  
  /* Reset Operations */
#define RESET_CMD                               0xFF
  /* Registers definition */
#define MR0                                     0x00000000
#define MR1                                     0x00000001
#define MR2                                     0x00000002
#define MR3                                     0x00000003
#define MR4                                     0x00000004
#define MR8                                     0x00000008
  
  /* Register Operations */
#define READ_REG_CMD                            0x40
#define WRITE_REG_CMD                           0xC0

  /* Default dummy clocks cycles */
#define DUMMY_CLOCK_CYCLES_READ                 4
#define DUMMY_CLOCK_CYCLES_WRITE                4
  
  /* Size of buffers */
#define BUFFERSIZE                              10240
#define KByte                                   1024

/**
* @brief  Write mode register
* @param  Ctx Component object pointer
* @param  Address Register address
* @param  Value Register value pointer
* @retval error status
*/
uint32_t APS256_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value)
{
  XSPI_RegularCmdTypeDef sCommand1={0};
  
  /* Initialize the write register command */
  sCommand1.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand1.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  sCommand1.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand1.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand1.Instruction        = WRITE_REG_CMD;
  sCommand1.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  sCommand1.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  sCommand1.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  sCommand1.Address            = Address;
  sCommand1.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand1.DataMode           = HAL_XSPI_DATA_8_LINES;
  sCommand1.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  sCommand1.DataLength         = 2;
  sCommand1.DummyCycles        = 0;
  sCommand1.DQSMode            = HAL_XSPI_DQS_DISABLE;
  
  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &sCommand1, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* Transmission of the data */
  if (HAL_XSPI_Transmit(Ctx, (uint8_t *)(Value), HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK;
}

/**
* @brief  Read mode register value
* @param  Ctx Component object pointer
* @param  Address Register address
* @param  Value Register value pointer
* @param  LatencyCode Latency used for the access
* @retval error status
*/
uint32_t APS256_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode)
{
  XSPI_RegularCmdTypeDef sCommand={0};
  
  /* Initialize the read register command */
  sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
  sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionWidth    = HAL_XSPI_INSTRUCTION_8_BITS;
  sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.Instruction        = READ_REG_CMD;
  sCommand.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  sCommand.AddressWidth        = HAL_XSPI_ADDRESS_32_BITS;
  sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  sCommand.Address            = Address;
  sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  sCommand.DataMode           = HAL_XSPI_DATA_8_LINES;
  sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  sCommand.DataLength            = 2;
  sCommand.DummyCycles        = (LatencyCode - 1U);
  sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE;
  
  /* Configure the command */
  if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  /* Reception of the data */
  if (HAL_XSPI_Receive(Ctx, (uint8_t *)Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK;
}
/**
* @brief  Switch from Octal Mode to Hexa Mode on the memory
* @param  None
* @retval None
*/
static void Configure_APMemory(void)
{
  /* MR0 register for read and write */
  uint8_t regW_MR0[2]={0x27,0x8D}; /* To configure AP memory Latency Type and drive Strength */
  uint8_t regR_MR0[2]={0};
  
  /* MR8 register for read and write */
  uint8_t regW_MR8[2]={0x4B,0x08}; /* To configure AP memory Burst Type */
  uint8_t regR_MR8[2]={0};
  
  /*Read Latency */
  uint8_t latency=6;
  
  /* Configure Read Latency and drive Strength */
  if (APS256_WriteReg(&hxspi1, MR0, regW_MR0) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Check MR0 configuration */
  if (APS256_ReadReg(&hxspi1, MR0, regR_MR0, latency ) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Check MR0 configuration */
  if (regR_MR0 [0] != regW_MR0 [0])
  {
    Error_Handler() ;
  }
  
  /* Configure Burst Length */
  if (APS256_WriteReg(&hxspi1, MR8, regW_MR8) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Check MR8 configuration */
  if (APS256_ReadReg(&hxspi1, MR8, regR_MR8, 6) != HAL_OK)
  {
    Error_Handler();
  }
  
  if (regR_MR8[0] != regW_MR8[0])
  {
    Error_Handler() ;
  }
}

/* Definitions of environment analog values */
  /* Value of analog reference voltage (Vref+), connected to analog voltage   */
  /* supply Vdda (unit: mV).                                                  */
  #define VDDA_APPLI                       (3300UL)

/* Definitions of data related to this example */
  /* Init variable out of expected ADC conversion data range */
  #define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)


const static uint16_t hardware_vol_rank[8] = 
{
  50,   // less than 50mV, fail
  75,   // 50-75 (normally 65mV) HW=0.1, BOM=0.1
  90,
  65535,
  65535,
  65535,
  65535,
  65535,
};

const static uint16_t hardware_ver_rank[8] = 
{
  0x0101, //HW 0.1, BOM 0.1
  0x0201,
  0x0202,
  0x1010,
  0x1011,
  0xFFFF,
  0xFFFF,
  0xFFFF,
};

int mJumpToApplication(void)
{
  uint32_t primask_bit;
  typedef  void (*pFunction)(void);
  pFunction JumpToApp;
  uint32_t Application_vector;

//  if (EXTMEM_OK != EXTMEM_GetMapAddress(EXTMEM_MEMORY_BOOTXIP, &Application_vector))
//  {
//      return BOOT_ERROR_INCOMPATIBLEMEMORY;
//  }

  /* Suspend SysTick */
  HAL_SuspendTick();

  /* Disable I-Cache---------------------------------------------------------*/
  SCB_DisableICache();

  /* Disable D-Cache---------------------------------------------------------*/
  SCB_DisableDCache();

  /* Initialize user application's Stack Pointer & Jump to user application  */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Apply offsets for image location and vector table offset */
//  Application_vector += EXTMEM_XIP_IMAGE_OFFSET + EXTMEM_HEADER_OFFSET;

  SCB->VTOR = (uint32_t)Application_vector;
  JumpToApp = (pFunction) (*(__IO uint32_t *)(Application_vector + 4u));

#if ((defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1)) || \
     (defined (__ARM_ARCH_8_1M_MAIN__ ) && (__ARM_ARCH_8_1M_MAIN__ == 1)) || \
     (defined (__ARM_ARCH_8M_BASE__ ) && (__ARM_ARCH_8M_BASE__ == 1))    )
  /* on ARM v8m, set MSPLIM before setting MSP to avoid unwanted stack overflow faults */
  __set_MSPLIM(0x00000000);
#endif  /* __ARM_ARCH_8M_MAIN__ or __ARM_ARCH_8M_BASE__ */

  __set_MSP(*(__IO uint32_t*) Application_vector);

  /* Re-enable the interrupts */
  __set_PRIMASK(primask_bit);

  JumpToApp();
  return 0;
}

int main(void)
{
    uint8_t device_id[6];
    XSPI_RegularCmdTypeDef sCommand = {0};
    int retr=0;
    
    {
        /* Variables for ADC conversion data */
        __IO uint16_t uhADCxConvertedData = VAR_CONVERTED_DATA_INIT_VALUE; /* ADC group regular conversion data */

        /* Variables for ADC conversion data computation to physical values */
        uint16_t uhADCxConvertedData_Voltage_mVolt = 0;  /* Value of voltage calculated from ADC conversion data (unit: mV) */
        uint16_t hardware_ver = 0;
        /* Initialize ADC1 configured peripherals */
        MX_ADC1_Init();

        /* Perform ADC calibration */
        if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
        {
          /* Calibration Error */
          Error_Handler();
        }
        /* Start ADC group regular conversion */
        if (HAL_ADC_Start(&hadc1) != HAL_OK)
        {
          /* ADC conversion start error */
          Error_Handler();
        }
        /* Wait for ADC conversion completed */
        if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
        {
          /* End Of Conversion flag not set on time */
          Error_Handler();
        }

        /* Retrieve ADC conversion data */
        uhADCxConvertedData = HAL_ADC_GetValue(&hadc1);

        /* Computation of ADC conversions raw data to physical values             */
        /* using helper macro.                                                    */
        uhADCxConvertedData_Voltage_mVolt = __LL_ADC_CALC_DATA_TO_VOLTAGE(VDDA_APPLI, uhADCxConvertedData, LL_ADC_RESOLUTION_12B);

        if (uhADCxConvertedData_Voltage_mVolt < hardware_vol_rank[0])
        {
          LOG_E("no hardware version detected on GPIO PA3");
        }
        else
        {
          for (uint8_t i = 0; i < sizeof(hardware_vol_rank)/sizeof(uint16_t) - 1; i++)
          {
            if ((uhADCxConvertedData_Voltage_mVolt >= hardware_vol_rank[i]) &&  (uhADCxConvertedData_Voltage_mVolt <= hardware_vol_rank[i+1]))
            {
                hardware_ver = hardware_ver_rank[i];
                LOG_I("Hardware Version: %d.%d, BoM Version: %d.%d\n", ((hardware_ver&0xF000)>>12), ((hardware_ver&0x0F00)>>8), ((hardware_ver&0x00F0)>>4), (hardware_ver&0x000F) );
                break;
            }
            
          }
          
        }
        

        rt_kprintf("ADC read %d, voltage %d\n",uhADCxConvertedData, uhADCxConvertedData_Voltage_mVolt);

        // ToDO: deinit ADC
    }
    /* set LED0 pin mode to output */
    rt_pin_mode(LED_RED, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_BACKLIGHT, PIN_MODE_OUTPUT);
    MX_XSPI1_Init();
    MX_XSPI2_Init();

    if(W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2, 
                                            W35T51NWTBIE_SPI_MODE, 
                                            W35T51NWTBIE_STR_TRANSFER, 
                                            device_id , W35T51NWTBIE_3BYTES_SIZE))
    
    {
        LOG_E("Read ID Fail");
    }
    else
    {
        LOG_I("Read ID success:%.2x %.2x %.2x", device_id[0], device_id[1], device_id[2]);
        retr =  W35T51NWTBIE_EnterOctal_DTR_Mode(&hxspi2);
        LOG_D("Enter octal:%d", retr);
        if(W35T51NWTBIE_OK != W35T51NWTBIE_ReadID(&hxspi2, 
                                                 W35T51NWTBIE_OPI_MODE, 
                                                 W35T51NWTBIE_DTR_TRANSFER, 
                                               device_id , W35T51NWTBIE_4BYTES_SIZE))
        {
            LOG_E("Enter octal failed");

        }
        else
        {
            LOG_I("Enter Octal DTR and Read ID success:%.2x %.2x %.2x", device_id[0], device_id[1], device_id[2]);
        }
    }

//    MX66UW_Init();
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
//#ifndef FIRMWARE_EXEC_USING_QEMU
//    W25QXX_Init();
//    W25Q_Memory_Mapped_Enable();
//#endif

  //   Configure_APMemory();
  // /*Configure Memory Mapped mode*/
  
  //   sCommand.OperationType      = HAL_XSPI_OPTYPE_WRITE_CFG;
  //   sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
  //   sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
  //   sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
  //   sCommand.Instruction        = WRITE_CMD;
  //   sCommand.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
  //   sCommand.AddressWidth       = HAL_XSPI_ADDRESS_32_BITS;
  //   sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
  //   sCommand.Address            = 0x0;
  //   sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
  //   sCommand.DataMode           = HAL_XSPI_DATA_16_LINES;
  //   sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
  //   sCommand.DataLength         = BUFFERSIZE;
  //   sCommand.DummyCycles        = DUMMY_CLOCK_CYCLES_WRITE;
  //   sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE;

  //   if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  //   {
  //   Error_Handler();
  //   }

  //   sCommand.OperationType = HAL_XSPI_OPTYPE_READ_CFG;
  //   sCommand.Instruction = READ_CMD;
  //   sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ;
  //   sCommand.DQSMode     = HAL_XSPI_DQS_ENABLE;

  //   if (HAL_XSPI_Command(&hxspi1, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  //   {
  //   Error_Handler();
  //   }

  //   sMemMappedCfg.TimeOutActivation = HAL_XSPI_TIMEOUT_COUNTER_ENABLE;
  //   sMemMappedCfg.TimeoutPeriodClock      = 0x34;


  //   if (HAL_XSPI_MemoryMapped(&hxspi1, &sMemMappedCfg) != HAL_OK)
  //   {
  //   Error_Handler();
  //   }

extern int32_t HSPI_RAM_XIP(void);

    HSPI_RAM_XIP();
    
//    SCB_DisableICache();
//    SCB_DisableDCache();
    rt_kprintf("\nJump to APP...\n");
    rt_thread_mdelay(300);
//    while (1)
//    {
//        rt_pin_write(LED_RED, PIN_HIGH);
//        rt_thread_mdelay(500);
//        rt_pin_write(LED_RED, PIN_LOW);
//        rt_thread_mdelay(500);
//    }

    SysTick->CTRL = 0;

    JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

    JumpToApplication();

    return RT_EOK;
}
