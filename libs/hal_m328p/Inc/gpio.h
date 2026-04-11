/**
 * @file gpio.h
 * @brief Driver de Abstracción de Hardware (HAL) para GPIO del ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este driver constituye el núcleo de la Capa 1. Proporciona una interfaz
 * estructurada para la manipulación de pines, abstrayendo los registros DDR, PORT y PIN.
 * La implementación utiliza punteros volátiles para garantizar que el compilador
 * no optimice los accesos a los registros de hardware.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
#include "../../common/bits.h"

/** * @name Definiciones de Tipos
 * @{ 
 */

/**
 * @enum gpio_mode_t
 * @brief Configuración de dirección del flujo de datos del pin.
 */
typedef enum {
    GPIO_INPUT  = 0, /**< Pin configurado como entrada (Tri-state o Pull-up) */
    GPIO_OUTPUT = 1  /**< Pin configurado como salida (Push-pull) */
} gpio_mode_t;

/**
 * @enum gpio_state_t
 * @brief Estados lógicos digitales.
 */
typedef enum {
    GPIO_LOW  = 0, /**< Nivel lógico bajo (GND) */
    GPIO_HIGH = 1  /**< Nivel lógico alto (VCC) */
} gpio_state_t;

/**
 * @struct gpio_port_t
 * @brief Mapeo estructural de los registros asociados a un puerto.
 * @note El uso de punteros 'volatile uint8_t *' asegura el acceso directo a 
 * las direcciones de memoria de los registros de E/S.
 */
typedef struct {
    volatile uint8_t *DDR;  /**< Data Direction Register: Define entrada/salida */
    volatile uint8_t *PORT; /**< Data Register: Define el estado de salida o Pull-up */
    volatile uint8_t *PIN;  /**< Port Input Pins: Registro de lectura de entrada */
} gpio_port_t;

/** @} */

/**
 * @name Instancias de Hardware (Objetos de Puerto)
 * @brief Referencias globales a las estructuras de puerto físicas.
 * @{
 */
extern const gpio_port_t GPIO_B; /**< Representación estructural del Puerto B */
extern const gpio_port_t GPIO_C; /**< Representación estructural del Puerto C */
extern const gpio_port_t GPIO_D; /**< Representación estructural del Puerto D */
/** @} */


/* --- API Pública de Capa 1 --- */

/**
 * @brief Configura la dirección (entrada/salida) de un pin específico.
 * @param port Estructura del puerto objetivo (ej: GPIO_B).
 * @param pin Número de pin (0 a 7).
 * @param mode Modo de operación (GPIO_INPUT o GPIO_OUTPUT).
 * @note Si se configura como INPUT, el estado previo del bit en el registro 
 * PORT definirá si la resistencia de Pull-up interna está activa o no.
 */
void GPIO_InitPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode);

/**
 * @brief Establece un estado lógico (Alto/Bajo) en un pin de salida.
 * @param port Estructura del puerto.
 * @param pin Número de pin (0 a 7).
 * @param state Estado deseado (GPIO_HIGH o GPIO_LOW).
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, gpio_state_t state);

/**
 * @brief Invierte el estado lógico actual de un pin de salida.
 * @param port Estructura del puerto.
 * @param pin Número de pin (0 a 7).
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin);

/**
 * @brief Lee el nivel lógico presente en un pin configurado como entrada.
 * @param port Estructura del puerto.
 * @param pin Número de pin (0 a 7).
 * @return gpio_state_t Estado leído del hardware.
 */
gpio_state_t GPIO_ReadPin(gpio_port_t port, uint8_t pin);

#endif /* GPIO_H_ */