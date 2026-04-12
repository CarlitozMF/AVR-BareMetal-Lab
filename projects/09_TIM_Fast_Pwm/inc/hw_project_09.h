/**
 * @file hw_project_09.h
 * @brief Mapeo de hardware y definiciones de periféricos para el Proyecto 09.
 */

#ifndef HW_PROJECT_09_H_
#define HW_PROJECT_09_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "gpio.h"
#include "systick.h"
#include "exti.h"
#include "timer2_fast_pwm.h"

/* --- Configuración del Pulsador --- */
#define BUTTON_PORT       GPIO_D
#define BUTTON_PIN        2             // INT0
#define BUTTON_EXTI_LINE  EXTI_INT0

/* --- Configuración de LEDs (PWM) --- */
#define LED_BREATH_CH     T2_PWM_CH_A   // PB3
#define LED_PULSE_CH      T2_PWM_CH_B   // PD3

/* --- Configuración de Systick --- */
#define SYSTICK_TIMER     TIMER_0

/**
 * @brief Inicializa todo el hardware específico del proyecto.
 */
void HW_Init(void);

#endif /* HW_PROJECT_09_H_ */
