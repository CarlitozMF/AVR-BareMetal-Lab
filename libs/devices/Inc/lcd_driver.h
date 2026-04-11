/**
 * @file lcd_driver.h
 * @brief Driver de Capa 2 para pantallas LCD Hitachi HD44780 (16x2, 20x4).
 * @author CarlitozMF (Basado en arquitectura de capas)
 * @date 2026
 * * @details Este driver implementa una abstracción de hardware independiente. 
 * Utiliza punteros a registros (volatile uint8_t*) permitiendo mapear pines 
 * en diferentes puertos físicos sin pérdida de performance. La comunicación 
 * se realiza en modo de 4 bits para optimizar el uso de GPIOs.
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>

/** * @name Tipos de Geometría
 * @{ */
typedef enum {
    LCD_16X2, /**< Display estándar de 16 columnas y 2 filas */
    LCD_20X4  /**< Display de gran formato de 20 columnas y 4 filas */
} lcd_type_t;
/** @} */

/** * @name Modos de Visibilidad
 * @{ */
typedef enum {
    LCD_CURSOR_OFF   = 0x0C, /**< Pantalla ON, Cursor invisible */
    LCD_CURSOR_ON    = 0x0E, /**< Pantalla ON, Cursor subrayado (_) */
    LCD_CURSOR_BLINK = 0x0F  /**< Pantalla ON, Cursor bloque parpadeante */
} lcd_cursor_mode_t;
/** @} */

/** * @name Control de Desplazamiento
 * @{ */
typedef enum {
    LCD_SHIFT_LEFT  = 0x18, /**< Desplaza contenido a la izquierda */
    LCD_SHIFT_RIGHT = 0x1C  /**< Desplaza contenido a la derecha */
} lcd_shift_dir_t;
/** @} */

/**
 * @brief Estructura de configuración del hardware LCD.
 * @note Los puertos deben pasarse como direcciones de memoria del periférico (ej. &PORTB).
 */
typedef struct {
    volatile uint8_t* port_rs; uint8_t pin_rs; /**< Register Select: Low=Instrucción, High=Dato */
    volatile uint8_t* port_en; uint8_t pin_en; /**< Enable: Disparo por flanco descendente */
    volatile uint8_t* port_d4; uint8_t pin_d4; /**< Pin de datos Bus D4 */
    volatile uint8_t* port_d5; uint8_t pin_d5; /**< Pin de datos Bus D5 */
    volatile uint8_t* port_d6; uint8_t pin_d6; /**< Pin de datos Bus D6 */
    volatile uint8_t* port_d7; uint8_t pin_d7; /**< Pin de datos Bus D7 */
    lcd_type_t   type;                         /**< Modelo del display para gestión de DDRAM */
} LCD_Config_t;

/* --- API Pública de Control --- */

/**
 * @brief Inicializa el LCD en modo de 4 bits.
 * @details Realiza la secuencia de reset por software y configura el Function Set.
 * @param config Puntero a la estructura de configuración (se recomienda persistencia en SRAM).
 * @note Requiere que los pines ya estén configurados como SALIDAS en la Capa 1.
 */
void LCD_Init(const LCD_Config_t *config);

/**
 * @brief Limpia la pantalla y regresa el cursor a la posición (0,0).
 * @note Esta operación es lenta (requiere ~1.52ms de espera).
 */
void LCD_Clear(void);

/**
 * @brief Posiciona el cursor en una coordenada específica.
 * @param row Fila (0-1 para 16x2, 0-3 para 20x4).
 * @param col Columna (0-15 o 0-19 según el modelo).
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Imprime una cadena de caracteres terminada en null.
 * @param str Puntero a la cadena en memoria RAM.
 */
void LCD_Print(const char *str);

/**
 * @brief Escribe un carácter individual en la posición actual del cursor.
 * @param data Carácter ASCII o ID de carácter personalizado (0-7).
 */
void LCD_WriteChar(char data);

/**
 * @brief Configura el modo del cursor (encendido, apagado o parpadeo).
 * @param mode Selección basada en lcd_cursor_mode_t.
 */
void LCD_SetCursorMode(lcd_cursor_mode_t mode);

/**
 * @brief Desplaza el contenido visual sin alterar la memoria DDRAM.
 * @param direction Selección de desplazamiento (Izquierda/Derecha).
 */
void LCD_Shift(lcd_shift_dir_t direction);

/**
 * @brief Carga un patrón de píxeles en la CGRAM para crear caracteres especiales.
 * @param location Índice del carácter personalizado (valores de 0 a 7).
 * @param charmap Array de 8 bytes (solo los 5 bits menos significativos son usados).
 */
void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]);

#endif /* LCD_DRIVER_H_ */