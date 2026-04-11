/**
 * @file lcd_driver.c
 * @brief Implementación del driver para displays LCD basados en el controlador Hitachi HD44780.
 * @author CarlitozMF
 * @date 2026
 * * @details Este driver gestiona la comunicación en modo de 4 bits. Utiliza punteros a registros 
 * para garantizar la independencia del hardware mientras mantiene la eficiencia del acceso 
 * directo. Los tiempos de delay han sido validados mediante analizador lógico para asegurar 
 * compatibilidad con clones del controlador original y robustez ante cables largos.
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
 * pasados desde el main, evitando lecturas erróneas desde la memoria de programa 
 * y permitiendo un acceso rápido en cada operación de I/O.
 */
static LCD_Config_t _lcd;

/* --- Funciones Privadas (Static) --- */

/**
 * @brief Genera un pulso de habilitación (Enable) en el display.
 * @details El pulso de 150us asegura que incluso los controladores más lentos 
 * capturen el estado del bus de datos. 
 * @note Basado en validaciones con analizador lógico para garantizar estabilidad HMI.
 */
static void LCD_PulseEnable(void) {
    SET_BIT(*_lcd.port_en, _lcd.pin_en);
    _delay_us(150);  /* Tiempo en alto (Data Setup Time) */
    CLR_BIT(*_lcd.port_en, _lcd.pin_en);
    _delay_us(200);  /* Tiempo de espera (Data Hold Time / Cycle Time) */
}

/**
 * @brief Envía un nibble de 4 bits a los pines de datos D4-D7.
 * @param nibble Valor de 4 bits (se procesan los bits 0-3 del parámetro).
 * @details Realiza una escritura bit a bit, lo que permite que el bus de datos 
 * esté distribuido en diferentes puertos físicos según la conveniencia del hardware.
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
    _delay_us(100);              /* Retardo de sincronía entre nibbles */
    LCD_SendNibble(byte & 0x0F); /* Envía los 4 bits menos significativos */
    _delay_ms(2);                /* Retardo de seguridad para asegurar el procesamiento interno */
}

/* --- Implementación de la API Pública --- */

/**
 * @brief Inicializa el hardware y el controlador LCD.
 * @param config Puntero a la estructura de configuración.
 * @note Implementa la secuencia de "Software Reset" necesaria para estabilizar 
 * el modo de 4 bits independientemente del estado previo del hardware.
 */
void LCD_Init(const LCD_Config_t *config) {
    _lcd = *config; /* Copia profunda de la configuración a la instancia local estática */

    _delay_ms(150); /* Tiempo de espera tras el encendido para estabilización de VDD */

    /* Secuencia de inicialización forzada (Secuencia de Reset del HD44780) */
    LCD_SendNibble(0x03); _delay_ms(5);
    LCD_SendNibble(0x03); _delay_ms(1);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02); /* Configura definitivamente el bus en modo 4 bits */

    /* Configuración de parámetros: Interfaz 4-bits, 2 líneas (o más), fuente 5x8 */
    LCD_SendByte(0x28, 0); 
    /* Configuración visual: Display ON, Cursor invisible por defecto */
    LCD_SendByte(0x0C, 0); 
    LCD_Clear();
}

/**
 * @brief Limpia la memoria de video (DDRAM) y resetea el cursor.
 * @note Esta operación es bloqueante por ~5ms debido a la latencia del controlador.
 */
void LCD_Clear(void) {
    LCD_SendByte(0x01, 0);
    _delay_ms(5); 
}

/**
 * @brief Posiciona el cursor en una coordenada DDRAM (fila, columna).
 * @param row Fila (0-1 para 16x2, 0-3 para 20x4).
 * @param col Columna (0-15 o 0-19 según el tipo definido en la inicialización).
 */
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr;
    if (_lcd.type == LCD_20X4) {
        /* Mapeo de direcciones DDRAM para displays de 4 filas */
        uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        addr = 0x80 + row_offsets[row] + col;
    } else {
        /* Mapeo estándar para 16x2 */
        addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    }
    LCD_SendByte(addr, 0);
}

/**
 * @brief Escribe una cadena de texto en la posición actual del cursor.
 * @param str Puntero a la cadena (string) terminada en null almacenada en RAM.
 */
void LCD_Print(const char *str) {
    while (*str) LCD_SendByte((uint8_t)(*str++), 1);
}

/**
 * @brief Escribe un único carácter en la DDRAM o CGRAM.
 * @param data Byte de datos o carácter ASCII.
 */
void LCD_WriteChar(char data) {
    LCD_SendByte((uint8_t)data, 1);
}

/**
 * @brief Configura el aspecto del cursor (Blink, On, Off).
 * @param mode Valor definido en el enum @ref lcd_cursor_mode_t.
 */
void LCD_SetCursorMode(lcd_cursor_mode_t mode) {
    LCD_SendByte((uint8_t)mode, 0);
}

/**
 * @brief Desplaza el contenido de la pantalla visualmente.
 * @param direction Dirección definida en @ref lcd_shift_dir_t.
 */
void LCD_Shift(lcd_shift_dir_t direction) {
    LCD_SendByte((uint8_t)direction, 0);
}

/**
 * @brief Almacena un patrón de mapa de bits para caracteres personalizados.
 * @param location Índice de memoria CGRAM (0 a 7).
 * @param charmap Matriz de 8 bytes representando el símbolo (5x8 píxeles).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; /* Asegura que el índice esté en el rango de 8 slots disponibles */
    /* Configura la dirección de CGRAM (0x40 es la base de CGRAM) */
    LCD_SendByte(0x40 | (location << 3), 0);
    for (uint8_t i = 0; i < 8; i++) {
        LCD_SendByte(charmap[i], 1);
    }
}