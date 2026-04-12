/**
 * @file timer2_fast_pwm.h
 * @brief Driver de Capa 1 (HAL) para Fast PWM en el Timer 2 (8-bits).
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo gestiona las salidas OC2A (PB3) y OC2B (PD3). El Timer 2 es 
 * especialmente útil por su prescaler extendido (Table 17-9), que permite un ajuste 
 * mucho más fino de la frecuencia PWM en comparación con el Timer 0, ideal para 
 * evitar ruidos audibles en drivers de potencia o parpadeos en LEDs.
 */

#ifndef TIMER2_FAST_PWM_H_
#define TIMER2_FAST_PWM_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @enum t2_pwm_prescaler_t
 * @brief Selección de la fuente de reloj y prescaler para el Timer 2.
 * @note Valores extraídos de la Table 17-9 (Clock Select Bit Description).
 * Las frecuencias están calculadas para un F_CPU de 16 MHz.
 */
typedef enum {
    T2_PWM_OFF        = 0, /**< Timer detenido. */
    T2_PWM_CLK_1      = 1, /**< F_CPU / 1   (62.5 kHz) */
    T2_PWM_CLK_8      = 2, /**< F_CPU / 8   (7.81 kHz) */
    T2_PWM_CLK_32     = 3, /**< F_CPU / 32  (1.95 kHz) */
    T2_PWM_CLK_64     = 4, /**< F_CPU / 64  (976.5 Hz) */
    T2_PWM_CLK_128    = 5, /**< F_CPU / 128 (488.2 Hz) */
    T2_PWM_CLK_256    = 6, /**< F_CPU / 256 (244.1 Hz) */
    T2_PWM_CLK_1024   = 7  /**< F_CPU / 1024(61.03 Hz) */
} t2_pwm_prescaler_t;

/**
 * @enum t2_pwm_channel_t
 * @brief Identificación de los canales de salida físicos del Timer 2.
 */
typedef enum {
    T2_PWM_CH_A, /**< Salida OC2A - Mapeada físicamente al pin PB3. */
    T2_PWM_CH_B  /**< Salida OC2B - Mapeada físicamente al pin PD3. */
} t2_pwm_channel_t;

/**
 * @enum t2_pwm_mode_t
 * @brief Modos de conmutación del pin ante un Match (Table 17-3).
 * @details Define si el PWM es directo (Non-Inverting) o invertido.
 */
typedef enum {
    T2_PWM_NON_INVERTING = 2, /**< Pin HIGH en BOTTOM, LOW al coincidir. */
    T2_PWM_INVERTING     = 3  /**< Pin LOW en BOTTOM, HIGH al coincidir. */
} t2_pwm_mode_t;

/* --- API Pública de Capa 1 --- */

/**
 * @brief Inicializa el Timer 2 para operar en modo Fast PWM (Modo 3).
 * @details Configura WGM2[2:0] = 3. El contador TCNT2 recorrerá de 0 a 255.
 * Las salidas se mantienen desconectadas hasta llamar a @ref Timer2_PWM_Fast_EnableChannel.
 * @param prescaler Fuente de reloj elegida para establecer la base de tiempo.
 */
void Timer2_PWM_Fast_Init(t2_pwm_prescaler_t prescaler);

/**
 * @brief Conecta el canal PWM seleccionado a su pin físico.
 * @details Configura los bits COM2x1:0 del registro TCCR2A.
 * @param channel Canal A (PB3) o Canal B (PD3).
 * @param mode Modo de operación (Inverting o Non-Inverting).
 */
void Timer2_PWM_Fast_EnableChannel(t2_pwm_channel_t channel, t2_pwm_mode_t mode);

/**
 * @brief Desconecta el canal del pin, devolviendo el control al GPIO estándar.
 * @details Setea los bits COM2x1:0 en 0. El estado del pin dependerá del registro PORT.
 * @param channel Canal A o B a desactivar.
 */
void Timer2_PWM_Fast_DisableChannel(t2_pwm_channel_t channel);

/**
 * @brief Actualiza el registro de comparación para modificar el ciclo de trabajo.
 * @details Escribe en OCR2A o OCR2B. El cambio se hace efectivo al final del ciclo actual.
 * @param channel Canal a modificar.
 * @param duty Valor de 8 bits (0-255).
 */
void Timer2_PWM_Fast_SetDuty(t2_pwm_channel_t channel, uint8_t duty);

/* --- Gestión de Interrupciones (Registro TIMSK2) --- */

/**
 * @brief Habilita o deshabilita la interrupción por desbordamiento (Overflow).
 * @param state 1 para habilitar, 0 para deshabilitar.
 */
void Timer2_PWM_IT_Overflow(uint8_t state);

/**
 * @brief Habilita o deshabilita la interrupción por comparación de canal.
 * @param channel Canal A o B.
 * @param state 1 para habilitar, 0 para deshabilitar.
 */
void Timer2_PWM_IT_CompareMatch(t2_pwm_channel_t channel, uint8_t state);

#endif /* TIMER2_FAST_PWM_H_ */