/**
 * @file step_motor_28BYJ48.h
 * @brief Driver genérico y encapsulado para el motor paso a paso 28BYJ-48.
 * @version 1.2
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Adaptación del driver de STM32 para AVR. Permite controlar múltiples 
 * motores mediante punteros a registros de puerto (PORTx), lo que otorga 
 * independencia total del hardware físico.
 */

#ifndef STEP_MOTOR_28BYJ48_H_
#define STEP_MOTOR_28BYJ48_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @enum Step_Mode_t
 * @brief Modos de excitación de las bobinas del motor.
 */
typedef enum {
    MODE_FULL_STEP = 0,    /**< 4 pasos por secuencia. Mayor torque, mayor vibración. */
    MODE_HALF_STEP = 1     /**< 8 pasos por secuencia. Mayor suavidad y precisión. */
} Step_Mode_t;

/**
 * @enum Step_Dir_t
 * @brief Sentido de giro del motor.
 */
typedef enum {
    STEP_CW  = 0,          /**< Sentido horario (Clockwise). */
    STEP_CCW = 1           /**< Sentido antihorario (Counter-Clockwise). */
} Step_Dir_t;

/**
 * @struct Stepper_t
 * @brief Estructura de control para el encapsulamiento de cada instancia del motor.
 * * @note En AVR, los puertos se pasan como punteros a registros volátiles de 8 bits.
 */
typedef struct {
    volatile uint8_t* GPIO_Ports[4]; /**< Direcciones de memoria de los puertos (&PORTA, &PORTB, etc). */
    uint8_t           GPIO_Pins[4];  /**< Números de pin asociados (0-7). */
    int8_t            current_step;  /**< Índice del paso actual en la secuencia activa. */
    Step_Mode_t       mode;          /**< Configuración de modo (Full/Half). */
    Step_Dir_t        direction;     /**< Sentido de giro actual. */
    bool              is_active;     /**< Flag de control (true: en movimiento, false: parado). */
} Stepper_t;

/* --- Interfaz del Driver --- */

/**
 * @brief Inicializa la estructura del motor y configura los pines como salida.
 * @param hstepper Puntero al handle del motor.
 * @param ports Arreglo de 4 punteros a puertos (uno por cada fase IN1-IN4).
 * @param pins Arreglo de 4 números de pin (correspondientes a IN1-IN4).
 * @param mode Modo de paso inicial (Full o Half).
 */
void Stepper_Init(Stepper_t* hstepper, volatile uint8_t* ports[], uint8_t pins[], Step_Mode_t mode);

/**
 * @brief Ejecuta un paso de la secuencia según el modo y la dirección configurada.
 * @details Esta función es no-bloqueante y debe llamarse periódicamente (ej: desde un Timer).
 * @param hstepper Puntero al handle del motor.
 */
void Stepper_Step_Sequential(Stepper_t* hstepper);

/**
 * @brief Configura el sentido de giro del motor.
 * @param hstepper Puntero al handle.
 * @param dir Nuevo sentido de giro (STEP_CW o STEP_CCW).
 */
void Stepper_Set_Direction(Stepper_t* hstepper, Step_Dir_t dir);

/**
 * @brief Detiene el motor y apaga todas las bobinas para ahorro de energía.
 * @param hstepper Puntero al handle.
 */
void Stepper_Stop(Stepper_t* hstepper);

#endif /* STEP_MOTOR_28BYJ48_H_ */