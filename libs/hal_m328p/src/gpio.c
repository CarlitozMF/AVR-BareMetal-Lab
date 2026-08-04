/**
 * @file gpio.c
 * @brief Implementación del Driver de Abstracción de Hardware (HAL) para GPIO.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este archivo define las instancias físicas de los puertos del ATmega328P
 * y proporciona la lógica de manipulación de bits para la Capa 1. Al utilizar
 * punteros a los registros de hardware, se garantiza una interfaz uniforme
 * para todas las capas superiores.
 */

#include "gpio.h"

/**
 * @name Instancias de Puertos
 * @brief Definición física de los objetos de puerto vinculados a los registros del MCU.
 * @{
 */
const gpio_port_t GPIO_B = { .DDR = &DDRB, .PORT = &PORTB, .PIN = &PINB };
const gpio_port_t GPIO_C = { .DDR = &DDRC, .PORT = &PORTC, .PIN = &PINC };
const gpio_port_t GPIO_D = { .DDR = &DDRD, .PORT = &PORTD, .PIN = &PIND };
/** @} */

/**
 * @brief Configura la dirección del pin (Entrada/Salida).
 * @param port Instancia del puerto (GPIO_x).
 * @param pin Número de pin (0-7).
 * @param mode Dirección deseada (GPIO_INPUT o GPIO_OUTPUT).
 * @details Modifica el registro DDR (Data Direction Register) del puerto seleccionado.
 */
void GPIO_InitPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode) {
    if (mode == GPIO_OUTPUT) {
        SET_BIT(*(port.DDR), pin);
    } else {
        CLR_BIT(*(port.DDR), pin);
    }
}

/**
 * @brief Escribe un nivel lógico en un pin configurado como salida.
 * @param port Instancia del puerto.
 * @param pin Número de pin (0-7).
 * @param state Nivel lógico (GPIO_HIGH o GPIO_LOW).
 * @details Modifica el registro PORT. Si el pin es entrada, escribir GPIO_HIGH 
 * activará la resistencia de pull-up interna.
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    if (state == GPIO_HIGH) {
        SET_BIT(*(port.PORT), pin);
    } else {
        CLR_BIT(*(port.PORT), pin);
    }
}

/**
 * @brief Invierte el estado lógico del pin.
 * @param port Instancia del puerto.
 * @param pin Número de pin (0-7).
 * @details Utiliza una operación XOR mediante la macro TOGGLE_BIT sobre el registro PORT.
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin) {
    TOGGLE_BIT(*(port.PORT), pin);
}

/**
 * @brief Lee y retorna el estado lógico actual de un pin.
 * @param port Instancia del puerto.
 * @param pin Número de pin (0-7).
 * @return gpio_state_t Estado leído (GPIO_HIGH si el bit está en 1, sino GPIO_LOW).
 * @details Accede al registro PIN, que refleja el estado físico real del pin 
 * independientemente de su configuración.
 */
gpio_state_t GPIO_ReadPin(gpio_port_t port, uint8_t pin) {
    if (BIT_IS_SET(*(port.PIN), pin)) {
        return GPIO_HIGH;
    } else {
        return GPIO_LOW;
    }
}