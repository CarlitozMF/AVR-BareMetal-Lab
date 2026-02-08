/**
 * @file step_motor_28BYJ48.c
 * @brief Implementación del driver genérico para AVR ATmega328P.
 */

#include "step_motor_28BYJ48.h"

/** * @brief Tabla de secuencia Full-Step (4 pasos).
 * @details Solo una fase activa a la vez para máximo ahorro de corriente o 
 * dos fases para máximo torque (según implementación).
 */
static const uint8_t FULL_STEP_TABLE[4] = { 0x08, 0x04, 0x02, 0x01 };

/** * @brief Tabla de secuencia Half-Step (8 pasos).
 * @details Intercala estados de una y dos fases para mayor resolución.
 */
static const uint8_t HALF_STEP_TABLE[8] = { 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09 };

void Stepper_Init(Stepper_t* hstepper, volatile uint8_t* ports[], uint8_t pins[], Step_Mode_t mode) {
    for(uint8_t i = 0; i < 4; i++) {
        hstepper->GPIO_Ports[i] = ports[i];
        hstepper->GPIO_Pins[i]  = pins[i];
        
        /**
         * @note Aritmética de registros: En la arquitectura AVR, el registro DDRx 
         * siempre se encuentra en la dirección de memoria inmediatamente anterior a PORTx.
         */
        volatile uint8_t* ddr_reg = hstepper->GPIO_Ports[i] - 1; 
        *ddr_reg |= (1 << hstepper->GPIO_Pins[i]); // Configura pin como SALIDA
    }
    
    hstepper->current_step = 0;
    hstepper->mode = mode;
    hstepper->is_active = false;
    hstepper->direction = STEP_CW;
    
    Stepper_Stop(hstepper); // Asegura que el motor inicie apagado
}

void Stepper_Step_Sequential(Stepper_t* hstepper) {
    if (!hstepper->is_active) return;

    uint8_t state_bits;
    uint8_t max_idx = (hstepper->mode == MODE_HALF_STEP) ? 8 : 4;

    /* Gestión del índice de la tabla según dirección */
    if (hstepper->direction == STEP_CW) {
        hstepper->current_step = (hstepper->current_step + 1) % max_idx;
    } else {
        hstepper->current_step = (hstepper->current_step - 1 + max_idx) % max_idx;
    }

    /* Obtención de los bits de fase desde la tabla correspondiente */
    state_bits = (hstepper->mode == MODE_HALF_STEP) ? 
                  HALF_STEP_TABLE[hstepper->current_step] : 
                  FULL_STEP_TABLE[hstepper->current_step];

    /**
     * @brief Aplicación de los estados a los puertos físicos.
     * @details Se itera sobre los 4 pines configurados en el handle, permitiendo 
     * que cada fase esté en un puerto o pin arbitrario.
     */
    for (uint8_t i = 0; i < 4; i++) {
        if (state_bits & (1 << i)) {
            *(hstepper->GPIO_Ports[i]) |= (1 << hstepper->GPIO_Pins[i]);
        } else {
            *(hstepper->GPIO_Ports[i]) &= ~(1 << hstepper->GPIO_Pins[i]);
        }
    }
}

void Stepper_Set_Direction(Stepper_t* hstepper, Step_Dir_t dir) {
    hstepper->direction = dir;
}

void Stepper_Stop(Stepper_t* hstepper) {
    hstepper->is_active = false;
    /* Apagado físico de todas las bobinas para evitar sobrecalentamiento */
    for (uint8_t i = 0; i < 4; i++) {
        *(hstepper->GPIO_Ports[i]) &= ~(1 << hstepper->GPIO_Pins[i]);
    }
}