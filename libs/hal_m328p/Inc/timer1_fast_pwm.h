/**
 * @file timer1_fast_pwm.h
 * @brief Driver de Capa 1 (HAL) para Fast PWM en el Timer 1 (16-bits).
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo gestiona las salidas OC1A (PB1) y OC1B (PB2). Al usar el Modo 14,
 * permite una resolución y frecuencia variables mediante el registro ICR1, siendo 
 * ideal para servomotores o aplicaciones de audio PWM.
 */

#ifndef TIMER1_FAST_PWM_H_
#define TIMER1_FAST_PWM_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @enum t1_pwm_prescaler_t
 * @brief Fuentes de reloj para el Timer 1.
 */
typedef enum {
    T1_PWM_OFF        = 0, /**< Timer detenido */
    T1_PWM_CLK_1      = 1, /**< F_CPU / 1   */
    T1_PWM_CLK_8      = 2, /**< F_CPU / 8   */
    T1_PWM_CLK_64     = 3, /**< F_CPU / 64  */
    T1_PWM_CLK_256    = 4, /**< F_CPU / 256 */
    T1_PWM_CLK_1024   = 5, /**< F_CPU / 1024*/
    T1_PWM_EXT_FALL   = 6, /**< Reloj externo en T1 (PD5) flanco descendente */
    T1_PWM_EXT_RISE   = 7  /**< Reloj externo en T1 (PD5) flanco ascendente */
} t1_pwm_prescaler_t;

typedef enum {
    T1_PWM_CH_A, /**< Salida OC1A - Pin PB1 */
    T1_PWM_CH_B  /**< Salida OC1B - Pin PB2 */
} t1_pwm_channel_t;

typedef enum {
    T1_PWM_NON_INVERTING = 2, /**< High al inicio, Low al coincidir */
    T1_PWM_INVERTING     = 3  /**< Low al inicio, High al coincidir */
} t1_pwm_mode_t;

/* --- API Pública --- */

/**
 * @brief Inicializa el Timer 1 en modo Fast PWM (Modo 14).
 * @param prescaler Fuente de reloj.
 * @param top_value Valor para el registro ICR1 (define frecuencia).
 */
void Timer1_PWM_Fast_Init(t1_pwm_prescaler_t prescaler, uint16_t top_value);

/**
 * @brief Conecta el canal al pin físico (PB1 o PB2).
 * @param channel Canal A o B.
 * @param mode Inverting o Non-Inverting.
 */
void Timer1_PWM_Fast_EnableChannel(t1_pwm_channel_t channel, t1_pwm_mode_t mode);

/**
 * @brief Desconecta el canal del PWM.
 * @param channel Canal A o B.
 */
void Timer1_PWM_Fast_DisableChannel(t1_pwm_channel_t channel);

/**
 * @brief Ajusta el valor de comparación (Duty Cycle).
 * @param channel Canal A o B.
 * @param val Valor de 16 bits (0 a top_value).
 */
void Timer1_PWM_Fast_SetCompare(t1_pwm_channel_t channel, uint16_t val);

/* --- Gestión de Interrupciones --- */

void Timer1_PWM_IT_Overflow(uint8_t state);
void Timer1_PWM_IT_CompareMatch(t1_pwm_channel_t channel, uint8_t state);

#endif /* TIMER1_FAST_PWM_H_ */