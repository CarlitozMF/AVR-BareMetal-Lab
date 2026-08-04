/**
 * @file systick.c
 * @brief Implementación de la HAL de Systick para el microcontrolador ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este módulo proporciona una base de tiempo (System Tick) de 1ms 
 * configurable mediante cualquiera de los tres Timers internos del MCU.
 * Implementa lectura atómica y funciones de retardo no bloqueante para 
 * soportar una arquitectura de multitarea cooperativa.
 */

#include "systick.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/** * @brief Contador global de milisegundos.
 * @note Se declara 'static' para encapsulamiento y 'volatile' para asegurar que
 * el compilador no optimice los accesos, ya que cambia en contexto de ISR.
 */
static volatile uint32_t ms_ticks = 0;

/**
 * @brief Inicializa el recurso de hardware para el System Tick.
 * * @param instance Selector del periférico físico (TIMER_0, TIMER_1, TIMER_2).
 * @note La configuración asume un F_CPU de 16MHz y establece un tick de 1ms.
 * @return void
 */
void Systick_Init(timer_instance_t instance) {
    switch (instance) {
        case TIMER_0:
            /* Configuración Timer0 (8 bits): Modo CTC, Prescaler 64, Tick 1ms */
            TCCR0A = (1 << WGM01);              // Modo CTC
            TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
            OCR0A  = 249;                       // (16MHz / 64 / 1kHz) - 1
            TIMSK0 = (1 << OCIE0A);             // Habilitar Int. por comparación
            break;

        case TIMER_1:
            /* Configuración Timer1 (16 bits): Modo CTC, Prescaler 64, Tick 1ms */
            TCCR1A = 0;                         
            TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC y Prescaler 64
            OCR1A  = 249;                       
            TIMSK1 = (1 << OCIE1A);             
            break;

        case TIMER_2:
            /* Configuración Timer2 (8 bits): Modo CTC, Prescaler 64, Tick 1ms */
            TCCR2A = (1 << WGM21);              
            TCCR2B = (1 << CS22);               // Prescaler 64 (Diferente a T0/T1)
            OCR2A  = 249;
            TIMSK2 = (1 << OCIE2A);
            break;
    }
}

/**
 * @brief Obtiene el valor actual del contador de milisegundos.
 * * @details Implementa una SECCIÓN CRÍTICA para garantizar una lectura atómica
 * de la variable ms_ticks de 32 bits en una arquitectura de 8 bits.
 * @return uint32_t Milisegundos transcurridos desde el inicio del programa.
 */
uint32_t get_tick(void) {
    uint32_t tick_copy;
    
    /* Backup del registro de estado para preservar el flag de interrupciones (I) */
    uint8_t sreg = SREG;
    
    /* Inicio Sección Crítica */
    cli();
    tick_copy = ms_ticks;
    
    /* Fin Sección Crítica: Restauración del estado original de interrupciones */
    SREG = sreg;
    
    return tick_copy;
}

/**
 * @brief Genera un retardo basado en el Systick (No bloqueante para ISRs).
 * * @param ms Tiempo en milisegundos a esperar.
 * @note Aunque es una espera activa en el flujo principal, permite que las 
 * interrupciones se sigan ejecutando normalmente.
 */
void delay_ms_tick(uint32_t ms) {
    uint32_t start_time = get_tick();
    while ((get_tick() - start_time) < ms) {
        /* Espera activa sincronizada por hardware */
    }
}

/**
 * @brief Rutinas de Servicio de Interrupción (ISR).
 * @details Las tres ISR incrementan el mismo contador. Solo debe dejarse 
 * habilitada la ISR correspondiente al Timer seleccionado en la inicialización 
 * para evitar overhead o conflictos.
 */

/* Nota: Descomentar las siguientes ISR según el Timer utilizado para el Systick.
 * Solo una de ellas debe estar activa si se desea evitar redundancia. */

/* ISR para Timer 0 */

ISR(TIMER0_COMPA_vect) { 
    ms_ticks++; 
}

/* ISR para Timer 1 */
/*
ISR(TIMER1_COMPA_vect) { 
    ms_ticks++; 
}
*/

/* ISR para Timer 2 */
/*
ISR(TIMER2_COMPA_vect) { 
    ms_ticks++; 
}
*/