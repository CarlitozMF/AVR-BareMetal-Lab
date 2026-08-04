/**
 * @file timer2_normal.h
 * @brief Driver de Abstracción de Hardware (HAL) para el Timer 2 (8-bits) del ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo provee una interfaz para el manejo del Timer 2 en Modo Normal.
 * A diferencia del Timer 0, el Timer 2 cuenta con una selección de prescalers 
 * más granular (incluyendo divisiones por 32 y 128) y la capacidad de operar de 
 * forma asíncrona mediante un cristal de 32.768kHz conectado a los pines TOSC1/2.
 */

#ifndef TIMER2_NORMAL_H_
#define TIMER2_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/** * @name Fuentes de Reloj Extendidas
 * @{ 
 */

/**
 * @enum t2_prescaler_t
 * @brief Define la fuente de reloj y divisores de frecuencia para el Timer 2.
 * @note Este periférico posee prescalers adicionales comparado con el Timer 0, 
 * permitiendo una mayor flexibilidad en la generación de frecuencias.
 */
typedef enum {
    T2_OFF          = 0, /**< Timer detenido. */
    T2_CLK_1        = 1, /**< F_CPU / 1. */
    T2_CLK_8        = 2, /**< F_CPU / 8. */
    T2_CLK_32       = 3, /**< F_CPU / 32 (Exclusivo de Timer 2). */
    T2_CLK_64       = 4, /**< F_CPU / 64. */
    T2_CLK_128      = 5, /**< F_CPU / 128 (Exclusivo de Timer 2). */
    T2_CLK_256      = 6, /**< F_CPU / 256. */
    T2_CLK_1024     = 7  /**< F_CPU / 1024. */
} t2_prescaler_t;

/** @} */

/** * @name Control de Salida (OC2x)
 * @{ 
 */

/**
 * @enum t2_comp_mode_t
 * @brief Configuración de los pines físicos OC2A (PB3) y OC2B (PD3).
 */
typedef enum {
    T2_PIN_DISCONNECT = 0, /**< Pin desconectado de la lógica del Timer. */
    T2_PIN_TOGGLE     = 1, /**< Alterna estado del pin al producirse el Match. */
    T2_PIN_CLEAR      = 2, /**< Pone el pin en LOW al producirse el Match. */
    T2_PIN_SET        = 3  /**< Pone el pin en HIGH al producirse el Match. */
} t2_comp_mode_t;

/** @} */

/* --- API Pública de Capa 1 --- */

/**
 * @brief Inicializa el hardware del Timer 2 en Modo Normal.
 * @param prescaler Selección del divisor de frecuencia (@ref t2_prescaler_t).
 * @param mode_a Comportamiento del pin OC2A (PB3).
 * @param mode_b Comportamiento del pin OC2B (PD3).
 */
void Timer2_Normal_Init(t2_prescaler_t prescaler, t2_comp_mode_t mode_a, t2_comp_mode_t mode_b);

/**
 * @brief Activa el funcionamiento asíncrono del Timer 2.
 * @details Configura el registro ASSR para que el Timer sea clockeado por un 
 * cristal externo de 32.768 kHz conectado a TOSC1 y TOSC2. 
 * @note Debe invocarse antes de la inicialización si se desea precisión de reloj.
 */
void Timer2_Enable_Async(void);

/** * @brief Escribe un valor en el contador de hardware (TCNT2).
 * @param val Valor de 8 bits.
 */
static inline void Timer2_Write_Counter(uint8_t val) { TCNT2 = val; }

/** * @brief Lee el valor actual del registro contador.
 * @return uint8_t Valor de TCNT2.
 */
static inline uint8_t Timer2_Read_Counter(void) { return TCNT2; }

/** * @brief Configura el punto de comparación para el Canal A (Alarma A).
 * @param val Valor de 8 bits para OCR2A.
 */
void Timer2_Set_AlarmA(uint8_t val);

/** * @brief Configura el punto de comparación para el Canal B (Alarma B).
 * @param val Valor de 8 bits para OCR2B.
 */
void Timer2_Set_AlarmB(uint8_t val);

/** * @name Gestión de Interrupciones
 * @{ 
 */

/** @brief Habilita la interrupción por desbordamiento (Overflow). */
static inline void Timer2_Enable_OVF_INT(void)  { TIMSK2 |= (1 << TOIE2); }

/** @brief Deshabilita la interrupción por desbordamiento. */
static inline void Timer2_Disable_OVF_INT(void) { TIMSK2 &= ~(1 << TOIE2); }

/** @} */

#endif /* TIMER2_NORMAL_H_ */