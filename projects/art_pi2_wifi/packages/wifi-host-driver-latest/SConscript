Import('RTT_ROOT')
Import('rtconfig')
from building import *

cwd = GetCurrentDir()

# add the general drivers.
src = Split("""
    wifi-host-driver/WiFi_Host_Driver/src/bus_protocols/whd_bus_common.c
    wifi-host-driver/WiFi_Host_Driver/src/bus_protocols/whd_bus_sdio_protocol.c
    wifi-host-driver/WiFi_Host_Driver/src/bus_protocols/whd_bus.c
""")


# add wifi host driver source and header files
src += Glob('wifi-host-driver/WiFi_Host_Driver/src/*.c')

path = [cwd + '/wifi-host-driver']
path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/src']
path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/inc']
path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/src/include']
path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/src/bus_protocols']
path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/resource_imp']

# Add wifi local resource files
if GetDepend(['WHD_RESOURCES_IN_EXTERNAL_STORAGE']) == False:
    src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/resource_imp/*.c')
    # add cyw4343W chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW4343W']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_4343W/4343WA1_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_4343W/4343WA1_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_4343W']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_4343W']

    # add cyw43438 chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW43438']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43438/43438A1_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43438/43438A1_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43438']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43438']

    # add cyw4373 chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW4373']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_4373/COMPONENT_MURATA-2AE/4373A0_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_4373/4373A0_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_4373/COMPONENT_MURATA-2AE']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_4373']

    # add cyw43012 chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW43012']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43012/43012C0_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43012/43012C0_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43012']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43012']

    # add cyw43439 chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW43439']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43439/43439A0_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43439/43439a0_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43439']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43439']

    # add cyw43022 chip drivers source and header files
    if GetDepend(['WHD_USING_CHIP_CYW43022']):
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43022/43022C1_clm_blob.c')
        src += Glob('wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43022/COMPONENT_SM/43022C1_bin.c')
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/clm/COMPONENT_43022']
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/firmware/COMPONENT_43022/COMPONENT_SM']

if GetDepend(['WHD_RESOURCES_CUSTOM_NVRAM']) == False:
    # add cyw4343W chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW4343W']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_4343W/COMPONENT_MURATA-1DX']

    # add cyw43438 chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW43438']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43438/COMPONENT_AW-CU427-P']

    # add cyw4373 chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW4373']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_4373/COMPONENT_MURATA-2AE']

    # add cyw43012 chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW43012']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43012/COMPONENT_CYSBSYS-RP01']

    # add cyw43439 chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW43439']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43439/COMPONENT_AZW-IFW56810']

    # add cyw43022 chip drivers nvram files
    if GetDepend(['WHD_USING_CHIP_CYW43022']):
        path += [cwd + '/wifi-host-driver/WiFi_Host_Driver/resources/nvram/COMPONENT_43022/COMPONENT_CYW43022CUB']

# add whd porting source files
src += Glob('porting/src/*/*.c')

path += [cwd + '/porting/inc/bsp']
path += [cwd + '/porting/inc/hal']
path += [cwd + '/porting/inc/rtos']


CPPDEFINES = ['']

group = DefineGroup('whd', src, depend = ['PKG_USING_WIFI_HOST_DRIVER'], CPPPATH = path, CPPDEFINES = CPPDEFINES)

Return('group')
