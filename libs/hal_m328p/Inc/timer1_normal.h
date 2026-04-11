/**
 * @file timer1_normal.h
 * @brief Driver de Abstracción de Hardware (HAL) para el Timer 1 (16-bits) del ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este driver constituye el núcleo de temporización de alta resolución del sistema.
 * Gracias a sus 16 bits (0-65535), permite gestionar intervalos extensos y eventos de 
 * precisión. Incluye soporte para el módulo de Input Capture, ideal para la medición de 
 * señales externas, y dos canales de comparación para control de actuadores (como el Step Motor).
 */

#ifndef TIMER1_NORMAL_H_
#define TIMER1_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/** * @name Configuraciones de Reloj (Prescalers)
 * @{ 
 */

/**
 * @enum t1_prescaler_t
 * @brief Fuentes de reloj para el Timer 1.
 * @note Permite contar eventos externos mediante el pin T1 (PD5).
 */
typedef enum {
    T1_OFF          = 0, /**< Timer detenido. */
    T1_CLK_1        = 1, /**< F_CPU / 1. */
    T1_CLK_8        = 2, /**< F_CPU / 8. */
    T1_CLK_64       = 3, /**< F_CPU / 64. */
    T1_CLK_256      = 4, /**< F_CPU / 256. */
    T1_CLK_1024     = 5, /**< F_CPU / 1024. */
    T1_EXT_FALLING  = 6, /**< Reloj externo en pin T1 (PD5) - Flanco bajada. */
    T1_EXT_RISING   = 7  /**< Reloj externo en pin T1 (PD5) - Flanco subida. */
} t1_prescaler_t;

/** @} */

/** * @name Control de Salida (Compare Output Mode)
 * @{ 
 */

/**
 * @enum t1_comp_mode_t
 * @brief Configuración de los pines físicos OC1A (PB1) y OC1B (PB2).
 */
typedef enum {
    T1_PIN_DISCONNECT = 0, /**< Pin desconectado del hardware del Timer. */
    T1_PIN_TOGGLE     = 1, /**< Alterna estado del pin en cada Match. */
    T1_PIN_CLEAR      = 2, /**< Pone el pin en LOW en cada Match. */
    T1_PIN_SET        = 3  /**< Pone el pin en HIGH en cada Match. */
} t1_comp_mode_t;

/** @} */

/* --- API Pública de Capa 1 --- */

/**
 * @brief Inicializa el Timer 1 en Modo Normal de 16-bits.
 * @param prescaler Fuente de reloj (@ref t1_prescaler_t).
 * @param mode_a Comportamiento del pin OC1A (PB1).
 * @param mode_b Comportamiento del pin OC1B (PB2).
 */
void Timer1_Normal_Init(t1_prescaler_t prescaler, t1_comp_mode_t mode_a, t1_comp_mode_t mode_b);

/**
 * @brief Configura la Unidad de Captura de Entrada (Input Capture Unit).
 * @details Permite registrar el valor de TCNT1 en el registro ICR1 ante un evento en el pin ICP1 (PB0).
 * @param rising_edge true: captura en flanco de subida, false: flanco de bajada.
 * @param noise_canceller true: activa filtrado digital (requiere 4 ciclos de reloj estables).
 */
void Timer1_InputCapture_Init(bool rising_edge, bool noise_canceller);

/**
 * @brief Escribe un valor de 16 bits en el contador (TCNT1).
 * @note El hardware de AVR asegura la atomicidad mediante un registro temporal de 8 bits interno.
 * @param val Valor de 16 bits (0-65535).
 */
void Timer1_Write_Counter(uint16_t val);

/**
 * @brief Lee el valor actual del contador de 16 bits.
 * @return uint16_t Valor actual de TCNT1.
 */
uint16_t Timer1_Read_Counter(void);

/**
 * @brief Configura el punto de comparación para el Canal A (Alarma A).
 * @param val Valor de 16 bits para OCR1A.
 */
void Timer1_Set_AlarmA(uint16_t val);

/**
 * @brief Configura el punto de comparación para el Canal B (Alarma B).
 * @param val Valor de 16 bits para OCR1B.
 */
void Timer1_Set_AlarmB(uint16_t val);

/** * @name Gestión de Interrupciones
 * @{ 
 */

/** @brief Habilita la interrupción por desbordamiento (Overflow). */
static inline void Timer1_Enable_OVF_INT(void)  { TIMSK1 |= (1 << TOIE1); }

/** @brief Deshabilita la interrupción por desbordamiento. */
static inline void Timer1_Disable_OVF_INT(void) { TIMSK1 &= ~(1 << TOIE1); }

/** @brief Habilita la interrupción por Captura de Entrada (Input Capture). */
static inline void Timer1_Enable_IC_INT(void)   { TIMSK1 |= (1 << ICIE1); }

/** @} */

#endif /* TIMER1_NORMAL_H_ */