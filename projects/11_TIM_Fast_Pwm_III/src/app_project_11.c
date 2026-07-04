#include "app_project_11.h"
#include "hw_project_11.h"
#include "servo_sg90.h"
#include "systick.h"
#include <avr/interrupt.h>

/* Instancias privadas de los servos */
static Servo_t servo1;
static Servo_t servo2;

/* Variables de estado del movimiento */
static uint8_t  angle = 0;
static int8_t   step  = 1;
static uint32_t last_tick = 0;

void App_Init(void) {
    /* 1. Hardware PWM */
    Timer1_PWM_Fast_Init(T1_PWM_CLK_8, SERVO_FREQ_TOP);
    Timer1_PWM_Fast_EnableChannel(T1_PWM_CH_A, T1_PWM_NON_INVERTING);
    Timer1_PWM_Fast_EnableChannel(T1_PWM_CH_B, T1_PWM_NON_INVERTING);

    /* 2. Drivers de Dispositivo (Inyección) */
    Servo_Init(&servo1,  SG90_MIN_TICKS, SG90_MAX_TICKS, hw_servo_A_write);
    Servo_Init(&servo2, SG90_MIN_TICKS, SG90_MAX_TICKS, hw_servo_B_write);

    /* 3. Posicionamiento inicial seguro */
    Servo_SetAngle(&servo1, 0);
    Servo_SetAngle(&servo2, 180);

    /* 4. Base de tiempo */
    Systick_Init(TIMER_0);
    //last_tick = get_tick();
}

void App_Task_ServoSweep(void) {
    if (get_tick() - last_tick >= 30) {
        last_tick = get_tick();

        // Lógica de rebote
        if (angle >= 180) step = -1;
        if (angle <= 0)   step = 1;
        angle += step;

        // Actualización de servos
        Servo_SetAngle(&servo1,  angle);
        Servo_SetAngle(&servo2, 180 - angle); // Espejo
    }
}