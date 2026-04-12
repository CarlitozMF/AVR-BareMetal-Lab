/**
 * @file main.c
 * @brief Implementación de control de LEDs mediante PWM, Systick y EXTI.
 * @author Mamani Flores Carlos
 * @date 2026
 */

 /**************************************************************************************
* PROYECTO: 09_TIM_Fast_Pwm
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN:  Aplicacion de Timer Fast PWM en leds, systick y exti
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "main_project_09.h"

/* --- Variables Globales --- */
volatile uint8_t pulsador_presionado = 0;

int main(void) {
    /* --- 1. Inicialización de GPIO (Usando HW Mapping) --- */
    GPIO_InitPin(BUTTON_PORT, BUTTON_PIN, GPIO_INPUT);
    GPIO_WritePin(BUTTON_PORT, BUTTON_PIN, GPIO_HIGH); // Pull-up
    
    /* --- 2. Inicialización de Capa 1 (Drivers) --- */
    Systick_Init(SYSTICK_TIMER); 
    Timer2_PWM_Fast_Init(T2_PWM_CLK_64); 
    
    // Solo inicia el led que respira
    Timer2_PWM_Fast_EnableChannel(LED_BREATH_CH, T2_PWM_NON_INVERTING); 
    
    // Configuración de EXTI
    EXTI_Init(BUTTON_EXTI_LINE, EXTI_FALLING_EDGE);
    
    sei(); 

    while(1) {
        task_breathing();
        task_button_led();
    }
}

/* --- Implementación de Tareas (Usando nombres del .h) --- */

void task_breathing(void) {
    static uint32_t last_tick = 0;
    static uint8_t brillo = 0;
    static int8_t paso = 1;
    
    if ((get_tick() - last_tick) >= 10) {
        last_tick = get_tick();
        brillo += paso;
        if (brillo == 255 || brillo == 0) paso *= -1;
        Timer2_PWM_Fast_SetDuty(LED_BREATH_CH, brillo);
    }
}

void task_button_led(void) {
    static uint8_t brillo_manual = 0;
    
    if (pulsador_presionado) {
        if (brillo_manual >= 255) brillo_manual = 0; 
        else brillo_manual += 51; 
        
        if (brillo_manual == 0) {
            Timer2_PWM_Fast_DisableChannel(LED_PULSE_CH);
            // Aquí podrías definir LED_PULSE_PORT y PIN en el .h para no hardcodear PD3
            GPIO_WritePin(GPIO_D, 3, GPIO_LOW);
        } else {
            Timer2_PWM_Fast_EnableChannel(LED_PULSE_CH, T2_PWM_NON_INVERTING);
            Timer2_PWM_Fast_SetDuty(LED_PULSE_CH, brillo_manual);
        }
        pulsador_presionado = 0; 
    }
}

/* --- Rutinas de Servicio de Interrupción (ISR) --- */

ISR(INT0_vect) {
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time = get_tick();
    
    if (current_time - last_interrupt_time > 200) {
        pulsador_presionado = 1;
        last_interrupt_time = current_time;
    }
}