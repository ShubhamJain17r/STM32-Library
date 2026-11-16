cmake_minimum_required(VERSION 3.16)

# Set toolchain file if not already specified on command line
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake")
endif()

project(stm32_library C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Target processor flags (STM32F446RE: Cortex-M4 with single-precision FPU)
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

add_compile_options(
    ${MCU_FLAGS}
    -Wall
    -Wextra
    -Wpedantic
    -ffunction-sections
    -fdata-sections
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
)

# Debug and Release defaults
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -g0 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g0 -DNDEBUG")

# Hardware definitions
add_definitions(-DSTM32F446xx)

# Include paths
include_directories(
    Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)

# Translation units: Core, Startup, CMSIS, and peripheral sources
# Explicitly lists the translation units residing under Inc/stm32/**
set(SOURCES
    # Application entry point & syscalls
    Src/main.cpp
    Src/syscalls.c
    Src/sysmem.c

    # Cortex-M4 Startup assembly & CMSIS system initialization
    Startup/startup_stm32f446retx.s
    Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c

    # Peripheral Translation Units (located under Inc/stm32/)
    Inc/stm32/common/systick/systick.cpp
    Inc/stm32/gpio/digital_input/digital_input.cpp
    Inc/stm32/gpio/digital_output/digital_output.cpp
    Inc/stm32/gpio/interrupt_input/interrupt_input.cpp
    Inc/stm32/exti/exti_manager.cpp
    Inc/stm32/exti/exti_irq.cpp
    Inc/stm32/timer/interrupt/timer_interrupt.cpp
    Inc/stm32/timer/interrupt/timer_irq.cpp
    Inc/stm32/uart/interrupt/uart_interrupt.cpp
    Inc/stm32/uart/interrupt/uart_irq.cpp
    Inc/stm32/spi/interrupt/spi_interrupt.cpp
    Inc/stm32/spi/interrupt/spi_irq.cpp
    Inc/stm32/i2c/interrupt/i2c_interrupt.cpp
    Inc/stm32/i2c/interrupt/i2c_irq.cpp
    Inc/stm32/adc/interrupt/adc_interrupt.cpp
    Inc/stm32/adc/interrupt/adc_irq.cpp
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/STM32F446RETX_FLASH.ld")

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -specs=nano.specs
    -specs=nosys.specs
)

# Generate raw binary (.bin) and Intel hex (.hex) artifacts
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex   ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    COMMENT "Building ${PROJECT_NAME}.bin and ${PROJECT_NAME}.hex, displaying memory footprint:"
)

# Flashing target with OpenOCD
add_custom_target(flash
    COMMAND openocd -f board/st_nucleo_f4.cfg -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing firmware to Nucleo-F446RE via OpenOCD..."
)

# Flashing target with st-flash (ST-Link tool)
add_custom_target(flash-stlink
    COMMAND st-flash write ${PROJECT_NAME}.bin 0x08000000
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Flashing ${PROJECT_NAME}.bin to 0x08000000 via st-flash..."
)
