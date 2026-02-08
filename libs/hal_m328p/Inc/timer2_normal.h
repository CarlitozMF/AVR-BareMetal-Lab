/**
 * @file timer2_normal.h
 * @brief Driver de Abstracción para el Timer 2 (8-bits) del ATmega328P.
 * @version 1.0
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Provee una interfaz para el manejo del Timer 2 en Modo Normal.
 * Este driver aprovecha las características asíncronas y los prescalers extendidos 
 * del Timer 2 para tareas de modulación por software (PWM) o generación de 
 * frecuencias de alarma, operando de forma independiente al reloj del sistema.
 */

#ifndef TIMER2_NORMAL_H_
#define TIMER2_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @enum t2_prescaler_t
 * @brief Fuentes de reloj para Timer 2. 
 * Posee más opciones que el Timer 0 (32 y 128).
 */
typedef enum {
    T2_OFF          = 0, /**< Timer detenido */
    T2_CLK_1        = 1, /**< F_CPU / 1 */
    T2_CLK_8        = 2, /**< F_CPU / 8 */
    T2_CLK_32       = 3, /**< F_CPU / 32 */
    T2_CLK_64       = 4, /**< F_CPU / 64 */
    T2_CLK_128      = 5, /**< F_CPU / 128 */
    T2_CLK_256      = 6, /**< F_CPU / 256 */
    T2_CLK_1024     = 7  /**< F_CPU / 1024 */
} t2_prescaler_t;

/**
 * @enum t2_comp_mode_t
 * @brief Configuración de los pines OC2A (PB3) y OC2B (PD3).
 */
typedef enum {
    T2_PIN_DISCONNECT = 0,
    T2_PIN_TOGGLE     = 1,
    T2_PIN_CLEAR      = 2,
    T2_PIN_SET        = 3
} t2_comp_mode_t;

/**
 * @brief Inicializa el Timer 2 en Modo Normal.
 * @param prescaler Divisor de frecuencia.
 * @param mode_a Configuración OC2A (PB3).
 * @param mode_b Configuración OC2B (PD3).
 */
void Timer2_Normal_Init(t2_prescaler_t prescaler, t2_comp_mode_t mode_a, t2_comp_mode_t mode_b);

/**
 * @brief Activa el modo asíncrono para usar un cristal externo de 32.768kHz.
 * @note Se debe llamar antes de Init si se requiere un RTC.
 */
void Timer2_Enable_Async(void);

/** @brief Escribir valor en el contador TCNT2 */
static inline void Timer2_Write_Counter(uint8_t val) { TCNT2 = val; }

/** @brief Leer valor actual del contador */
static inline uint8_t Timer2_Read_Counter(void) { return TCNT2; }

/** @brief Configura Alarma A (OCR2A) */
void Timer2_Set_AlarmA(uint8_t val);

/** @brief Configura Alarma B (OCR2B) */
void Timer2_Set_AlarmB(uint8_t val);

/** @brief Control de interrupción por Overflow */
static inline void Timer2_Enable_OVF_INT(void)  { TIMSK2 |= (1 << TOIE2); }
static inline void Timer2_Disable_OVF_INT(void) { TIMSK2 &= ~(1 << TOIE2); }

#endif