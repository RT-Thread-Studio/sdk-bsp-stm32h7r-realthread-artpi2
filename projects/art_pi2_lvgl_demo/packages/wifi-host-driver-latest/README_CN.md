## RT-Thread Wi-Fi Host Driver (WHD)

中文 | [English](./README.md)

### 概述
WHD是一个独立的嵌入式Wi-Fi主机驱动程序，它提供了一组与英飞凌WLAN芯片交互的api。WHD是一个独立的固件产品，可以很容易地移植到任何嵌入式软件环境，包括流行的物联网框架，如Mbed OS和Amazon FreeRTOS。因此，WHD包含了RTOS和TCP/IP网络抽象层的钩子。

有关Wi-Fi主机驱动程序的详细信息可在[Wi-Fi Host Driver readme](./wifi-host-driver/README.md)文件中找到。<br>
[release notes](./wifi-host-driver/RELEASE.md)详细说明了当前的版本。您还可以找到有关以前版本的信息。

该存储库已将WHD适应于RT-Thread系统，目前仅支持SDIO总线协议，并使用RT-Thread的mmcsd进行SDIO总线操作。<br>
欢迎大家`PR`支持更多总线接口和芯片。

### 使用

**在软件包选中`Wifi-Host-Driver(WHD) for RT-Thread`**
```
RT-Thread online packages  --->                         # 在线软件包
    IoT - internet of things  --->                      # IOT菜单栏中
        [*] Wifi-Host-Driver(WHD) for RT-Thread.  --->  # 选中该软件包
```

**软件包配置**
```
--- Wifi-Host-Driver(WHD) for RT-Thread
      Select Chips (CYW43438)  --->                     # 选择相应的芯片
[*]   Using resources in external storage  --->         # 使用文件系统或FAL组件加载资源
[ ]   Using custom nvram files                          # 使用自定义nvram头文件
[ ]   Default enable powersave mode                     # 默认启用低功耗模式
(8)   The priority level value of WHD thread            # 配置WHD线程的优先级
(5120) The stack size for WHD thread                    # 配置WHD线程的堆栈大小
      Select the pin name or number (Number)  --->      # 选择引脚名称或引脚编号
(-1)  Set the WiFi_REG ON pin number                    # 设置模块的WL_REG_ON引脚
(-1)  Set the HOST_WAKE_IRQ pin number                  # 设置模块的HOST_WAKE_IRQ引脚
      Select HOST_WAKE_IRQ event type (falling)  --->   # 选择“唤醒主机”的边沿
(2)   Set the interrput priority for HOST_WAKE_IRQ pin  # 设置外部中断优先级
[ ]   Using thread initialization                       # 创建一个线程来初始化驱动
```

- 使用外部储存加载资源文件时，文件系统或者FAL组件会自动选中，用户必须将WiFi模组用到的`firmware`和`clm`文件传输到对应路径或`FAL`分区。
- 为了更快地启动速度，提供了一个可以用于等待文件系统装载的函数(whd_wait_fs_mount)，用户可重写该弱函数等待文件系统装载的信号量后再加载资源文件。
- 使用自定义`nvram`文件时，用户需要自行编写`wifi_nvram_image.h`文件，并包含该头文件路径，可参考`WHD`自带的[wifi_nvram_image.h](./wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43012/COMPONENT_CYSBSYS-RP01/wifi_nvram_image.h)文件。
- 默认启用低功耗模式时，模组会在空闲的时候进入节能模式，这会导致唤醒模组时`SDIO`驱动打印超时的日志，这是属于正常现象。

**注意**<br>
SDIO驱动需要支持数据流传输，在RT-Thread的bsp中，大多数芯片都未适配数据流传输的功能。<br>
`Cortex-M4`内核需要软件来计算`CRC16`并在数据后面发送它，参考 [数据流传输解决方案](./docs/SDIO数据流传输.md)。<br>
对于`Cortex-M7`内核，只需要修改`drv_sdio.c`文件的一处地方即可，示例如下: <br>
```c
/* 该示例是STM32H750的SDIO驱动程序 */
SCB_CleanInvalidateDCache();

reg_cmd |= SDMMC_CMD_CMDTRANS;
hw_sdio->mask &= ~(SDMMC_MASK_CMDRENDIE | SDMMC_MASK_CMDSENTIE);
hw_sdio->dtimer = HW_SDIO_DATATIMEOUT;
hw_sdio->dlen = data->blks * data->blksize;
hw_sdio->dctrl = (get_order(data->blksize)<<4) |
                    (data->flags & DATA_DIR_READ ? SDMMC_DCTRL_DTDIR : 0) | \
                    /* 增加数据流标志的检测 */
                    ((data->flags & DATA_STREAM) ? SDMMC_DCTRL_DTMODE_0 : 0);
hw_sdio->idmabase0r = (rt_uint32_t)sdio->cache_buf;
hw_sdio->idmatrlr = SDMMC_IDMA_IDMAEN;
```

