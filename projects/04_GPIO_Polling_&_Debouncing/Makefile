# --- Configuración del Proyecto ---
MAIN_NAME = main
MCU       = atmega328p
F_CPU     = 16000000UL
PROGRAMMER = usbasp
PORT      = usb

# --- Rutas de la Isla (Relativas al Makefile) ---
# projects/01_blinky -> sube 2 niveles para llegar a la raíz de la isla
ROOT_DIR   := $(abspath ../../)

ifeq ($(OS),Windows_NT)
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

# --- Directorios de Librerías (Usando ROOT_DIR de la isla) ---
BUILD_DIR   = build
LIB_COMMON  = $(ROOT_DIR)/libs/common
LIB_HAL     = $(ROOT_DIR)/libs/hal_m328p
LIB_DEVICES = $(ROOT_DIR)/libs/devices

# --- Flags y Archivos ---
# Envolvemos las rutas con comillas para evitar errores por espacios en el nombre de usuario
INCLUDES = -I"./inc" \
           -I"$(LIB_COMMON)" \
           -I"$(LIB_HAL)/inc" \
           -I"$(LIB_DEVICES)/inc"

CFLAGS   = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDES)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS  = -Wl,--gc-sections

# --- ARCHIVOS FUENTE ---
SRCS = src/main.c
SRCS += "$(LIB_HAL)/src/gpio.c"

# --- Reglas ---
all: $(BUILD_DIR) compilacion size

$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

compilacion:
	@echo "Compilando en isla aislada: $(MCU)..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$(MAIN_NAME).elf $(SRCS)
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(MAIN_NAME).elf $(BUILD_DIR)/$(MAIN_NAME).hex

size:
	@echo "--- Uso de Memoria ---"
	$(SIZE) --format=berkeley $(BUILD_DIR)/$(MAIN_NAME).elf

flash:
	$(DUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(BUILD_DIR)/$(MAIN_NAME).hex:i

clean:
	$(RM) $(BUILD_DIR)

.PHONY: all compilacion flash clean size
