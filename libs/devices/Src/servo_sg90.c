/**
 * @file servo_sg90.c
 * @brief Implementación del driver genérico para servomotores SG90.
 * @author Mamani Flores Carlos - CarlitozMF
 * @date 2026
 * * @details Este driver implementa una lógica de control angular independiente del hardware.
 * Utiliza "Inyección de Dependencias" mediante punteros a función, lo que permite:
 * 1. Manejar múltiples servos simultáneamente (instanciación).
 * 2. Ser agnóstico al Timer utilizado (8 o 16 bits).
 * 3. Ejecutar el mapeo de grados a ticks mediante aritmética de precisión sin float.
 */

#include "servo_sg90.h"

/**
 * @brief Inicializa una instancia de servo con sus parámetros de calibración.
 * * @param instance Puntero a la estructura de datos del servo (Servo_t).
 * @param min Ticks de Timer correspondientes al ancho de pulso para 0° (típicamente 1ms).
 * @param max Ticks de Timer correspondientes al ancho de pulso para 180° (típicamente 2ms).
 * @param callback Puntero a la función de la Capa 1 que escribe en el registro OCR.
 * * @note Esta función no configura los registros del Timer. La inicialización del
 * periférico debe realizarse previamente en el Hardware Mapping.
 */
void Servo_Init(Servo_t *instance, uint16_t min, uint16_t max, servo_write_hw_ptr callback) {
    /* Verificación de punteros nulos para evitar fallos de segmentación */
    if (instance == 0 || callback == 0) return;

    /* Configuración de los límites de la instancia */
    instance->min_ticks = min;
    instance->max_ticks = max;
    instance->write_hw = callback;
    Servo_SetAngle(instance, 0);
}

/**
 * @brief Establece el ángulo del servo calculando el duty cycle correspondiente.
 * * @param instance Puntero a la instancia del servo que se desea mover.
 * @param angle Valor angular en grados (0 a 180).
 * * @details 
 * El mapeo se basa en la ecuación de la recta: y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
 * Donde:
 * - x es el ángulo deseado.
 * - y son los ticks resultantes para el OCR.
 * * @warning La función utiliza promoción a 32 bits para el cálculo intermedio. 
 * Esto es vital si se usan Timers de 16 bits (como el Timer 1) donde (angle * range)
 * puede superar fácilmente el límite de 65535 de un uint16_t.
 */
void Servo_SetAngle(Servo_t *instance, uint8_t angle) {
    /* Verificación de integridad de la instancia y el callback de hardware */
    if (instance == 0 || instance->write_hw == 0) return;

    /* Saturación de límites: el SG90 físico no puede superar los 180 grados */
    if (angle > 180) {
        angle = 180;
    }

    /**
     * @brief Cálculo de ticks mediante aritmética de punto fijo.
     * * 1. Calculamos el rango dinámico (delta de ticks).
     * 2. Promocionamos 'angle' a uint32_t para asegurar que la multiplicación
     * no desborde antes de la división por 180.
     * 3. Sumamos el offset mínimo para posicionar en el cuadrante correcto.
     */
    uint32_t range = (uint32_t)(instance->max_ticks - instance->min_ticks);
    uint32_t intermediate_calc = ((uint32_t)angle * range) / 180;
    uint16_t target_ticks = (uint16_t)(intermediate_calc + instance->min_ticks);

    /**
     * @brief Inyección de hardware.
     * Se invoca la función callback pasando el valor de ticks calculado.
     * Esto permite que esta misma lógica mueva un servo en OCR1A, OCR1B o incluso Soft-PWM.
     */
    instance->write_hw(target_ticks);
}