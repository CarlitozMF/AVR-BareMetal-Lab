#ifndef GPIO_H_
#define GPIO_H_

/**
 * @file gpio.h
 * @brief Driver de Abstracción para GPIO del ATmega328P.
 * @author Mamani Flores Carlos
 */

#include <avr/io.h>
#include "../../common/bits.h"

/* --- Definiciones de Tipos (Enums para Robustez) --- */

/**
 * @brief Direcciones posibles de un pin.
 */
typedef enum {
    GPIO_INPUT  = 0,
    GPIO_OUTPUT = 1
} gpio_mode_t;

/**
 * @brief Estados lógicos de un pin.
 */
typedef enum {
    GPIO_LOW  = 0,
    GPIO_HIGH = 1
} gpio_state_t;

/**
 * @brief Puertos disponibles en el ATmega328P.
 * Se usan punteros volátiles para apuntar a las direcciones de memoria de los registros.
 */
typedef struct {
    volatile uint8_t *DDR;
    volatile uint8_t *PORT;
    volatile uint8_t *PIN;
} gpio_port_t;

/* --- Mapeo de Registros Estructural --- */
// Esto permite usar GPIO_B->PORT en lugar de PORTB directamente
#define GPIO_B ((gpio_port_t){&DDRB, &PORTB, &PINB})
#define GPIO_C ((gpio_port_t){&DDRC, &PORTC, &PINC})
#define GPIO_D ((gpio_port_t){&DDRD, &PORTD, &PIND})

/* --- Prototipos de Funciones (Servicios de Capa 1) --- */

/**
 * @brief Configura un pin como entrada o salida.
 * @param port Estructura del puerto (GPIO_B, GPIO_C, etc.)
 * @param pin Número de pin (0-7)
 * @param mode GPIO_INPUT o GPIO_OUTPUT
 */
void GPIO_InitPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode);

/**
 * @brief Escribe un estado lógico en un pin.
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, gpio_state_t state);

/**
 * @brief Cambia el estado actual del pin.
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin);

/**
 * @brief Lee el estado actual de un pin de entrada.
 */
gpio_state_t GPIO_ReadPin(gpio_port_t port, uint8_t pin);

#endif /* GPIO_H_ */