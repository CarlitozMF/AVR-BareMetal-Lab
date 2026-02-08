/**
 * @file timer0_normal.h
 * @brief Driver de Abstracción para el Timer 0 (8-bits) del ATmega328P.
 * @version 1.0
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Provee una interfaz para el manejo del Timer 0 en Modo Normal. 
 */

#ifndef TIMER0_NORMAL_H_
#define TIMER0_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @enum t0_prescaler_t
 * @brief Fuentes de reloj y contadores de eventos externos.
 */
typedef enum {
    T0_OFF          = 0, /**< Timer detenido */
    T0_CLK_1        = 1, /**< F_CPU / 1 */
    T0_CLK_8        = 2, /**< F_CPU / 8 */
    T0_CLK_64       = 3, /**< F_CPU / 64 */
    T0_CLK_256      = 4, /**< F_CPU / 256 */
    T0_CLK_1024     = 5, /**< F_CPU / 1024 */
    T0_EXT_FALLING  = 6, /**< Reloj externo en pin T0 (PD4) - Flanco bajada */
    T0_EXT_RISING   = 7  /**< Reloj externo en pin T0 (PD4) - Flanco subida */
} t0_prescaler_t;

/**
 * @enum t0_comp_mode_t
 * @brief Configuración de los pines físicos OC0A (PD6) y OC0B (PD5).
 */
typedef enum {
    T0_PIN_DISCONNECT = 0, /**< GPIO Normal */
    T0_PIN_TOGGLE     = 1, /**< Toggle físico al coincidir (Match) */
    T0_PIN_CLEAR      = 2, /**< Pin a LOW al coincidir */
    T0_PIN_SET        = 3  /**< Pin a HIGH al coincidir */
} t0_comp_mode_t;

/**
 * @brief Inicializa el Timer 0 en Modo Normal.
 * @param prescaler Fuente de reloj o pin T0.
 * @param mode_a Configuración de salida para OC0A (PD6).
 * @param mode_b Configuración de salida para OC0B (PD5).
 */
void Timer0_Normal_Init(t0_prescaler_t prescaler, t0_comp_mode_t mode_a, t0_comp_mode_t mode_b);

/** @brief Carga el valor del contador (Precarga para ajustar el tiempo de Overflow) */
static inline void Timer0_Write_Counter(uint8_t val) { TCNT0 = val; }

/** @brief Lee el valor actual del contador */
static inline uint8_t Timer0_Read_Counter(void) { return TCNT0; }

/** @brief Configura la Alarma A (Match) y habilita su interrupción */
void Timer0_Set_AlarmA(uint8_t val);

/** @brief Configura la Alarma B (Match) y habilita su interrupción */
void Timer0_Set_AlarmB(uint8_t val);

/** @brief Habilita la interrupción por desbordamiento (Overflow) */
static inline void Timer0_Enable_OVF_INT(void) { TIMSK0 |= (1 << TOIE0); }

/** @brief Deshabilita la interrupción por desbordamiento (Overflow) */
static inline void Timer0_Disable_OVF_INT(void) { TIMSK0 &= ~(1 << TOIE0); }

#endif