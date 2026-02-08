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
# Agregamos todas las rutas necesarias, incluyendo COMMON
INCLUDES = -I./inc \
           -I$(LIB_COMMON) \
           -I$(LIB_MCU)/inc \
           -I$(LIB_DEVICES)/inc

# Usamos += por si acaso quisiéramos agregar más flags después
CFLAGS   = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDES)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS  = -Wl,--gc-sections

# --- ARCHIVOS FUENTE ---
# Asegúrate de que las rutas coincidan con tu estructura (fíjate en Mayúsculas/Minúsculas)
SRCS = src/main.c
SRCS += $(LIB_MCU)/Src/gpio.c
SRCS += $(LIB_MCU)/Src/timer.c
SRCS += $(LIB_MCU)/Src/exti.c
SRCS += $(LIB_MCU)/Src/timer1_normal.c
SRCS += $(LIB_MCU)/Src/timer2_normal.c
SRCS += $(LIB_DEVICES)/Src/lcd_driver.c
SRCS += $(LIB_DEVICES)/Src/step_motor_28BYJ48.c

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