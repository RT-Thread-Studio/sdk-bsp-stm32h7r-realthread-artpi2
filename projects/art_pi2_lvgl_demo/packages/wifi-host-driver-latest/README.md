## RT-Thread Wi-Fi Host Driver (WHD)

[中文](./README_CN.md) | English

### Overview
The WHD is an independent, embedded Wi-Fi Host Driver that provides a set of APIs to interact with Infineon WLAN chips. The WHD is an independent firmware product that is easily portable to any embedded software environment, including popular IoT frameworks such as Mbed OS and Amazon FreeRTOS. Therefore, the WHD includes hooks for RTOS and TCP/IP network abstraction layers.

Details about Wi-Fi Host Driver can be found in the [Wi-Fi Host Driver readme](./wifi-host-driver/README.md).<br>
The [release notes](./wifi-host-driver/RELEASE.md) detail the current release. You can also find information about previous versions.

The repository has adapted WHD to the RT-Thread system, currently only supports the SDIO bus protocol, and uses the mmcsd of RT-Thread for sdio bus operations.<br>
Welcome everyone `PR` to support more bus interface and chips.

### Using
**In the package, select `Wifi-Host-Driver(WHD) for RT-Thread`**
```
RT-Thread online packages  --->                         # Online software package
    IoT - internet of things  --->                      # IOT menu bar
        [*] Wifi-Host-Driver(WHD) for RT-Thread.  --->  # Select the package
```

### Package configuration
```
--- Wifi-Host-Driver(WHD) for RT-Thread
      Select Chips (CYW43438)  --->                     # Select the corresponding chip
[*]   Using resources in external storage  --->         # Using the FS/FAL to load the resource
[ ]   Using custom nvram files                          # Use custom nvram header files
[ ]   Default enable powersave mode                     # The low power mode is selected by default
(8)   The priority level value of WHD thread            # Configure the priority of the WHD thread
(5120) The stack size for WHD thread                    # Configure the stack size of the WHD thread
      Select the pin name or number (Number)  --->      # Select a pin name or pin number
(-1)  Set the WiFi_REG ON pin number                    # Set the WiFi_REG ON pin of the module
(-1)  Set the HOST_WAKE_IRQ pin number                  # Set the HOST_WAKE_IRQ pin of the module
      Select HOST_WAKE_IRQ event type (falling)  --->   # Select the edge of Wake up host
(2)   Set the interrput priority for HOST_WAKE_IRQ pin  # Set the external interrupt priority
[ ]   Using thread initialization                       # Create a thread to initialize the driver
```

- When using the external storage to load a resource file, file system or FAL component is automatically selected, user must transfer the `firmware` and `clm` files used by the WiFi module to the corresponding path or FAL partition.
- For faster startup, a function that can be used to wait for the file system to mount (whd_wait_fs_mount) is provided, user can rewrite the weak function to wait for the file system to mount the semaphore before loading the resource file.
- When using a custom `nvram` file, users need to write their own `wifi_nvram_image.h` file and include the header file path, refer to the [wifi_nvram_image.h](./wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43012/COMPONENT_CYSBSYS-RP01/wifi_nvram_image.h) file that comes with `WHD`.
- When using the default enable powersave mode, the module will enter the energy saving mode when idle, which will cause the `SDIO` driver to print a timeout log when the module is woken up. This is a normal phenomenon.

**Note**<br>
sdio driver needs to support stream transfer. In the bsp of RT-Thread, most chips do not have the function of adapting stream transfer. <br>
The `Cortex-M4` core also requires software to compute `CRC16` and send it after the data, reference [stream transmission solution](./docs/SDIO数据流传输.md).<br>
For the `Cortex-M7` core, modify the "drv_sdio.c" file as shown in the following example: <br>
```c
/* The example is an sdio driver for the STM32H750 */
SCB_CleanInvalidateDCache();

reg_cmd |= SDMMC_CMD_CMDTRANS;
hw_sdio->mask &= ~(SDMMC_MASK_CMDRENDIE | SDMMC_MASK_CMDSENTIE);
hw_sdio->dtimer = HW_SDIO_DATATIMEOUT;
hw_sdio->dlen = data->blks * data->blksize;
hw_sdio->dctrl = (get_order(data->blksize)<<4) |
                    (data->flags & DATA_DIR_READ ? SDMMC_DCTRL_DTDIR : 0) | \
                    /* Adds detection of the DATA_STREAM flag */
                    ((data->flags & DATA_STREAM) ? SDMMC_DCTRL_DTMODE_0 : 0);
hw_sdio->idmabase0r = (rt_uint32_t)sdio->cache_buf;
hw_sdio->idmatrlr = SDMMC_IDMA_IDMAEN;
```

