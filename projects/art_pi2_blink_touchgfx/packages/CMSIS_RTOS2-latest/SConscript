from building import *

src	= Glob('*.c')
cwd = GetCurrentDir()

if GetDepend('RT_USING_UTEST'):
    src += ['testcases/thread_tc.c']

path = [cwd]
group = DefineGroup('CMSIS-RTOS2', src, depend = ['PKG_USING_CMSIS_RTOS2'], CPPPATH = path)
Return('group')
