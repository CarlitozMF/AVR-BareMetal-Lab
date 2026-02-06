# --- Configuración del Proyecto ---
MAIN_NAME = main
MCU       = atmega328p
F_CPU     = 16000000UL
PROGRAMMER = usbasp
PORT       = usb

# --- Rutas Portátiles ---
ifeq ($(OS),Windows_NT)
    ROOT_DIR   := $(abspath ../../../)
    TOOLS_DIR  := $(ROOT_DIR)/tools/avr-gcc
    CC         := "$(TOOLS_DIR)/bin/avr-gcc.exe"
    OBJCOPY    := "$(TOOLS_DIR)/bin/avr-objcopy.exe"
    SIZE       := "$(TOOLS_DIR)/bin/avr-size.exe"
    DUDE       := "$(ROOT_DIR)/tools/avrdude/avrdude.exe"
    RM         = rm -rf
    MKDIR      = mkdir -p
else
    CC      = avr-gcc
    OBJCOPY = avr-objcopy
    SIZE    = avr-size
    DUDE    = avrdude
    RM      = rm -rf
    MKDIR   = mkdir -p
endif

# --- Directorios de Librerías ---
BUILD_DIR   = build
LIB_COMMON  = ../../../libs/common
LIB_MCU     = ../../../libs/atmega328p
LIB_DEVICES = ../../../libs/devices

# --- Flags y Archivos ---
# CORRECCIÓN: Agregamos el path de los .h de devices e inc local
INCLUDES = -I./inc \
           -I$(LIB_COMMON) \
           -I$(LIB_MCU)/Inc \
           -I$(LIB_DEVICES)/Inc

CFLAGS   = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDES)

# --- ARCHIVOS FUENTE ---
# Asegúrate de que las rutas coincidan con tu estructura (fíjate en Mayúsculas/Minúsculas)
SRCS = src/main.c
SRCS += $(LIB_MCU)/Src/gpio.c
SRCS += $(LIB_MCU)/Src/timer.c
SRCS += $(LIB_MCU)/Src/exti.c
SRCS += $(LIB_DEVICES)/Src/lcd_driver.c

# --- Reglas ---
all: $(BUILD_DIR) compilacion size

$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

compilacion:
	@echo "Compilando para $(MCU)..."
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(MAIN_NAME).elf $(SRCS)
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(MAIN_NAME).elf $(BUILD_DIR)/$(MAIN_NAME).hex

size:
	@echo "--- Uso de Memoria ---"
	$(SIZE) --format=berkeley $(BUILD_DIR)/$(MAIN_NAME).elf
    
flash:
	$(DUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(BUILD_DIR)/$(MAIN_NAME).hex:i

clean:
	$(RM) $(BUILD_DIR)

.PHONY: all compilacion flash clean size