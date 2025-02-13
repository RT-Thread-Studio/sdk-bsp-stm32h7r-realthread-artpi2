import os
import shutil
from building import *

group      = []
cwd        = GetCurrentDir()
src        = []
CPPPATH    = []
CPPDEFINES = []

packages_root_path = cwd + '/../'

def find_CMSIS_5_folder(root_path):
    dirs = os.listdir(root_path)
    for dir in dirs:
        if os.path.isdir(packages_root_path + dir):
            if "CMSIS_5-" in dir:
                return dir
    print("Cannot find CMSIS_5's path!")
    return None

cmsis_folder_name = find_CMSIS_5_folder(packages_root_path)
if (cmsis_folder_name == None):
    Return('group') # if we cannot find the CMSIS folder, it will directly return

cmsis_root_path = packages_root_path + cmsis_folder_name
cmsis_path = cmsis_root_path + '/CMSIS/'

if GetDepend('PKG_CMSIS_CORE'):
    CPPPATH += [cmsis_path + 'Core/Include']

if GetDepend('PKG_CMSIS_DSP'):
    CPPPATH += [cmsis_path + 'DSP/Include']
    CPPPATH += [cmsis_path + 'DSP/Include/dsp']
    CPPPATH += [cmsis_path + 'DSP/PrivateInclude']

    if GetDepend('PKG_CMSIS_DSP_BASIC_MATH'):
        dsp_BasicMathFunctions_path = cmsis_path + 'DSP/Source/BasicMathFunctions/'
        src += [dsp_BasicMathFunctions_path + 'BasicMathFunctions.c']
        src += [dsp_BasicMathFunctions_path + 'BasicMathFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_BAYES'):
        dsp_BayesFunctions_path = cmsis_path + 'DSP/Source/BayesFunctions/'
        src += [dsp_BayesFunctions_path + 'BayesFunctions.c']
        src += [dsp_BayesFunctions_path + 'BayesFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_COMMON_TABLES'):
        dsp_CommonTables_path = cmsis_path + 'DSP/Source/CommonTables/'
        src += [dsp_CommonTables_path + 'CommonTables.c']
        src += [dsp_CommonTables_path + 'CommonTablesF16.c']
    if GetDepend('PKG_CMSIS_DSP_COMPLEX_MATH'):
        dsp_ComplexMathFunctions_path = cmsis_path + 'DSP/Source/ComplexMathFunctions/'
        src += [dsp_ComplexMathFunctions_path + 'ComplexMathFunctions.c']
        src += [dsp_ComplexMathFunctions_path + 'ComplexMathFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_CONTROLLER'):
        dsp_ControllerFunctions_path = cmsis_path + 'DSP/Source/ControllerFunctions/'
        src += [dsp_ControllerFunctions_path + 'ControllerFunctions.c']
    if GetDepend('PKG_CMSIS_DSP_DISTANCE'):
        dsp_DistanceFunctions_path = cmsis_path + 'DSP/Source/DistanceFunctions/'
        src += [dsp_DistanceFunctions_path + 'DistanceFunctions.c']
        src += [dsp_DistanceFunctions_path + 'DistanceFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_FAST_MATH'):
        dsp_FastMathFunctions_path = cmsis_path + 'DSP/Source/FastMathFunctions/'
        src += [dsp_FastMathFunctions_path + 'FastMathFunctions.c']
        src += [dsp_FastMathFunctions_path + 'FastMathFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_FILTERING'):
        dsp_FilteringFunctions_path = cmsis_path + 'DSP/Source/FilteringFunctions/'
        src += [dsp_FilteringFunctions_path + 'FilteringFunctions.c']
        src += [dsp_FilteringFunctions_path + 'FilteringFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_INTERPOLATION'):
        dsp_InterpolationFunctions_path = cmsis_path + 'DSP/Source/InterpolationFunctions/'
        src += [dsp_InterpolationFunctions_path + 'InterpolationFunctions.c']
        src += [dsp_InterpolationFunctions_path + 'InterpolationFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_MATRIX'):
        dsp_MatrixFunctions_path = cmsis_path + 'DSP/Source/MatrixFunctions/'
        src += [dsp_MatrixFunctions_path + 'MatrixFunctions.c']
        src += [dsp_MatrixFunctions_path + 'MatrixFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_QUATERNION_MATH'):
        dsp_QuaternionMathFunctions_path = cmsis_path + 'DSP/Source/QuaternionMathFunctions/'
        src += [dsp_QuaternionMathFunctions_path + 'QuaternionMathFunctions.c']
    if GetDepend('PKG_CMSIS_DSP_STATISTICS'):
        dsp_StatisticsFunctions_path = cmsis_path + 'DSP/Source/StatisticsFunctions/'
        src += [dsp_StatisticsFunctions_path + 'StatisticsFunctions.c']
        src += [dsp_StatisticsFunctions_path + 'StatisticsFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_SUPPORT'):
        dsp_SupportFunctions_path = cmsis_path + 'DSP/Source/SupportFunctions/'
        src += [dsp_SupportFunctions_path + 'SupportFunctions.c']
        src += [dsp_SupportFunctions_path + 'SupportFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_SVM'):
        dsp_SVMFunctions_path = cmsis_path + 'DSP/Source/SVMFunctions/'
        src += [dsp_SVMFunctions_path + 'SVMFunctions.c']
        src += [dsp_SVMFunctions_path + 'SVMFunctionsF16.c']
    if GetDepend('PKG_CMSIS_DSP_TRANSFORM'):
        dsp_TransformFunctions_path = cmsis_path + 'DSP/Source/TransformFunctions/'
        src += [dsp_TransformFunctions_path + 'TransformFunctions.c']
        src += [dsp_TransformFunctions_path + 'TransformFunctionsF16.c']

