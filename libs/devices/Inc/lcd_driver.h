/**
 * @file lcd_driver.h
 * @brief Driver genérico para pantallas LCD Hitachi HD44780 (16x2, 20x4).
 * @author CarlitozMF (Basado en arquitectura de capas)
 * @date 2026
 * * @details Este driver implementa una Capa 2 (Device Driver) independiente.
 * Utiliza punteros a registros (volatile uint8_t*) para interactuar con la 
 * Capa 1, permitiendo mapear pines en diferentes puertos sin pérdida de performance.
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>

/** * @brief Tipos de display soportados.
 * Define la geometría para el cálculo de direcciones de memoria DDRAM.
 */
typedef enum {
    LCD_16X2, /**< Display estándar de 16 columnas y 2 filas */
    LCD_20X4  /**< Display de gran formato de 20 columnas y 4 filas */
} lcd_type_t;

/** * @brief Modos de visibilidad del cursor.
 * Basado en el registro 'Display ON/OFF Control'.
 */
typedef enum {
    LCD_CURSOR_OFF   = 0x0C, /**< Pantalla ON, Cursor invisible */
    LCD_CURSOR_ON    = 0x0E, /**< Pantalla ON, Cursor subrayado (_) */
    LCD_CURSOR_BLINK = 0x0F  /**< Pantalla ON, Cursor bloque parpadeante */
} lcd_cursor_mode_t;

/** * @brief Direcciones de desplazamiento de pantalla.
 */
typedef enum {
    LCD_SHIFT_LEFT  = 0x18, /**< Desplaza contenido a la izquierda */
    LCD_SHIFT_RIGHT = 0x1C  /**< Desplaza contenido a la derecha */
} lcd_shift_dir_t;

/**
 * @brief Estructura de configuración del hardware.
 * @note Los puertos deben pasarse como direcciones de memoria (ej. &PORTB).
 */
typedef struct {
    volatile uint8_t* port_rs; uint8_t pin_rs; /**< Puerto y pin para Register Select */
    volatile uint8_t* port_en; uint8_t pin_en; /**< Puerto y pin para Enable */
    volatile uint8_t* port_d4; uint8_t pin_d4; /**< Pin de datos D4 */
    volatile uint8_t* port_d5; uint8_t pin_d5; /**< Pin de datos D5 */
    volatile uint8_t* port_d6; uint8_t pin_d6; /**< Pin de datos D6 */
    volatile uint8_t* port_d7; uint8_t pin_d7; /**< Pin de datos D7 */
    lcd_type_t  type;    /**< Modelo del display (Geometría) */
} LCD_Config_t;

/* --- API Pública --- */

/**
 * @brief Inicializa el LCD en modo de 4 bits.
 * @param config Puntero a la estructura de configuración. Se copia a SRAM interna.
 */
void LCD_Init(const LCD_Config_t *config);

/**
 * @brief Limpia la pantalla y regresa el cursor a la posición (0,0).
 */
void LCD_Clear(void);

/**
 * @brief Posiciona el cursor en una coordenada específica.
 * @param row Fila (0-1 para 16x2, 0-3 para 20x4).
 * @param col Columna.
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Imprime una cadena de caracteres terminada en null.
 * @param str Puntero a la cadena en RAM.
 */
void LCD_Print(const char *str);

/**
 * @brief Escribe un carácter individual en la posición actual.
 * @param data Carácter ASCII o ID de carácter personalizado.
 */
void LCD_WriteChar(char data);

/**
 * @brief Configura el modo del cursor (encendido, apagado o parpadeo).
 * @param mode Valor de lcd_cursor_mode_t.
 */
void LCD_SetCursorMode(lcd_cursor_mode_t mode);

/**
 * @brief Desplaza todo el contenido de la pantalla sin alterar la DDRAM.
 * @param direction Dirección del desplazamiento.
 */
void LCD_Shift(lcd_shift_dir_t direction);

/**
 * @brief Carga un patrón de 8 bytes en la CGRAM del LCD.
 * @param location Índice del carácter (0-7).
 * @param charmap Array de 8 bytes con el patrón (5 bits de ancho).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]);

#endif /* LCD_DRIVER_H_ */