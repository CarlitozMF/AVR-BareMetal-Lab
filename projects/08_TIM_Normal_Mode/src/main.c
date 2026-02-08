/**
 * @file main.c
 * @brief Orquestación del Proyecto 08: Motor, Botones y LED Breathing.
 * @author Mamani Flores Carlos
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "timer1_normal.h"
#include "timer2_normal.h"
#include "step_motor_28BYJ48.h"
#include "hardware.h"

/* Instancia global del motor */
Stepper_t motor_principal;

/* Variables de estado para los botones */
volatile bool motor_running = false;
volatile Step_Dir_t motor_dir = STEP_CW;

int main(void) {
    /* 1. Configuración de Hardware (Capa 3) */
    // Mapeo de pines para el motor según hardware.h
    volatile uint8_t* m_ports[] = {M1_IN1_PORT, M1_IN2_PORT, M1_IN3_PORT, M1_IN3_PORT};
    uint8_t m_pins[] = {M1_IN1_PIN, M1_IN2_PIN, M1_IN3_PIN, M1_IN3_PIN};
    
    /* 2. Inicialización de Drivers (Capa 2) */
    Stepper_Init(&motor_principal, m_ports, m_pins, MODE_HALF_STEP);
    
    /* 3. Configuración de Timers (Capa 1) */
    // Timer 1: Multitarea (Motor + Botones)
    Timer1_Normal_Init(T1_CLK_64, T1_PIN_DISCONNECT, T1_PIN_DISCONNECT);
    Timer1_Set_AlarmA(MOTOR_SPEED_DEFAULT);   // Alarma para el primer paso
    Timer1_Set_AlarmB(BUTTON_DEBOUNCE_TICKS); // Alarma para el primer polling
    
    // Timer 2: LED Breathing (PWM)
    Timer2_Normal_Init(T2_CLK_8, T2_PIN_SET, T2_PIN_DISCONNECT); // Ejemplo simplificado
    
    sei(); // Habilitar interrupciones

    while (1) {
        /* Aquí irá el refresco del LCD (Capa 2) cada 200ms aprox */
        // LCD_Update(motor_running, motor_dir);
    }
}

/* --- GESTIÓN DE INTERRUPCIONES (CAPA 3) --- */

/**
 * @brief ISR Canal A: Control de pasos del motor.
 */
ISR(TIMER1_COMPA_vect) {
    if (motor_running) {
        motor_principal.is_active = true;
        motor_principal.direction = motor_dir;
        Stepper_Step_Sequential(&motor_principal);
    } else {
        Stepper_Stop(&motor_principal);
    }
    
    // Re-programar alarma relativa para el siguiente paso
    OCR1A += MOTOR_SPEED_DEFAULT;
}

/**
 * @brief ISR Canal B: Polling de botones con Debounce simple.
 */
ISR(TIMER1_COMPB_vect) {
    static uint8_t last_state_play = 1;
    static uint8_t last_state_dir = 1;

    // Leer pines definidos en hardware.h
    uint8_t current_play = (PIND & (1 << BTN_START_STOP));
    uint8_t current_dir  = (PIND & (1 << BTN_DIR));

    // Detección de flanco de bajada para Play/Stop
    if (last_state_play && !current_play) {
        motor_running = !motor_running;
    }
    last_state_play = current_play;

    // Detección de flanco de bajada para Sentido
    if (last_state_dir && !current_dir) {
        motor_dir = (motor_dir == STEP_CW) ? STEP_CCW : STEP_CW;
    }
    last_state_dir = current_dir;

    // Re-programar alarma para el próximo chequeo de botones (20ms)
    OCR1B += BUTTON_DEBOUNCE_TICKS;
}