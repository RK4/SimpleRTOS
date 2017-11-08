#####################################################

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

#####################################################

PROJ_NAME=simplerots

###################################################
# Add application files here.
SRCS = main.c 
# OS files.
SRCS += bsp.c periodic_thread.c os/src/osasm.s os.c

###################################################

CFLAGS  = -g  -O0 -Wall -Tld/stm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

vpath %.c app os/inc os/src
vpath %.a lib
vpath %.a os

ROOT=$(shell pwd)

CFLAGS += -Ilib -Ilib/inc -Ios/inc
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 
SRCS += lib/startup_stm32f4xx.s  

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib  proj

all: lib  proj 

lib:
	$(MAKE) -C lib


proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS)  $^ -o $@  -Llib -lstm32f4 
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	$(MAKE) -C lib clean
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