### Resource download
You can download resource files in ymodem mode. The resource files use the FAL component.<br>
The resource download function depends on the ymodem components.<br>
Make sure that `RT_USING_RYM` and `WHD_RESOURCES_IN_EXTERNAL_STORAGE` definitions are turned on.
- Run the "whd_res_download" command on the terminal to download the resources.
- This command requires you to enter the partition name of the resource file.
- For example resource download(Use the default partition name, Enter your own partition name):
```shell
# For example, my partition configuration
/* partition table */
/*      magic_word          partition name      flash name          offset          size            reserved        */
#define FAL_PART_TABLE                                                                                              \
{                                                                                                                   \
    { FAL_PART_MAGIC_WORD,  "whd_firmware",     "onchip_flash",     0,              448 * 1024,         0 },        \
    { FAL_PART_MAGIC_WORD,  "whd_clm",          "onchip_flash",     448 * 1024,     32 * 1024,          0 },        \
    { FAL_PART_MAGIC_WORD,  "easyflash",        "onchip_flash",     480 * 1024,     32 * 1024,          0 },        \
    { FAL_PART_MAGIC_WORD,  "filesystem",       "onchip_flash",     512 * 1024,     512 * 1024,         0 },        \
}

# Download firmware files
whd_res_download whd_firmware

# Download clm files
whd_res_download whd_clm
```
- The ymodem tool can use xshell, after completing the command input, wait for xshell to initiate the file transfer.
```
msh >whd_res_download whd_firmware
Please select the whd_firmware file and use Ymodem to send.
```
- At this point, right-click the mouse in xshell and select "file transfer" to "using ymodem send".
- In the `resources(wifi-host-driver/WiFi_Host_Driver/resources)` directory of `whd`, select the resource file for the corresponding chip.
- After the transmission is complete, msh will output the following log.
```
Download whd_firmware to flash success. file size: 419799
```
- After downloading the firmware and clm resource files, reset and restart.

#### Resource file verification function (Recommended)
- In the package, select `TinyCrypt: A tiny and configurable crypt library`
```
RT-Thread online packages  --->                                   # Online software package
    security packages  --->                                       # In the security package
        TinyCrypt: A tiny and configurable crypt library  --->    # Select TinyCrypt
```
- Select the `TinyCrypt` package, after the file is downloaded complete, the md5 checksum is calculated and printed automatically.

### Supported Chip

| **CHIP**  |**SDIO**|**SPI**|**M2M**|
|-----------|--------|-------|-------|
| CYW4343W  |   *    |   x   |   x   |
| CYW43438  |   o    |   x   |   x   |
| CYW4373   |   *    |   x   |   x   |
| CYW43012  |   o    |   x   |   x   |
| CYW43439  |   o    |   x   |   x   |
| CYW43022  |   *    |   x   |   x   |

'x' indicates no support<br>
'o' indicates tested and supported<br>
'*' means theoretically supported, but not tested

### More information
* [Wi-Fi Host Driver API Reference Manual and Porting Guide](https://infineon.github.io/wifi-host-driver/html/index.html)
* [Wi-Fi Host Driver Release Notes](./wifi-host-driver/RELEASE.md)
* [Infineon Technologies](http://www.infineon.com)

### Contact & Suppor
- mail: 1425295900@qq.com
- WeChat: Evlers
- If there is any problem on the driver, welcome to submit PR or contact me to communicate with you.
- If you think this project is good and can meet your needs, please tip me~ Thank you for your support!<br>
![wechat](./docs/images/qrcode.png)
