# ==============================================================================
# stm32_sources.mk - STM32-Library Source & Include Makefile Fragment
# Tier 1 Bare-Metal Silicon HAL Engine
# ==============================================================================
# Usage in downstream Makefiles:
#   LIB_DIR ?= path/to/STM32-Library
#   include $(LIB_DIR)/stm32_sources.mk
#
# Provided variables:
#   STM32_LIB_SOURCES      - Complete list of all HAL C, C++, and ASM source files
#   STM32_LIB_C_SOURCES    - HAL C source files (syscalls, sysmem, CMSIS system init)
#   STM32_LIB_CPP_SOURCES  - HAL C++ source files (SysTick, GPIO, EXTI, Timer, UART, SPI, I2C, ADC, DMA)
#   STM32_LIB_ASM_SOURCES  - Cortex-M4 startup assembly source file
#   STM32_LIB_INCLUDES     - Compiler include flags (-I...)
#   STM32_LIB_INCLUDE_DIRS - Raw directory paths without -I
#   STM32_LINKER_SCRIPT    - Path to STM32F446RETX_FLASH.ld linker script
#   STM32_MCU_FLAGS        - Target processor flags (-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
#   STM32_DEFS             - Target hardware macro definitions (-DSTM32F446xx)
# ==============================================================================

# Determine directory of this fragment if not explicitly set
STM32_LIB_DIR ?= $(if $(LIB_DIR),$(LIB_DIR),$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST)))))
ifeq ($(STM32_LIB_DIR),.)
  STM32_LIB_PREFIX :=
else
  STM32_LIB_PREFIX := $(STM32_LIB_DIR)/
endif

# Target Processor & Hardware Defines
STM32_MCU_FLAGS ?= -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
STM32_DEFS      ?= -DSTM32F446xx

# Include Paths
STM32_LIB_INCLUDE_DIRS = \
    $(STM32_LIB_PREFIX)Inc \
    $(STM32_LIB_PREFIX)Drivers/CMSIS/Device/ST/STM32F4xx/Include \
    $(STM32_LIB_PREFIX)Drivers/CMSIS/Include

STM32_LIB_INCLUDES = $(addprefix -I,$(STM32_LIB_INCLUDE_DIRS))

# Linker Script
STM32_LINKER_SCRIPT = $(STM32_LIB_PREFIX)STM32F446RETX_FLASH.ld

# C Sources (Syscalls, Heap, CMSIS system initialization)
STM32_LIB_C_SOURCES = \
    $(STM32_LIB_PREFIX)Src/syscalls.c \
    $(STM32_LIB_PREFIX)Src/sysmem.c \
    $(STM32_LIB_PREFIX)Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c

# C++ Peripheral Translation Units
STM32_LIB_CPP_SOURCES = \
    $(STM32_LIB_PREFIX)Inc/stm32/common/systick/systick.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/gpio/digital_input/digital_input.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/gpio/digital_output/digital_output.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/gpio/interrupt_input/interrupt_input.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/exti/exti_manager.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/exti/exti_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/timer/interrupt/timer_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/timer/interrupt/timer_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/uart/interrupt/uart_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/uart/interrupt/uart_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/spi/interrupt/spi_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/spi/interrupt/spi_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/i2c/interrupt/i2c_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/i2c/interrupt/i2c_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/adc/interrupt/adc_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/adc/interrupt/adc_irq.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/dma/interrupt/dma_interrupt.cpp \
    $(STM32_LIB_PREFIX)Inc/stm32/dma/interrupt/dma_irq.cpp

# Assembly Startup File
STM32_LIB_ASM_SOURCES = \
    $(STM32_LIB_PREFIX)Startup/startup_stm32f446retx.s

# Aggregate of all HAL sources
STM32_LIB_SOURCES = \
    $(STM32_LIB_C_SOURCES) \
    $(STM32_LIB_CPP_SOURCES) \
    $(STM32_LIB_ASM_SOURCES)
