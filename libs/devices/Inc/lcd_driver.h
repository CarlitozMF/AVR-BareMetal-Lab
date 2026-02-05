/**
 * @file lcd.h
 * @brief Driver genérico para pantallas LCD Hitachi HD44780 (16x2, 20x4).
 * @author CarlitozMF (Basado en arquitectura de capas)
 * @date 2026
 * * @details Este driver implementa una Capa 2 (Device Driver) que es totalmente
 * agnóstica al hardware. Se comunica con el microcontrolador a través de la
 * Capa 1 (GPIO/Timer) permitiendo una portabilidad total entre AVRs.
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>
#include "gpio.h"   /* Capa 1 - HAL */
#include "timer.h"  /* Capa 1 - HAL (Para delays determinísticos) */

/** * @brief Tipos de display soportados.
 * Define la geometría y el mapeo de memoria DDRAM.
 */
typedef enum {
    LCD_16X2, /**< Display estándar de 16x2 */
    LCD_20X4  /**< Display de gran formato 20x4 */
} lcd_type_t;

/** * @brief Modos de visibilidad del cursor.
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
 * Permite asignar cualquier pin de cualquier puerto de forma independiente.
 */
typedef struct {
    gpio_port_t port_rs; uint8_t pin_rs;
    gpio_port_t port_en; uint8_t pin_en;
    gpio_port_t port_d4; uint8_t pin_d4;
    gpio_port_t port_d5; uint8_t pin_d5;
    gpio_port_t port_d6; uint8_t pin_d6;
    gpio_port_t port_d7; uint8_t pin_d7;
    lcd_type_t  type;    /**< Modelo del display */
} LCD_Config_t;

/* --- API Pública --- */

/**
 * @brief Inicializa el LCD en modo de 4 bits.
 * @param config Puntero a la estructura de configuración (debe persistir en memoria).
 */
void LCD_Init(const LCD_Config_t *config);

/**
 * @brief Limpia la pantalla y regresa el cursor a (0,0).
 */
void LCD_Clear(void);

/**
 * @brief Posiciona el cursor en una coordenada específica.
 * @param row Fila (0-1 para 16x2, 0-3 para 20x4).
 * @param col Columna (0-15 o 0-19).
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Imprime una cadena de caracteres (String).
 * @param str Cadena terminada en '\0'.
 */
void LCD_Print(const char *str);

/**
 * @brief Escribe un carácter individual.
 * @param data Carácter ASCII o ID de carácter personalizado.
 */
void LCD_WriteChar(char data);

/**
 * @brief Configura el modo del cursor.
 * @param mode Valor del enum lcd_cursor_mode_t.
 */
void LCD_SetCursorMode(lcd_cursor_mode_t mode);

/**
 * @brief Desplaza el contenido de la pantalla.
 * @param direction Dirección del desplazamiento.
 */
void LCD_Shift(lcd_shift_dir_t direction);

/**
 * @brief Crea un carácter personalizado.
 * @param location Posición 0-7.
 * @param charmap Array de 8 bytes (5 bits de ancho).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]);

#endif /* LCD_DRIVER_H_ */