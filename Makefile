#make file for agi sim

CC = gcc
CXX = g++
LD = ld

PROJECT = samduino-loader

OUTPUT_DIR = output
CURRENT_DIR = ${CURDIR}

INCLUDE_DIRS = -I$(CURRENT_DIR)/src -I$(CURRENT_DIR)/src/core -I$(CURRENT_DIR)/src/core/ASF/thirdparty/freertos/freertos-10.0.0/Source/include/ -I$(CURRENT_DIR)/src/core/config/ -I$(CURRENT_DIR)/src/core/ASF/ -I$(CURRENT_DIR)/src/core/ASF/common/ -I$(CURRENT_DIR)/src/core/ASF/common/boards/ -I$(CURRENT_DIR)/src/core/ASF/common/boards/user_board/ -I$(CURRENT_DIR)/src/core/ASF/common/components/ -I$(CURRENT_DIR)/src/core/ASF/common/components/memory/ -I$(CURRENT_DIR)/src/core/ASF/common/components/memory/sd_mmc/ -I$(CURRENT_DIR)/src/core/ASF/common/services/ -I$(CURRENT_DIR)/src/core/ASF/common/services/clock/ -I$(CURRENT_DIR)/src/core/ASF/common/services/clock/sam4s/ -I$(CURRENT_DIR)/src/core/ASF/common/services/delay/ -I$(CURRENT_DIR)/src/core/ASF/common/services/delay/sam/ -I$(CURRENT_DIR)/src/core/ASF/common/services/gpio/ -I$(CURRENT_DIR)/src/core/ASF/common/services/gpio/sam_gpio/ -I$(CURRENT_DIR)/src/core/ASF/common/services/ioport/ -I$(CURRENT_DIR)/src/core/ASF/common/services/ioport/sam/ -I$(CURRENT_DIR)/src/core/ASF/common/services/sleepmgr/ -I$(CURRENT_DIR)/src/core/ASF/common/services/sleepmgr/sam/ -I$(CURRENT_DIR)/src/core/ASF/common/services/storage/ -I$(CURRENT_DIR)/src/core/ASF/common/services/storage/ctrl_access/ -I$(CURRENT_DIR)/src/core/ASF/common/services/usb/ -I$(CURRENT_DIR)/src/core/ASF/common/services/usb/class/ -I$(CURRENT_DIR)/src/core/ASF/common/services/usb/class/cdc/ -I$(CURRENT_DIR)/src/core/ASF/common/services/usb/class/cdc/device/ -I$(CURRENT_DIR)/src/core/ASF/common/services/usb/udc/ -I$(CURRENT_DIR)/src/core/ASF/common/services/twi/ -I$(CURRENT_DIR)/src/core/ASF/common/utils/ -I$(CURRENT_DIR)/src/core/ASF/common/utils/interrupt/ -I$(CURRENT_DIR)/src/core/ASF/common/utils/stdio/ -I$(CURRENT_DIR)/src/core/ASF/common/utils/stdio/stdio_usb/ -I$(CURRENT_DIR)/src/core/ASF/sam/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/adc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/dacc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/hsmci/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/matrix/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/pdc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/pdc/pdc_uart_example/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/pio/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/pmc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/rstc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/rstc/example1/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/rtc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/tc/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/tc/tc_capture_waveform_example/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/uart/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/spi/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/twi/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/udp/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/usart/ -I$(CURRENT_DIR)/src/core/ASF/sam/drivers/efc/ -I$(CURRENT_DIR)/src/core/ASF/sam/services/flash_efc/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/include/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/include/component/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/include/instance/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/include/pio/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/source/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/source/templates/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/cmsis/sam4s/source/templates/gcc/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/header_files/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/linker_scripts/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/linker_scripts/sam4s/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/linker_scripts/sam4s/sam4s8/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/linker_scripts/sam4s/sam4s8/gcc/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/make/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/preprocessor/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/syscalls/ -I$(CURRENT_DIR)/src/core/ASF/sam/utils/syscalls/gcc/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/CMSIS/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/CMSIS/Include/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/CMSIS/Lib/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/CMSIS/Lib/GCC/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-port-r0.09/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-port-r0.09/sam/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/doc/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/doc/en/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/doc/img/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/src/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/fatfs/fatfs-r0.09/src/option/ -I$(CURRENT_DIR)/src/core/ASF/thirdparty/freertos/freertos-10.0.0/Source/portable/GCC/ARM_CM3/

DEFINES =

CCFLAGS = -g 
CXXFLAGS = -g

LINKER_FLAGS = 
LIBS = 
LINKER_SCRIPT = 

OBJ_COPY_FLAGS = -O binary

AS_SRCS=$(shell find . -name '*.asm')
CC_SRCS=$(shell find -L . -name '*.c')
CXX_SRCS=$(shell find . -name '*.cpp')

AS_OBJS=$(patsubst %.asm,$(OUTPUT_DIR)/%.0,$(AS_SRCS))
CC_OBJS=$(patsubst %.c,$(OUTPUT_DIR)/%.o,$(CC_SRCS))
CXX_OBJS=$(patsubst %.cpp,$(OUTPUT_DIR)/%.o,$(CXX_SRCS))

all: $(OUTPUT_DIR)/$(PROJECT)
	

$(OUTPUT_DIR)/$(PROJECT): $(CC_OBJS) $(CXX_OBJS)
	@echo compiling main
	@$(CC) $(LINKER_FLAGS) $(DEFINES) $(CC_OBJS) $(CXX_OBJS) $(LIBS) $(LINKER_SCRIPT) -o $@.out

$(OUTPUT_DIR)/%.o: %.c
	@echo compiling $<
	@mkdir -p $(@D)
	@$(CC) $(CCFLAGS) $(INCLUDE_DIRS) $(DEFINES) -MMD -MF $@.d -c $< -o $@

$(OUTPUT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(DEFINES) -MMD -MF $@.d -c $< -o $@
	

rebuild: clean all

clean:
	rm -rf output

.PHONY: all clean