### 资源下载
可以通过`ymodem`协议下载资源文件。驱动会使用FAL组件来加载资源文件。<br>
资源下载功能依赖于`ymodem`组件，请确保打开`RT_USING_RYM`和`WHD_RESOURCES_IN_EXTERNAL_STORAGE`宏定义。<br>
- 在终端上执行`whd_res_download`命令开始下载资源。
- 该命令需要输入资源文件的分区名。
- 下载资源文件的实例(使用默认分区名，输入自己的分区名):
```shell
# 例如我的分区配置
/* partition table */
/*      magic_word          partition name      flash name          offset          size            reserved        */
#define FAL_PART_TABLE                                                                                              \
{                                                                                                                   \
    { FAL_PART_MAGIC_WORD,  "whd_firmware",     "onchip_flash",     0,              448 * 1024,         0 },        \
    { FAL_PART_MAGIC_WORD,  "whd_clm",          "onchip_flash",     448 * 1024,     32 * 1024,          0 },        \
    { FAL_PART_MAGIC_WORD,  "easyflash",        "onchip_flash",     480 * 1024,     32 * 1024,          0 },        \
    { FAL_PART_MAGIC_WORD,  "filesystem",       "onchip_flash",     512 * 1024,     512 * 1024,         0 },        \
}

# 下载固件文件
whd_res_download whd_firmware

# 下载clm文件
whd_res_download whd_clm
```
- `ymodem`可以使用`xshell`工具，在完成命令输入后，等待`xshell`启动文件传输。
```
msh >whd_res_download whd_firmware
Please select the whd_firmware file and use Ymodem to send.
```
- 此时，在`xshell`中右键单击鼠标，选择`文件传输`到`使用ymodem发送`。
- 在`whd`的`resources(wifi-host-driver/WiFi_Host_Driver/resources)`目录下，选择对应芯片的资源文件。
- 传输完成后，msh将输出如下日志：
```
Download whd_firmware to flash success. file size: 419799
```
- 下载完固件和clm资源文件后，复位重启即可正常加载资源文件。

#### 资源文件的校验功能（建议打开）
- 在软件包选中`TinyCrypt: A tiny and configurable crypt library`
```
RT-Thread online packages  --->                                   # 在线软件包
    security packages  --->                                       # 安全软件包中
        TinyCrypt: A tiny and configurable crypt library  --->    # 选中TinyCrypt
```
- 打开`TinyCrypt`软件包后，资源文件下载完成会自动计算并打印文件的`MD5`校验和。

### 芯片支持

| **CHIP**  |**SDIO**|**SPI**|**M2M**|
|-----------|--------|-------|-------|
| CYW4343W  |   *    |   x   |   x   |
| CYW43438  |   o    |   x   |   x   |
| CYW4373   |   *    |   x   |   x   |
| CYW43012  |   o    |   x   |   x   |
| CYW43439  |   o    |   x   |   x   |
| CYW43022  |   *    |   x   |   x   |

'x' 表示不支持<br>
'o' 表示已测试和支持<br>
'*' 理论上支持，但未经过测试

### 更多信息
* [Wi-Fi Host Driver API Reference Manual and Porting Guide](https://infineon.github.io/wifi-host-driver/html/index.html)
* [Wi-Fi Host Driver Release Notes](./wifi-host-driver/RELEASE.md)
* [Infineon Technologies](http://www.infineon.com)

### 联系方式&支持
- 邮箱: 1425295900@qq.com
- 微信：Evlers
- 如果驱动上有什么问题欢迎提交PR或者联系我一起交流
- 如果你觉得这个项目不错，并且能够满足您的需求，请打赏打赏我吧~ 感谢支持！！<br>
![wechat](./docs/images/qrcode.png)
