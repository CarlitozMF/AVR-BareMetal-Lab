/**
 * @file lcd.h
 * @brief Driver genérico para pantallas LCD Hitachi HD44780 (16x2, 20x4).
 * @author CarlitozMF (Basado en arquitectura de capas)
 * @date 2026
 * * @details Este driver implementa una Capa 2 (Device Driver) que es totalmente
 * agnóstica al hardware. Se comunica con el microcontrolador a través de la
 * Capa 1 (GPIO/Timer) permitiendo una portabilidad total entre AVRs.
 */
/**
 * @file lcd.c
 * @brief Implementación de la lógica de comunicación para LCD en modo 4 bits.
 */

#include "lcd_driver.h"

/* Handle estático para guardar la referencia a la configuración */
static const LCD_Config_t *lcd_handle;

/* Offsets de memoria DDRAM según el datasheet del HD44780 */
static const uint8_t row_offsets_16x2[] = { 0x00, 0x40 };
static const uint8_t row_offsets_20x4[] = { 0x00, 0x40, 0x14, 0x54 };

/* --- Funciones Privadas (Encapsulamiento) --- */

/**
 * @brief Genera el pulso de Enable (E) para validar datos/comandos.
 */
static void LCD_PulseEnable(void) {
    GPIO_WritePin(lcd_handle->port_en, lcd_handle->pin_en, GPIO_HIGH);
    delay_ms_tick(1); 
    GPIO_WritePin(lcd_handle->port_en, lcd_handle->pin_en, GPIO_LOW);
    delay_ms_tick(1);
}

/**
 * @brief Envía un nible (4 bits) al bus de datos.
 */
static void LCD_SendNibble(uint8_t nibble) {
    GPIO_WritePin(lcd_handle->port_d4, lcd_handle->pin_d4, (nibble >> 0) & 0x01);
    GPIO_WritePin(lcd_handle->port_d5, lcd_handle->pin_d5, (nibble >> 1) & 0x01);
    GPIO_WritePin(lcd_handle->port_d6, lcd_handle->pin_d6, (nibble >> 2) & 0x01);
    GPIO_WritePin(lcd_handle->port_d7, lcd_handle->pin_d7, (nibble >> 3) & 0x01);
    LCD_PulseEnable();
}

/**
 * @brief Envía un byte completo dividiéndolo en dos nibles.
 */
static void LCD_SendByte(uint8_t byte, gpio_state_t mode) {
    GPIO_WritePin(lcd_handle->port_rs, lcd_handle->pin_rs, mode);
    LCD_SendNibble(byte >> 4);   /* Nible alto */
    LCD_SendNibble(byte & 0x0F); /* Nible bajo */
}

/* --- Implementación de Funciones Públicas --- */

void LCD_Init(const LCD_Config_t *config) {
    lcd_handle = config;

    /* Configurar pines como salida */
    GPIO_InitPin(lcd_handle->port_rs, lcd_handle->pin_rs, GPIO_OUTPUT);
    GPIO_InitPin(lcd_handle->port_en, lcd_handle->pin_en, GPIO_OUTPUT);
    GPIO_InitPin(lcd_handle->port_d4, lcd_handle->pin_d4, GPIO_OUTPUT);
    GPIO_InitPin(lcd_handle->port_d5, lcd_handle->pin_d5, GPIO_OUTPUT);
    GPIO_InitPin(lcd_handle->port_d6, lcd_handle->pin_d6, GPIO_OUTPUT);
    GPIO_InitPin(lcd_handle->port_d7, lcd_handle->pin_d7, GPIO_OUTPUT);

    /* Secuencia de inicialización crítica del Datasheet */
    delay_ms_tick(50); 
    GPIO_WritePin(lcd_handle->port_rs, lcd_handle->pin_rs, GPIO_LOW);
    
    /* Forzar modo 4 bits */
    LCD_SendNibble(0x03);
    delay_ms_tick(5);
    LCD_SendNibble(0x03);
    delay_ms_tick(1);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02); 

    /* Configuración por defecto */
    LCD_SendByte(0x28, GPIO_LOW); /* Modo 4 bits, 2 líneas, 5x8 font */
    LCD_SendByte(0x0C, GPIO_LOW); /* Display ON, Cursor OFF */
    LCD_SendByte(0x06, GPIO_LOW); /* Incremento automático del cursor */
    LCD_Clear();
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address = 0;
    if (lcd_handle->type == LCD_16X2) {
        address = row_offsets_16x2[row % 2] + col;
    } else {
        address = row_offsets_20x4[row % 4] + col;
    }
    LCD_SendByte(0x80 | address, GPIO_LOW);
}

void LCD_Clear(void) {
    LCD_SendByte(0x01, GPIO_LOW);
    delay_ms_tick(2); /* Clear requiere 1.52ms según datasheet */
}

void LCD_WriteChar(char data) {
    LCD_SendByte(data, GPIO_HIGH);
}

void LCD_Print(const char *str) {
    while (*str) LCD_WriteChar(*str++);
}

void LCD_SetCursorMode(lcd_cursor_mode_t mode) {
    LCD_SendByte((uint8_t)mode, GPIO_LOW);
}

void LCD_Shift(lcd_shift_dir_t direction) {
    LCD_SendByte((uint8_t)direction, GPIO_LOW);
}

/**
 * @brief Crea un carácter personalizado.
 * @param location Posición 0-7.
 * @param charmap Array de 8 bytes (5 bits de ancho).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x07; // Solo 8 espacios disponibles
    LCD_SendByte(0x40 | (location << 3), GPIO_LOW); // Apuntar a CGRAM
    for (uint8_t i = 0; i < 8; i++) {
        LCD_SendByte(charmap[i], GPIO_HIGH);
    }
}