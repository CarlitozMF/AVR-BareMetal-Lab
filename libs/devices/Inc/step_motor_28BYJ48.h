/**
 * @file step_motor_28BYJ48.h
 * @brief Driver genérico y encapsulado para el motor paso a paso unipolar 28BYJ-48.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Adaptación de arquitectura profesional (tipo HAL de STM32) para AVR. 
 * Permite controlar múltiples instancias de motores de forma independiente mediante 
 * el uso de punteros a registros de puerto (PORTx), otorgando soberanía total 
 * sobre la distribución de pines en el hardware.
 */

#ifndef STEP_MOTOR_28BYJ48_H_
#define STEP_MOTOR_28BYJ48_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/** * @name Tipos de Configuración de Giro
 * @{ 
 */

/**
 * @enum Step_Mode_t
 * @brief Define los modos de excitación disponibles para las bobinas.
 */
typedef enum {
    MODE_FULL_STEP = 0,    /**< 4 pasos por secuencia. Prioriza el torque de retención. */
    MODE_HALF_STEP = 1     /**< 8 pasos por secuencia. Prioriza la suavidad y resolución angular. */
} Step_Mode_t;

/**
 * @enum Step_Dir_t
 * @brief Define el sentido de rotación del eje.
 */
typedef enum {
    STEP_CW  = 0,          /**< Sentido horario (Clockwise). */
    STEP_CCW = 1           /**< Sentido antihorario (Counter-Clockwise). */
} Step_Dir_t;

/** @} */

/**
 * @struct Stepper_t
 * @brief Estructura de control para el encapsulamiento de cada instancia del motor.
 * * @note El uso de punteros a registros 'volatile uint8_t*' permite que este objeto 
 * sea agnóstico a la posición física de los pines, facilitando el ruteo del PCB.
 */
typedef struct {
    volatile uint8_t* GPIO_Ports[4]; /**< Direcciones de memoria de los puertos (ej: &PORTB) para IN1..IN4. */
    uint8_t           GPIO_Pins[4];  /**< Números de pin (0-7) asociados a cada puerto de fase. */
    int8_t            current_step;  /**< Índice interno de la secuencia de pasos. */
    Step_Mode_t       mode;          /**< Modo de operación seleccionado (Full/Half). */
    Step_Dir_t        direction;     /**< Sentido de giro configurado. */
    bool              is_active;     /**< Flag de habilitación del movimiento. */
} Stepper_t;

/* --- API Pública de Control --- */

/**
 * @brief Inicializa la estructura del motor y configura automáticamente el DDRx.
 * * @param hstepper Puntero al handle (instancia) del motor.
 * @param ports Arreglo de 4 punteros a registros PORTx (IN1, IN2, IN3, IN4).
 * @param pins Arreglo de 4 números de pin (0-7) correspondientes a las fases.
 * @param mode Modo de paso inicial.
 */
void Stepper_Init(Stepper_t* hstepper, volatile uint8_t* ports[], uint8_t pins[], Step_Mode_t mode);

/**
 * @brief Ejecuta el siguiente paso lógico de la secuencia.
 * * @details Esta función es NO-BLOQUEANTE. Para controlar la velocidad (RPM), 
 * debe llamarse desde un Scheduler o una base de tiempo basada en Systick.
 * * @param hstepper Puntero al handle del motor.
 */
void Stepper_Step_Sequential(Stepper_t* hstepper);

/**
 * @brief Cambia el sentido de giro del motor.
 * * @param hstepper Puntero al handle.
 * @param dir Sentido deseado (STEP_CW o STEP_CCW).
 */
void Stepper_Set_Direction(Stepper_t* hstepper, Step_Dir_t dir);

/**
 * @brief Detiene el motor y fuerza un estado de baja potencia.
 * * @details Deshabilita el flag 'is_active' y pone todas las bobinas en estado LOW.
 * Es una medida de protección térmica necesaria para el integrado ULN2003.
 * * @param hstepper Puntero al handle.
 */
void Stepper_Stop(Stepper_t* hstepper);

#endif /* STEP_MOTOR_28BYJ48_H_ */