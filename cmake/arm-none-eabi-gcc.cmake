# CMake Toolchain File for ARM GCC (Cortex-M4F STM32F446RE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Allow users to specify toolchain prefix/path via environment variable or CMake variable
if(NOT DEFINED TOOLCHAIN_PREFIX)
    if(DEFINED ENV{ARM_TOOLCHAIN_DIR})
        set(TOOLCHAIN_PREFIX "$ENV{ARM_TOOLCHAIN_DIR}/arm-none-eabi-")
    elseif(EXISTS "/home/shubham/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-gcc")
        set(TOOLCHAIN_PREFIX "/home/shubham/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-")
    else()
        set(TOOLCHAIN_PREFIX "arm-none-eabi-")
    endif()
endif()

set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_OBJDUMP      "${TOOLCHAIN_PREFIX}objdump")
set(CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
