# TouchGFX例程

## 简介

本例程主要功能是实现了对ST官方的图形库-TouchGFX的移植；

## 硬件说明

需要使用RGB屏幕和ARTPI的屏幕接口连接

## 软件说明

[touchgfx_lib](../../libraries/touchgfx_lib) 目录为 TouchGFX的移植目录；

用户如需自行修改实现自己设计的UI，需要替换 [generated](../../libraries/touchgfx_lib/TouchGFX/generated) 和 [gui](../../libraries/touchgfx_lib/TouchGFX/gui) 目录为自己的文件夹，然后使用env重新生成MDK工程编译即可

## 运行
### 编译&下载

编译完成后，将开发板的 ST-Link USB 口与 PC 机连接，然后将固件下载至开发板。

### 运行效果

正常运行后，屏幕会显示GUI，可以通过触摸进行交互；

注意事项：

目前 TouchGFX 的 Video 功能还不能正常使用，也欢迎有经验的小伙伴参与修复~

