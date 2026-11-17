# Makefile for STM32F446RE Bare-Metal C++17 Library on Linux

TARGET = stm32_library
BUILD_DIR = build

# Toolchain configuration (override via environment or CLI: make ARM_TOOLCHAIN_DIR=...)
ifneq ($(ARM_TOOLCHAIN_DIR),)
    PREFIX = $(ARM_TOOLCHAIN_DIR)/arm-none-eabi-
else ifneq ($(wildcard /home/shubham/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-gcc),)
    PREFIX = /home/shubham/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
else
    PREFIX = arm-none-eabi-
endif

CC      = $(PREFIX)gcc
CXX     = $(PREFIX)g++
AS      = $(PREFIX)gcc -x assembler-with-cpp
CP      = $(PREFIX)objcopy
SZ      = $(PREFIX)size
HEX     = $(CP) -O ihex
BIN     = $(CP) -O binary -S

# Target Architecture Flags (Cortex-M4F)
MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Hardware Defines
DEFS = -DSTM32F446xx

# Include Paths
INCLUDES = \
    -IInc \
    -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
    -IDrivers/CMSIS/Include

# Compiler Flags
COMMON_FLAGS = $(MCU) $(DEFS) $(INCLUDES) -Wall -Wextra -Wpedantic -ffunction-sections -fdata-sections -O2 -g3
CFLAGS   = $(COMMON_FLAGS) -std=c11
CXXFLAGS = $(COMMON_FLAGS) -std=c++17 -fno-exceptions -fno-rtti -fno-use-cxa-atexit
ASFLAGS  = $(MCU) $(DEFS) $(INCLUDES) -Wall -fdata-sections -ffunction-sections

# Linker Flags
LDSCRIPT = STM32F446RETX_FLASH.ld
LDFLAGS = $(MCU) -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs

# Source Files
C_SOURCES = \
    Src/syscalls.c \
    Src/sysmem.c \
    Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c

CPP_SOURCES = \
    Src/main.cpp \
    Inc/stm32/common/systick/systick.cpp \
    Inc/stm32/gpio/digital_input/digital_input.cpp \
    Inc/stm32/gpio/digital_output/digital_output.cpp \
    Inc/stm32/gpio/interrupt_input/interrupt_input.cpp \
    Inc/stm32/exti/exti_manager.cpp \
    Inc/stm32/exti/exti_irq.cpp \
    Inc/stm32/timer/interrupt/timer_interrupt.cpp \
    Inc/stm32/timer/interrupt/timer_irq.cpp \
    Inc/stm32/uart/interrupt/uart_interrupt.cpp \
    Inc/stm32/uart/interrupt/uart_irq.cpp \
    Inc/stm32/spi/interrupt/spi_interrupt.cpp \
    Inc/stm32/spi/interrupt/spi_irq.cpp \
    Inc/stm32/i2c/interrupt/i2c_interrupt.cpp \
    Inc/stm32/i2c/interrupt/i2c_irq.cpp \
    Inc/stm32/adc/interrupt/adc_interrupt.cpp \
    Inc/stm32/adc/interrupt/adc_irq.cpp \
    Inc/stm32/dma/interrupt/dma_interrupt.cpp \
    Inc/stm32/dma/interrupt/dma_irq.cpp

ASM_SOURCES = \
    Startup/startup_stm32f446retx.s

# Object List
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

.PHONY: all clean size flash flash-stlink debug

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin size

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "  CC    $<"
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "  CXX   $<"
	@$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "  AS    $<"
	@$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "  LINK  $@"
	@$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "  HEX   $@"
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "  BIN   $@"
	@$(BIN) $< $@

$(BUILD_DIR):
	@mkdir -p $@

size: $(BUILD_DIR)/$(TARGET).elf
	@echo "  SIZE  $<"
	@$(SZ) --format=berkeley $<

clean:
	@rm -rf $(BUILD_DIR)

flash: all
	openocd -f board/st_nucleo_f4.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

flash-stlink: all
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

debug: all
	openocd -f board/st_nucleo_f4.cfg
