# WiFi例程

## 简介

本例程主要功能是让ARTPI2支持WiFi功能。
这个例程也可以做为您的创作IOT的基础工程。

## 软件说明
- 使用`pkgs --upgrade` 命令更新在线软件包配置
- 如果想 `使用SD卡加载资源文件`，，需要将`wifi-host-driver`软件包中以下`firmware`和`clm`文件放到SD卡的根目录:

```
packages/wifi-host-driver-latest/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43438/43438A1.clm_blob
packages/wifi-host-driver-latest/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43438/43438A1.bin
```

## 运行
### 编译&下载
- 使用`pkgs --update`命令更新软件包(下载`wifi-host-driver`和`netutils`软件包)
- 使用`scons -j24`或者`scons --target=mdk4/mdk5/iar` 命令生产`IDE`工程进行编译
- 编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果
```
 \ | /
- RT -     Thread Operating System
 / | \     5.1.0 build Oct 30 2024 15:31:59
 2006 - 2024 Copyright by RT-Thread team
lwIP-2.0.3 initialized!
[I/sal.skt] Socket Abstraction Layer initialize success.
msh />[I/SDIO] SD card capacity 31166976 KB.
found part[0], begin: 16384, size: 29.740GB
[I/app.filesystem] sd card mount to '/sdcard'
RT-Thread WiFi Host Drivers (WHD)
You can get the latest version on https://github.com/Evlers/rt-thread_wifi-host-driver
WLAN MAC Address : 2C:B0:FD:A2:26:76
WLAN Firmware    : wl0: Mar 28 2021 22:55:55 version 7.45.98.117 (dc5d9c4 CY) FWID 01-d36e8386
WLAN CLM         : API: 12.2 Data: 9.10.39 Compiler: 1.29.4 ClmImport: 1.36.3 Creation: 2021-03-28 22:47:33
WHD VERSION      : 3.1.0.23284 : v3.1.0 : ARM CLANG 5060960 : 2024-03-21 22:57:11 +0800
[I/WLAN.dev] wlan init success
[I/WLAN.lwip] eth device init ok name:w0
[I/WLAN.dev] wlan init success
[I/WLAN.lwip] eth device init ok name:w1
msh />wifi join TP-LINK_86A9 xxxxxxxx
[I/WLAN.mgnt] wifi connect success ssid:TP-LINK_86A9
msh />[I/WLAN.lwip] Got IP address : 192.168.0.101
msh />iperf -c 192.168.0.111
msh />[I/iperf] Connect to iperf server successful!
[I/iperf] iperfc0: 13.5890 Mbps!
[I/iperf] iperfc0: 13.2310 Mbps!
[I/iperf] iperfc0: 13.2060 Mbps!
[I/iperf] iperfc0: 13.5260 Mbps!
[I/iperf] iperfc0: 13.6960 Mbps!
[I/iperf] iperfc0: 13.7920 Mbps!
[I/iperf] iperfc0: 13.8540 Mbps!
```

## 注意事项

确保使用`pkgs --update`命令更新软件包，并完成了`wifi-host-driver-latest`软件包的下拉
编译优化等级可以调高，例如`-O3`，可提高`iperf`测试速度。
