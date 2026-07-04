/**
 * @file servo_sg90.h
 * @brief Driver genérico para el control de servomotores SG90.
 * @author Mamani Flores Carlos
 * @details Este driver permite gestionar múltiples instancias de servos
 * mediante inyección de dependencias, siendo independiente del Timer utilizado.
 */

#ifndef SERVO_SG90_H_
#define SERVO_SG90_H_

#include <stdint.h>

/**
 * @brief Definición del puntero a función para la capa de abstracción de hardware.
 * @param ticks Valor de comparación (OCR) calculado según la resolución del Timer.
 */
typedef void (*servo_write_hw_ptr)(uint16_t ticks);

/**
 * @brief Estructura de control para una instancia de Servo.
 * @details Almacena los límites de calibración y el enlace al hardware.
 */
typedef struct {
    uint16_t min_ticks;         /**< Ticks para posición 0° (aprox 1.0ms) */
    uint16_t max_ticks;         /**< Ticks para posición 180° (aprox 2.0ms) */
    servo_write_hw_ptr write_hw; /**< Callback hacia la Capa 1 (Wrapper del Timer) */
} Servo_t;

/**
 * @brief Inicializa una instancia de servo.
 * @param instance Puntero a la estructura Servo_t.
 * @param min Ticks correspondientes al ancho de pulso mínimo.
 * @param max Ticks correspondientes al ancho de pulso máximo.
 * @param callback Función de hardware que escribe en el registro OCR.
 */
void Servo_Init(Servo_t *instance, uint16_t min, uint16_t max, servo_write_hw_ptr callback);

/**
 * @brief Establece el ángulo de un servo específico.
 * @param instance Puntero a la instancia del servo a mover.
 * @param angle Ángulo en grados (rango 0 - 180).
 */
void Servo_SetAngle(Servo_t *instance, uint8_t angle);

#endif /* SERVO_SG90_H_ */