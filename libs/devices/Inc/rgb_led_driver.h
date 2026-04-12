/**
 * @file rgb_led_driver.h
 * @brief Driver de Capa 2 (Device Driver) para el control genérico de LEDs RGB.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este módulo proporciona una abstracción completa para la gestión de LEDs RGB.
 * Está diseñado bajo el principio de "Inyección de Dependencias", siendo agnóstico 
 * a la plataforma de hardware (AVR, STM32, PIC). La vinculación con la Capa 1 (HAL)
 * se realiza mediante punteros a funciones (callbacks) definidos durante la inicialización.
 */

#ifndef RGB_LED_DRIVER_H_
#define RGB_LED_DRIVER_H_

#include <stdint.h>

/** * @brief Definición de puntero a función para la manipulación del Duty Cycle.
 * @param duty Valor de 8 bits (0-255) que representa el ciclo de trabajo.
 */
typedef void (*pwm_set_duty_t)(uint8_t duty);

/**
 * @enum rgb_type_t
 * @brief Define la configuración eléctrica del LED RGB.
 */
typedef enum {
    RGB_CATHODE_COMMON, /**< Cátodo común: El pin PWM entrega VCC (Lógica directa). */
    RGB_ANODE_COMMON    /**< Ánodo común: El pin PWM entrega GND (Lógica invertida). */
} rgb_type_t;

/**
 * @enum RGB_PresetColor_t
 * @brief Paleta de colores predefinidos para acceso rápido desde la aplicación.
 */
typedef enum {
    COLOR_RED,      /**< Rojo puro */
    COLOR_GREEN,    /**< Verde puro */
    COLOR_BLUE,     /**< Azul puro */
    COLOR_YELLOW,   /**< Mezcla Rojo + Verde */
    COLOR_CYAN,     /**< Mezcla Verde + Azul */
    COLOR_MAGENTA,  /**< Mezcla Rojo + Azul */
    COLOR_WHITE,    /**< Mezcla R+G+B (Blanco) */
    COLOR_OFF       /**< Todos los canales al 0% */
} RGB_PresetColor_t;

/**
 * @struct RGB_LED_t
 * @brief Estructura de control para una instancia de LED RGB.
 * * @details Contiene tanto los punteros a las funciones de hardware como
 * el estado actual de los canales para permitir operaciones de lectura/escritura.
 */
typedef struct {
    pwm_set_duty_t set_red;   /**< Callback para el canal Rojo. */
    pwm_set_duty_t set_green; /**< Callback para el canal Verde. */
    pwm_set_duty_t set_blue;  /**< Callback para el canal Azul. */
    
    rgb_type_t type;          /**< Configuración de hardware (Ánodo/Cátodo). */
    uint8_t max_brightness;   /**< Límite superior de intensidad (0-255). */
    
    uint8_t current_r;        /**< Intensidad roja actual en memoria. */
    uint8_t current_g;        /**< Intensidad verde actual en memoria. */
    uint8_t current_b;        /**< Intensidad azul actual en memoria. */
} RGB_LED_t;

/* --- API Pública --- */

/**
 * @brief Inicializa el objeto LED RGB y vincula los callbacks de hardware.
 * * @param led Puntero a la estructura de control del LED.
 * @param type Polaridad del LED (@ref rgb_type_t).
 * @param max_br Brillo máximo permitido para esta instancia.
 * @param f_red Función de la Capa 1 para el canal Rojo.
 * @param f_green Función de la Capa 1 para el canal Verde.
 * @param f_blue Función de la Capa 1 para el canal Azul.
 */
void RGB_Init(RGB_LED_t *led, rgb_type_t type, uint8_t max_br, 
              pwm_set_duty_t f_red, pwm_set_duty_t f_green, pwm_set_duty_t f_blue);

/**
 * @brief Establece un color RGB específico de forma inmediata.
 * * @details Esta función gestiona la limitación de brillo máximo y la inversión 
 * de lógica en caso de ser un LED de Ánodo Común.
 * * @param led Puntero a la instancia del LED.
 * @param r Intensidad del canal Rojo (0-255).
 * @param g Intensidad del canal Verde (0-255).
 * @param b Intensidad del canal Azul (0-255).
 */
void RGB_Set_Color_Direct(RGB_LED_t *led, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Aplica un color predefinido desde la paleta de presets.
 * * @param led Puntero a la instancia del LED.
 * @param color Identificador del color deseado (@ref RGB_PresetColor_t).
 */
void RGB_Set_Preset(RGB_LED_t *led, RGB_PresetColor_t color);

#endif /* RGB_LED_DRIVER_H_ */