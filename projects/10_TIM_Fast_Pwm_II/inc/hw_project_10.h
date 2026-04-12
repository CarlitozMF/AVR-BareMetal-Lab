/**
 * @file hw_project_10.h
 * @brief Mapeo de hardware y wrappers de Capa 1 para el Proyecto 10.
 * @details Centraliza las definiciones de pines y la abstracción de periféricos
 * para permitir que la Capa de Aplicación sea independiente del pinout físico.
 * @author Mamani Flores Carlos
 */

#ifndef HW_PROJECT_10_H_
#define HW_PROJECT_10_H_

/* --- 1. Definiciones de Preprocesador --- */
#ifndef F_CPU
/** @brief Frecuencia de CPU para cálculos de delays y baudrate (16MHz) */
#define F_CPU 16000000UL 
#endif

/* --- 2. Inclusión de Drivers de Capa 1 (HAL) --- */
#include "gpio.h"
#include "timer0_fast_pwm.h"
#include "timer2_fast_pwm.h"

/* --- 3. Mapeo Lógico de Hardware --- */

/** @name Heartbeat System LED
 * LED indicador de estado del sistema (Capa 4 - Tarea de Toggle)
 */
/**@{*/
#define LED_SYS_PORT   GPIO_B
#define LED_SYS_PIN    0            // Pin PB0
/**@}*/

/** @name User Button (EXTI)
 * Pulsador configurado para interrupción externa INT0
 */
/**@{*/
#define BTN_PORT       GPIO_D
#define BTN_PIN        2            // Pin PD2 (INT0)
#define BTN_EXTI_LINE  EXTI_INT0    // Línea de interrupción externa 0
/**@}*/

/** @name Manual Dimmer LED
 * LED controlado por pulsaciones externas vía PWM en Timer 2
 */
/**@{*/
#define LED_BTN_PORT    GPIO_D
#define LED_BTN_PIN     3           // Pin PD3
#define LED_BTN_CH      T2_PWM_CH_B // Canal B del Timer 2 (OC2B)
/**@}*/

/* --- 4. Wrappers de Capa 1 (Abstracción de Hardware) --- */

/**
 * @brief Wrappers estáticos para inyección de dependencias en el driver RGB.
 * @details Estas funciones adaptan la firma de los drivers de Timer 0 y 2
 * al formato esperado por el driver genérico de Capa 2 (rgb_led_driver.h).
 * El uso de 'static inline' elimina la sobrecarga de la llamada a función,
 * optimizando el rendimiento en tiempo de ejecución.
 * * @param d Duty cycle (0-255)
 */
static inline void set_r_hw(uint8_t d) { Timer0_PWM_SetDuty(T0_PWM_CH_A, d); }
static inline void set_g_hw(uint8_t d) { Timer0_PWM_SetDuty(T0_PWM_CH_B, d); }
static inline void set_b_hw(uint8_t d) { Timer2_PWM_Fast_SetDuty(T2_PWM_CH_A, d); }


#endif /* HW_PROJECT_10_H_ */