/**
 * @file lcd_driver.c
 * @brief Implementación del driver para displays LCD basados en el controlador Hitachi HD44780.
 * @author CarlitozMF
 * @date 2026
 * * @details Este driver gestiona la comunicación en modo de 4 bits. Utiliza punteros a registros 
 * para garantizar la independencia del hardware mientras mantiene la eficiencia del acceso 
 * directo. Los tiempos de delay han sido validados mediante analizador lógico para asegurar 
 * compatibilidad con clones del controlador original.
 */

#ifndef F_CPU
#define F_CPU 16000000UL /**< Frecuencia de CPU por defecto si no se define en el Makefile */
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_driver.h"
#include "bits.h" /* Capa Common: Macros SET_BIT, CLR_BIT, etc. */

/** * @brief Instancia local de configuración.
 * @note Se almacena en SRAM para persistir las direcciones de los puertos (punteros) 
 * pasados desde el main, evitando lecturas erróneas desde la memoria de programa.
 */
static LCD_Config_t _lcd;

/* --- Funciones Privadas (Static) --- */

/**
 * @brief Genera un pulso de habilitación (Enable) en el display.
 * @details El pulso de 150us es superior al estándar (1us) para soportar pantallas
 * con controladores de baja calidad o cables largos.
 */
static void LCD_PulseEnable(void) {
    SET_BIT(*_lcd.port_en, _lcd.pin_en);
    _delay_us(150);  /* Validado con captura de analizador lógico */
    CLR_BIT(*_lcd.port_en, _lcd.pin_en);
    _delay_us(200);  /* Tiempo de espera para que el controlador procese el flanco */
}

/**
 * @brief Envía un nibble de 4 bits a los pines de datos D4-D7.
 * @param nibble Valor de 4 bits (bits 0-3 del parámetro).
 * @details Realiza una escritura bit a bit para permitir el uso de pines en diferentes puertos.
 */
static void LCD_SendNibble(uint8_t nibble) {
    (nibble & 0x01) ? SET_BIT(*_lcd.port_d4, _lcd.pin_d4) : CLR_BIT(*_lcd.port_d4, _lcd.pin_d4);
    (nibble & 0x02) ? SET_BIT(*_lcd.port_d5, _lcd.pin_d5) : CLR_BIT(*_lcd.port_d5, _lcd.pin_d5);
    (nibble & 0x04) ? SET_BIT(*_lcd.port_d6, _lcd.pin_d6) : CLR_BIT(*_lcd.port_d6, _lcd.pin_d6);
    (nibble & 0x08) ? SET_BIT(*_lcd.port_d7, _lcd.pin_d7) : CLR_BIT(*_lcd.port_d7, _lcd.pin_d7);
    LCD_PulseEnable();
}

/**
 * @brief Envía un byte completo al controlador dividiéndolo en dos nibbles.
 * @param byte El comando o dato a enviar.
 * @param is_data Flag de selección: 1 para datos (RS High), 0 para comandos (RS Low).
 */
static void LCD_SendByte(uint8_t byte, uint8_t is_data) {
    (is_data) ? SET_BIT(*_lcd.port_rs, _lcd.pin_rs) : CLR_BIT(*_lcd.port_rs, _lcd.pin_rs);
    
    LCD_SendNibble(byte >> 4);   /* Envía los 4 bits más significativos primero */
    _delay_us(100);              /* Tiempo entre nibbles */
    LCD_SendNibble(byte & 0x0F); /* Envía los 4 bits menos significativos */
    _delay_ms(2);                /* Delay de seguridad para comandos lentos */
}

/* --- Implementación de la API Pública --- */

/**
 * @brief Inicializa el hardware y el controlador LCD.
 * @param config Puntero a la estructura de configuración.
 * @note Implementa la secuencia de "Software Reset" necesaria para el modo de 4 bits.
 */
void LCD_Init(const LCD_Config_t *config) {
    _lcd = *config; /* Copia profunda de la configuración a SRAM */

    _delay_ms(150); /* Tiempo de espera tras el encendido (VDD estable) */

    /* Secuencia de inicialización forzada del HD44780 */
    LCD_SendNibble(0x03); _delay_ms(5);
    LCD_SendNibble(0x03); _delay_ms(1);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02); /* Configura definitivamente el modo 4 bits */

    /* Configuración por defecto: Interfaz 4-bits, 2 líneas, fuente 5x8 */
    LCD_SendByte(0x28, 0); 
    /* Display ON, Cursor OFF según el enum del driver */
    LCD_SendByte(0x0C, 0); 
    LCD_Clear();
}

/**
 * @brief Limpia la memoria de video y resetea el cursor.
 */
void LCD_Clear(void) {
    LCD_SendByte(0x01, 0);
    _delay_ms(5); /* La operación más lenta del display */
}

/**
 * @brief Posiciona el cursor en una coordenada (fila, columna).
 * @param row Fila (0-1 para 16x2, 0-3 para 20x4).
 * @param col Columna (0-15 o 0-19).
 */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr;
    if (_lcd.type == LCD_20X4) {
        uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        addr = 0x80 + row_offsets[row] + col;
    } else {
        addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    }
    LCD_SendByte(addr, 0);
}

/**
 * @brief Escribe una cadena de texto en la posición actual.
 * @param str Puntero a la cadena (string) terminada en null.
 */
void LCD_Print(const char *str) {
    while (*str) LCD_SendByte((uint8_t)(*str++), 1);
}

/**
 * @brief Escribe un único carácter.
 * @param data Byte de datos o carácter ASCII.
 */
void LCD_WriteChar(char data) {
    LCD_SendByte((uint8_t)data, 1);
}

/**
 * @brief Cambia el aspecto del cursor (Blink, On, Off).
 * @param mode Valor definido en @ref lcd_cursor_mode_t.
 */
void LCD_SetCursorMode(lcd_cursor_mode_t mode) {
    LCD_SendByte((uint8_t)mode, 0);
}

/**
 * @brief Desplaza el contenido de la pantalla sin modificar la memoria DDRAM.
 * @param direction Dirección definida en @ref lcd_shift_dir_t.
 */
void LCD_Shift(lcd_shift_dir_t direction) {
    LCD_SendByte((uint8_t)direction, 0);
}

/**
 * @brief Almacena un carácter personalizado en la CGRAM.
 * @param location Índice de memoria (0 a 7).
 * @param charmap Matriz de 8 bytes (5 bits significativos por fila).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; /* Máximo 8 caracteres personalizados */
    LCD_SendByte(0x40 | (location << 3), 0);
    for (uint8_t i = 0; i < 8; i++) {
        LCD_SendByte(charmap[i], 1);
    }
}