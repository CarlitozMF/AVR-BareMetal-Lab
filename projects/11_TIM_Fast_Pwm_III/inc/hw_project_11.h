#ifndef HW_PROJECT_11_H
#define HW_PROJECT_11_H

#include <avr/io.h>
#include "timer1_fast_pwm.h"

/* Definiciones de Hardware */
#define SERVO_FREQ_TOP      39999 
#define SG90_MIN_TICKS      1000  
#define SG90_MAX_TICKS      5500  

/* Wrappers: Adaptan la Capa 1 a la Capa 2 */
static inline void hw_servo_A_write(uint16_t ticks) {
    Timer1_PWM_Fast_SetCompare(T1_PWM_CH_A, ticks);
}

static inline void hw_servo_B_write(uint16_t ticks) {
    Timer1_PWM_Fast_SetCompare(T1_PWM_CH_B, ticks);
}

#endif