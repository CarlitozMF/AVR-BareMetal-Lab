/**
 * @file timer.c
 * @brief Implementación de la HAL de Timers para el ATmega328P.
 * @author Mamani Flores Carlos
 */

#include "timer.h"

/** * @brief Contador global de milisegundos.
 * Se declara static para encapsulamiento y volatile para evitar optimizaciones
 * del compilador, ya que se modifica dentro de una ISR.
 */
static volatile uint32_t ms_ticks = 0;

void TIMER_Init(timer_instance_t instance) {
    switch (instance) {
        case TIMER_0:
            /* Configuración Timer0: Modo CTC, Prescaler 64, Tick 1ms */
            TCCR0A = (1 << WGM01);              
            TCCR0B = (1 << CS01) | (1 << CS00); 
            OCR0A  = 249;                       
            TIMSK0 = (1 << OCIE0A);             
            break;

        case TIMER_1:
            /* Configuración Timer1 (16 bits): Modo CTC, Prescaler 64, Tick 1ms */
            TCCR1A = 0;                         
            TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); 
            OCR1A  = 249;                       
            TIMSK1 = (1 << OCIE1A);
            break;

        case TIMER_2:
            /* Configuración Timer2: Modo CTC, Prescaler 64, Tick 1ms */
            /* Nota: Los bits de prescaler CS22 en Timer2 equivalen a div 64 */
            TCCR2A = (1 << WGM21);              
            TCCR2B = (1 << CS22);               
            OCR2A  = 249;
            TIMSK2 = (1 << OCIE2A);
            break;
    }
}

uint32_t get_tick(void) {
    uint32_t tick_copy;
    
    /* Guardamos el registro de estado (SREG) para preservar el flag de interrupciones */
    uint8_t sreg = SREG;
    
    /* SECCIÓN CRÍTICA: Deshabilitamos Ints para leer 32 bits de forma atómica */
    cli();
    tick_copy = ms_ticks;
    
    /* Restauramos SREG (esto habilita las Ints si estaban habilitadas antes) */
    SREG = sreg;
    
    return tick_copy;
}

void delay_ms_tick(uint32_t ms) {
    uint32_t start_time = get_tick();
    while ((get_tick() - start_time) < ms) {
        /* Espera activa basada en hardware */
    }
}

/**
 * @brief Rutinas de Servicio de Interrupción (ISR).
 * @details Las tres ISRs incrementan el mismo contador ms_ticks, lo que permite
 * que el sistema sea agnóstico al timer físico utilizado para el Systick.
 */
ISR(TIMER0_COMPA_vect) { ms_ticks++; }
ISR(TIMER1_COMPA_vect) { ms_ticks++; }
ISR(TIMER2_COMPA_vect) { ms_ticks++; }