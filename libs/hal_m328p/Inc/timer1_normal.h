/**
 * @file timer1_normal.h
 * @brief Driver de Abstracción para el Timer 1 (16-bits) del ATmega328P.
 * @version 1.0
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Provee una interfaz para el manejo del Timer 1 en Modo Normal.
 * Gracias a su resolución de 16 bits, este driver actúa como el "Motor Master" 
 * del sistema, gestionando la multitarea asíncrona mediante alarmas en los 
 * canales A y B para el control del motor paso a paso y el polling de botones.
 */

#ifndef TIMER1_NORMAL_H_
#define TIMER1_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/**
 * @enum t1_prescaler_t
 * @brief Fuentes de reloj para el Timer 1.
 */
typedef enum {
    T1_OFF          = 0, /**< Timer detenido */
    T1_CLK_1        = 1, /**< F_CPU / 1 */
    T1_CLK_8        = 2, /**< F_CPU / 8 */
    T1_CLK_64       = 3, /**< F_CPU / 64 */
    T1_CLK_256      = 4, /**< F_CPU / 256 */
    T1_CLK_1024     = 5, /**< F_CPU / 1024 */
    T1_EXT_FALLING  = 6, /**< Reloj externo en pin T1 (PD5) - Flanco bajada */
    T1_EXT_RISING   = 7  /**< Reloj externo en pin T1 (PD5) - Flanco subida */
} t1_prescaler_t;

/**
 * @enum t1_comp_mode_t
 * @brief Configuración de los pines OC1A (PB1) y OC1B (PB2).
 */
typedef enum {
    T1_PIN_DISCONNECT = 0,
    T1_PIN_TOGGLE     = 1,
    T1_PIN_CLEAR      = 2,
    T1_PIN_SET        = 3
} t1_comp_mode_t;

/**
 * @brief Inicializa el Timer 1 en Modo Normal.
 * @param prescaler Fuente de reloj o evento externo.
 * @param mode_a Configuración OC1A (PB1).
 * @param mode_b Configuración OC1B (PB2).
 */
void Timer1_Normal_Init(t1_prescaler_t prescaler, t1_comp_mode_t mode_a, t1_comp_mode_t mode_b);

/**
 * @brief Configura la Unidad de Captura de Entrada (Input Capture).
 * @param rising_edge true: captura en flanco de subida, false: bajada.
 * @param noise_canceller true: activa el filtrado digital (requiere 4 ciclos de reloj).
 */
void Timer1_InputCapture_Init(bool rising_edge, bool noise_canceller);

/** @brief Escribir valor de 16 bits en el contador TCNT1 */
void Timer1_Write_Counter(uint16_t val);

/** @brief Leer valor actual de 16 bits del contador */
uint16_t Timer1_Read_Counter(void);

/** @brief Configura Alarma A (OCR1A) */
void Timer1_Set_AlarmA(uint16_t val);

/** @brief Configura Alarma B (OCR1B) */
void Timer1_Set_AlarmB(uint16_t val);

/** @brief Habilitación de interrupciones de Overflow e Input Capture */
static inline void Timer1_Enable_OVF_INT(void)  { TIMSK1 |= (1 << TOIE1); }
static inline void Timer1_Disable_OVF_INT(void) { TIMSK1 &= ~(1 << TOIE1); }
static inline void Timer1_Enable_IC_INT(void)   { TIMSK1 |= (1 << ICIE1); }

#endif