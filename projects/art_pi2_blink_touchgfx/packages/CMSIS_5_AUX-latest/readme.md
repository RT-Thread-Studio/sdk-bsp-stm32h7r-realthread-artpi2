# CMSIS-5 auxiliary package

本软件包为[官方CMSIS_5软件包](https://github.com/ARM-software/CMSIS_5)的辅助软件包，本软件包对用户不可见，也不需要用户进行选定，在用户选择使用CMSIS_5软件包时，会自动选定本软件包。其原因是，CMSIS_5官方仓库不允许我们提交Sconscript脚本，但是为了能让RT-Thread小伙伴使用到最新的CMSIS代码，采用了一种新的软件包同步方案：

1） 将官方CMSIS-5仓库注册为RT-Thread软件包，但是由于该仓库内没有Sconscript脚本，无法构建功能；

2） 因此构建工程的任务交给了CMSIS_5_AUX辅助软件包，该软件包的Sconscript脚本实际上就是CMSIS_5缺的那个Sconscript脚本，在运行时，CMSIS_5_AUX的Sconscript会将目录向上退一级并进入到CMSIS_5软件包文件目录内，对CMSIS_5的源码进行组织。这样就做到了即便CMSIS_5仓库内即便没有RT-Thread的Sconscript脚本，我们也可以使用最新的官方代码了；

3） 对于使用者来讲，CMSIS_5_AUX软件包是不可见的，在选定使用CMSIS_5软件包时，系统会自动也选定CMSIS_5_AUX软件包。

仓库地址：https://github.com/RT-Thread-packages/CMSIS_5_AUX
