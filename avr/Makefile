# Makefile

# targets:
#   test:  test the programmer connection to the mcu
#   flash: writes compiled hex file to the mcu's flash memory
#   fuse:  writes fuse bytes to the mcu
#   clean: removes all .hex, .elf, and .o files in the source code

TARGET  := main
DEVICE  := m16
MCU     := atmega16a
PROG    := usbasp
LFU     := 0xE4

SRC_DIR := src
SRCS    :=  	\
	main.c		\
	twi.c		\
	usart.c		\
	spi.c		\
	lcdi2c.c	\
	nrf24l01.c

SRCS    := $(SRCS:%=$(SRC_DIR)/%)

BUILD_DIR   := build
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

INC_DIR := include
CFLAGS  := -Wall -Os -mmcu=$(MCU) -I$(INC_DIR)
CC      := avr-gcc
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
SIZE    := avr-size --format=avr --mcu=$(MCU)
DUDE    := avrdude -c $(PROG) -p $(DEVICE) -v

all: $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

test:
	$(DUDE)

flash: $(BUILD_DIR)/$(TARGET).hex
	$(DUDE) -U flash:w:$<:i

fuse:
	$(DUDE) -U lfuse:w:$(LFU):m

clean:
	rm -f $(BUILD_DIR)/*.hex $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.o

.PHONY: all test flash fuse clean