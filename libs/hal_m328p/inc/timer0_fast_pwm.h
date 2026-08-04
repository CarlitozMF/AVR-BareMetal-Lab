/**
 * @file timer0_fast_pwm.h
 * @brief Driver de Capa 1 (HAL) completo para Fast PWM en Timer 0 (8-bits).
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo abstrae el hardware del Timer 0 para permitir configuraciones
 * flexibles de PWM. Soporta modos Inverting y Non-Inverting, gestión de canales
 * independiente y control de interrupciones para sincronización de tareas.
 */

#ifndef TIMER0_FAST_PWM_H_
#define TIMER0_FAST_PWM_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @enum t0_pwm_prescaler_t
 * @brief Fuentes de reloj y prescalers según Table 14-9 del datasheet.
 */
typedef enum {
    T0_PWM_OFF        = 0, /**< Timer detenido */
    T0_PWM_CLK_1      = 1, /**< F_CPU / 1   (62.5 kHz @16MHz) */
    T0_PWM_CLK_8      = 2, /**< F_CPU / 8   (7.81 kHz) */
    T0_PWM_CLK_64     = 3, /**< F_CPU / 64  (976.5 Hz) */
    T0_PWM_CLK_256    = 4, /**< F_CPU / 256 (244.1 Hz) */
    T0_PWM_CLK_1024   = 5, /**< F_CPU / 1024(61.03 Hz) */
    T0_PWM_EXT_FALL   = 6, /**< Reloj externo en T0 (PD4) flanco descendente */
    T0_PWM_EXT_RISE   = 7  /**< Reloj externo en T0 (PD4) flanco ascendente */
} t0_pwm_prescaler_t;

/**
 * @enum t0_pwm_channel_t
 * @brief Canales de comparación del Timer 0.
 */
typedef enum {
    T0_PWM_CH_A, /**< Salida OC0A - Pin PD6 */
    T0_PWM_CH_B  /**< Salida OC0B - Pin PD5 */
} t0_pwm_channel_t;

/**
 * @enum t0_pwm_mode_t
 * @brief Modos de conmutación del pin según Table 14-3 del datasheet.
 */
typedef enum {
    T0_PWM_NON_INVERTING = 2, /**< High al inicio, Low al coincidir (Duty directo) */
    T0_PWM_INVERTING     = 3  /**< Low al inicio, High al coincidir (Duty inverso) */
} t0_pwm_mode_t;

/* --- API de Configuración Base --- */

/**
 * @brief Inicializa el hardware del Timer 0 para Fast PWM (Modo 3).
 * @param prescaler Selección de la fuente de reloj.
 */
void Timer0_PWM_Init(t0_pwm_prescaler_t prescaler);

/* --- API de Gestión de Canales --- */

/**
 * @brief Conecta un canal PWM al pin físico correspondiente.
 * @param channel Canal A o B.
 * @param mode Selección entre modo Inverting o Non-Inverting.
 */
void Timer0_PWM_EnableChannel(t0_pwm_channel_t channel, t0_pwm_mode_t mode);

/**
 * @brief Desconecta el canal del pin físico, devolviendo el control al GPIO.
 * @param channel Canal A o B.
 */
void Timer0_PWM_DisableChannel(t0_pwm_channel_t channel);

/**
 * @brief Actualiza el ancho de pulso (Duty Cycle).
 * @param channel Canal A o B.
 * @param duty Valor de 0 a 255.
 */
void Timer0_PWM_SetDuty(t0_pwm_channel_t channel, uint8_t duty);

/* --- API de Funciones Avanzadas --- */

/**
 * @brief Habilita/Deshabilita la interrupción por desbordamiento (Overflow).
 * @details Útil para ejecutar código al final de cada ciclo PWM.
 * @param state 1 para habilitar, 0 para deshabilitar.
 */
void Timer0_PWM_IT_Overflow(uint8_t state);

/**
 * @brief Habilita/Deshabilita la interrupción por comparación de canal.
 * @param channel Canal A o B.
 * @param state 1 para habilitar, 0 para deshabilitar.
 */
void Timer0_PWM_IT_CompareMatch(t0_pwm_channel_t channel, uint8_t state);

#endif /* TIMER0_FAST_PWM_H_ */