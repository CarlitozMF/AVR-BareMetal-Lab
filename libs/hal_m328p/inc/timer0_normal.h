/**
 * @file timer0_normal.h
 * @brief Driver de Abstracción de Hardware (HAL) para el Timer 0 (8-bits) del ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Provee una interfaz estructurada para el manejo del Timer 0 en Modo Normal. 
 * Este driver permite la gestión de tiempos mediante desbordamiento (Overflow) y la 
 * configuración de eventos determinísticos por hardware en los canales de comparación A y B.
 */

#ifndef TIMER0_NORMAL_H_
#define TIMER0_NORMAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/** * @name Configuraciones de Reloj
 * @{ 
 */

/**
 * @enum t0_prescaler_t
 * @brief Define las fuentes de reloj y los divisores de frecuencia (Prescalers).
 * @note Incluye opciones para contadores de eventos externos mediante el pin T0 (PD4).
 */
typedef enum {
    T0_OFF          = 0, /**< Timer detenido. */
    T0_CLK_1        = 1, /**< Reloj de sistema sin división (F_CPU / 1). */
    T0_CLK_8        = 2, /**< F_CPU / 8. */
    T0_CLK_64       = 3, /**< F_CPU / 64. */
    T0_CLK_256      = 4, /**< F_CPU / 256. */
    T0_CLK_1024     = 5, /**< F_CPU / 1024. */
    T0_EXT_FALLING  = 6, /**< Contador de eventos: Flanco de bajada en pin T0 (PD4). */
    T0_EXT_RISING   = 7  /**< Contador de eventos: Flanco de subida en pin T0 (PD4). */
} t0_prescaler_t;

/** @} */

/** * @name Control de Salida Física (OC0x)
 * @{ 
 */

/**
 * @enum t0_comp_mode_t
 * @brief Configuración del comportamiento de los pines OC0A (PD6) y OC0B (PD5).
 * @details Define la respuesta automática del hardware ante una coincidencia de comparación (Match).
 */
typedef enum {
    T0_PIN_DISCONNECT = 0, /**< Pin desconectado del Timer (Funcionamiento GPIO estándar). */
    T0_PIN_TOGGLE     = 1, /**< Alterna el estado físico del pin al producirse el Match. */
    T0_PIN_CLEAR      = 2, /**< Pone el pin en estado LOW al producirse el Match. */
    T0_PIN_SET        = 3  /**< Pone el pin en estado HIGH al producirse el Match. */
} t0_comp_mode_t;

/** @} */

/* --- API Pública de Capa 1 --- */

/**
 * @brief Inicializa el hardware del Timer 0 en Modo Normal.
 * @param prescaler Selección de la fuente de reloj (@ref t0_prescaler_t).
 * @param mode_a Configuración de salida para el pin OC0A (PD6).
 * @param mode_b Configuración de salida para el pin OC0B (PD5).
 */
void Timer0_Normal_Init(t0_prescaler_t prescaler, t0_comp_mode_t mode_a, t0_comp_mode_t mode_b);

/** * @brief Carga un valor en el registro de conteo (TCNT0).
 * @details Útil para ajustar el tiempo de desbordamiento (Precarga).
 * @param val Valor de 8 bits a cargar.
 */
static inline void Timer0_Write_Counter(uint8_t val) { TCNT0 = val; }

/** * @brief Lee el valor actual del contador de hardware.
 * @return uint8_t Valor actual de TCNT0.
 */
static inline uint8_t Timer0_Read_Counter(void) { return TCNT0; }

/** * @brief Configura la Alarma A (Match) y habilita su interrupción.
 * @param val Valor de comparación (0-255).
 */
void Timer0_Set_AlarmA(uint8_t val);

/** * @brief Configura la Alarma B (Match) y habilita su interrupción.
 * @param val Valor de comparación (0-255).
 */
void Timer0_Set_AlarmB(uint8_t val);

/** * @brief Habilita la interrupción por desbordamiento (Overflow).
 * @note El vector de interrupción asociado es TIMER0_OVF_vect.
 */
static inline void Timer0_Enable_OVF_INT(void) { TIMSK0 |= (1 << TOIE0); }

/** * @brief Deshabilita la interrupción por desbordamiento.
 */
static inline void Timer0_Disable_OVF_INT(void) { TIMSK0 &= ~(1 << TOIE0); }

#endif /* TIMER0_NORMAL_H_ */