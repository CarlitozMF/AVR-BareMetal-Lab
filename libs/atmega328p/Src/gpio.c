/**
 * @file gpio.c
 * @brief Implementación del Driver de GPIO para ATmega328P.
 * @author Mamani Flores Carlos
 */

#include "gpio.h"

/**
 * @brief Configura la dirección del pin (Entrada/Salida)
 */
void GPIO_InitPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode) {
    if (mode == GPIO_OUTPUT) {
        SET_BIT(*(port.DDR), pin);
    } else {
        CLR_BIT(*(port.DDR), pin);
    }
}

/**
 * @brief Escribe un nivel lógico en el pin
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    if (state == GPIO_HIGH) {
        SET_BIT(*(port.PORT), pin);
    } else {
        CLR_BIT(*(port.PORT), pin);
    }
}

/**
 * @brief Alterna el estado del pin (Toggle)
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin) {
    TOGGLE_BIT(*(port.PORT), pin);
}

/**
 * @brief Lee y retorna el estado de un pin
 */
gpio_state_t GPIO_ReadPin(gpio_port_t port, uint8_t pin) {
    if (BIT_IS_SET(*(port.PIN), pin)) {
        return GPIO_HIGH;
    } else {
        return GPIO_LOW;
    }
}