if GetDepend('PKG_CMSIS_NN'):
    CPPPATH += [cmsis_path + 'NN/Include']

    if GetDepend('PKG_CMSIS_NN_ACTIVATION'):
        src += Glob(cmsis_path + 'NN/Source/ActivationFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_BASIC_MATH'):
        src += Glob(cmsis_path + 'NN/Source/BasicMathFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_CONCATENATION'):
        src += Glob(cmsis_path + 'NN/Source/ConcatenationFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_CONVOLUTION'):
        src += Glob(cmsis_path + 'NN/Source/ConvolutionFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_FULLY_CONNECTED'):
        src += Glob(cmsis_path + 'NN/Source/FullyConnectedFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_SUPPORT'):
        src += Glob(cmsis_path + 'NN/Source/NNSupportFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_POOLING'):
        src += Glob(cmsis_path + 'NN/Source/PoolingFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_RESHAPE'):
        src += Glob(cmsis_path + 'NN/Source/ReshapeFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_SVDF'):
        src += Glob(cmsis_path + 'NN/Source/SVDFunctions/*.c')
    if GetDepend('PKG_CMSIS_NN_SOFTMAX'):
        src += Glob(cmsis_path + 'NN/Source/SoftmaxFunctions/*.c')

if GetDepend('PKG_USING_CMSIS_RTOS2'):
    CPPPATH += [cmsis_path + 'RTOS2/Include']

if GetDepend('PKG_CMSIS_RTOS2_COMPATIBLE_CMSIS_RTOS1'):
    CPPPATH = CPPPATH + [cmsis_path + 'RTOS2/Template']
    src += Glob(cmsis_path + 'RTOS2/Template/*.c')

# Definitions for MATH
if GetDepend('ARCH_ARM_CORTEX_M7'):
    CPPDEFINES += ['ARM_MATH_CM7']
elif GetDepend('ARCH_ARM_CORTEX_M4'):
    CPPDEFINES += ['ARM_MATH_CM4']
elif GetDepend('ARCH_ARM_CORTEX_M3'):
    CPPDEFINES += ['ARM_MATH_CM3']
elif GetDepend('ARCH_ARM_CORTEX_M4'):
    CPPDEFINES += ['ARM_MATH_CM4']
elif GetDepend('ARCH_ARM_CORTEX_M0'):
    CPPDEFINES += ['ARM_MATH_CM0']

if GetDepend('ARCH_ARM_CORTEX_FPU'):
    CPPDEFINES += ['__FPU_PRESENT=1']

#delate non-used files
try:
    shutil.rmtree(os.path.join(cmsis_root_path,'.github'))
    shutil.rmtree(os.path.join(cmsis_root_path,'Device'))
    shutil.rmtree(os.path.join(cmsis_root_path,'docker'))
except:
    pass

group = DefineGroup('CMSIS-5', src, depend = ['PKG_USING_CMSIS_5'], CPPPATH = CPPPATH, LOCAL_CPPDEFINES=CPPDEFINES)

Return('group')
