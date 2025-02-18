# BootLoader 例程

## 简介

本例程主要的功能是让程序从 `0x08000000`跳转到`0x70000000`。
STM32H7R 的片上 ROM 大小为128K，无法满足一般的程序开发，必须使用 XIP 的方式运行程序。所以需要使用 BootLoader 来跳转到 XIP(0x70000000) 地址。
这个例程也可以作为您开发更多高级 bootloader 功能的基础工程。

## 软件说明

源代码位于 `/projects/art_pi2_bootloader/applications/main.c` 中。

在 main 函数中，实现了跳转。

```c
int JumpToApplication(void)
{
    uint32_t primask_bit;
    typedef void (*pFunction)(void);
    pFunction JumpToApp;
    uint32_t Application_vector;

    /* Suspend SysTick */
    HAL_SuspendTick();

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

    JumpToApp();
    return 0;
}
```

## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

上电之后会执行 bootloader 程序之后就会跳转执行 HyprFlash 中的可执行程序

## 注意事项

1. 如果 HyprFlash  中没有可执行程序，那么 MCU 在执行跳转指令后就会停止运行。
