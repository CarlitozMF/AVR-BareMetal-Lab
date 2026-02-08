/**
 * @file timer.h
 * @brief Driver de Abstracción Genérico para Timers del ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Provee una interfaz unificada para el manejo de los tres timers 
 * internos del MCU, permitiendo generar un System Tick (Systick) de 1ms.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Enumeración de instancias de Timer disponibles.
 */
typedef enum {
    TIMER_0, /**< Timer de 8 bits: Comúnmente usado para Systick. */
    TIMER_1, /**< Timer de 16 bits: Ideal para alta precisión. */
    TIMER_2  /**< Timer de 8 bits: Con capacidad de operación asíncrona. */
} timer_instance_t;

/**
 * @brief Inicializa el timer seleccionado en modo CTC para generar un tick de 1ms.
 * * @param instance Instancia del timer a configurar (TIMER_0, TIMER_1 o TIMER_2).
 * @note Requiere que el cristal del sistema sea de 16MHz.
 */
void TIMER_Init(timer_instance_t instance);

/**
 * @brief Retorna los milisegundos transcurridos desde el arranque del sistema.
 * * @details Implementa una sección crítica para garantizar una lectura atómica
 * de la variable de 32 bits en una arquitectura de 8 bits.
 * @return uint32_t Cantidad de milisegundos acumulados.
 */
uint32_t get_tick(void);

/**
 * @brief Genera un retardo basado en el Systick del hardware.
 * * @param ms Tiempo de espera en milisegundos.
 * @note Es una función bloqueante, pero precisa y agnóstica al hardware.
 */
void delay_ms_tick(uint32_t ms);

#endif /* TIMER_H_ */