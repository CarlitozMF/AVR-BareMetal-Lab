/**
 * @file step_motor_28BYJ48.c
 * @brief Implementación del driver para el motor paso a paso unipolar 28BYJ-48.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este driver implementa el control de fases para motores unipolares 
 * utilizando una arquitectura de mapeo de pines flexible. Optimiza el uso de 
 * memoria RAM almacenando las secuencias de excitación en la memoria FLASH (PROGMEM).
 * La lógica es agnóstica al hardware, permitiendo distribuir las fases (IN1-IN4)
 * en cualquier puerto del MCU.
 */

#include <avr/pgmspace.h>
#include "step_motor_28BYJ48.h"

/** * @name Tablas de Secuencia en FLASH
 * @{
 * @brief Secuencias de excitación almacenadas en la memoria de programa (Flash).
 * @note Se accede mediante pgm_read_byte() para minimizar el consumo de SRAM.
 */

/** @brief Secuencia Full-Step (4 pasos). Proporciona el máximo torque de retención. */
const uint8_t FULL_STEP_TABLE[] PROGMEM = { 
    0x08, // 1000 (Fase 4)
    0x04, // 0100 (Fase 3)
    0x02, // 0010 (Fase 2)
    0x01  // 0001 (Fase 1)
};

/** @brief Secuencia Half-Step (8 pasos). Doble resolución y movimiento más fluido. */
const uint8_t HALF_STEP_TABLE[] PROGMEM = { 
    0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09 
};
/** @} */

/**
 * @brief Inicializa la estructura del motor y configura los pines como salidas.
 * @param hstepper Puntero a la instancia del motor (Handle).
 * @param ports Array de punteros a los puertos PORTx de cada fase.
 * @param pins Array con los números de pin respectivos (0-7).
 * @param mode Modo de paso inicial (FULL o HALF step).
 * * @note Implementa aritmética de punteros: En AVR, la dirección del registro DDRx 
 * es siempre (PORTx - 1). Esto permite configurar el modo salida sin punteros extra.
 */
void Stepper_Init(Stepper_t* hstepper, volatile uint8_t* ports[], uint8_t pins[], Step_Mode_t mode) {
    for(uint8_t i = 0; i < 4; i++) {
        hstepper->GPIO_Ports[i] = ports[i];
        hstepper->GPIO_Pins[i]  = pins[i];
        
        /* Configuración automática de DDRx como salida */
        volatile uint8_t* ddr_reg = hstepper->GPIO_Ports[i] - 1; 
        *ddr_reg |= (1 << hstepper->GPIO_Pins[i]); 
    }
    
    hstepper->current_step = 0;
    hstepper->mode = mode;
    hstepper->is_active = false;
    hstepper->direction = STEP_CW;
    
    Stepper_Stop(hstepper); // Estado seguro inicial (bobinas desenergizadas)
}

/**
 * @brief Ejecuta el siguiente paso de la secuencia de excitación.
 * @param hstepper Puntero a la instancia del motor.
 * * @details Calcula el índice circular según la dirección de giro y el modo. 
 * Recupera el patrón de bits desde FLASH y lo mapea a los puertos físicos.
 * Esta función debe ser llamada periódicamente (ej. desde un Timer) para 
 * controlar la velocidad de rotación.
 */
void Stepper_Step_Sequential(Stepper_t* hstepper) {
    if (!hstepper->is_active) return;

    uint8_t state_bits;
    uint8_t max_idx = (hstepper->mode == MODE_HALF_STEP) ? 8 : 4;

    /* 1. Cálculo del índice circular con protección de desbordamiento */
    if (hstepper->direction == STEP_CW) {
        hstepper->current_step = (hstepper->current_step + 1) % max_idx;
    } else {
        hstepper->current_step = (hstepper->current_step - 1 + max_idx) % max_idx;
    }

    /* 2. Recuperación de datos desde Memoria FLASH (PROGMEM) */
    if (hstepper->mode == MODE_HALF_STEP) {
        state_bits = pgm_read_byte(&HALF_STEP_TABLE[hstepper->current_step]);
    } else {
        state_bits = pgm_read_byte(&FULL_STEP_TABLE[hstepper->current_step]);
    }

    /* 3. Mapeo de bits de la tabla a los pines físicos configurados */
    for (uint8_t i = 0; i < 4; i++) {
        if (state_bits & (1 << i)) {
            *(hstepper->GPIO_Ports[i]) |= (1 << hstepper->GPIO_Pins[i]);
        } else {
            *(hstepper->GPIO_Ports[i]) &= ~(1 << hstepper->GPIO_Pins[i]);
        }
    }
}

/**
 * @brief Configura la dirección de giro del motor.
 * @param hstepper Puntero a la instancia.
 * @param dir Dirección deseada (STEP_CW o STEP_CCW).
 */
void Stepper_Set_Direction(Stepper_t* hstepper, Step_Dir_t dir) {
    hstepper->direction = dir;
}

/**
 * @brief Detiene el motor y desenergiza las bobinas.
 * @param hstepper Puntero a la instancia.
 * @details Es una función crítica de seguridad para prevenir el sobrecalentamiento 
 * del integrado driver (ej. ULN2003) y del propio motor durante periodos de inactividad.
 */
void Stepper_Stop(Stepper_t* hstepper) {
    hstepper->is_active = false;
    for (uint8_t i = 0; i < 4; i++) {
        *(hstepper->GPIO_Ports[i]) &= ~(1 << hstepper->GPIO_Pins[i]);
    }
}