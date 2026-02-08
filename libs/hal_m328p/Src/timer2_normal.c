/**
 * @file timer2_normal.c
 * @brief Implementación para el Timer 2 en Modo Normal.
 */

#include "timer2_normal.h"

/**
 * @brief Inicializa el Timer 2 en Modo Normal (8-bits).
 * * @details Configura la generación de formas de onda en modo Normal (cuenta de 0 a 255).
 * Los pines OC2A y OC2B se configuran según el modo elegido para interactuar con el 
 * hardware sin intervención de la ISR (Toggle, Clear o Set).
 * * @param prescaler Fuente de reloj (Interna o Asíncrona según estado de ASSR).
 * @param mode_a Comportamiento del pin físico OC2A (PB3).
 * @param mode_b Comportamiento del pin físico PD3 (OC2B).
 */
void Timer2_Normal_Init(t2_prescaler_t prescaler, t2_comp_mode_t mode_a, t2_comp_mode_t mode_b) {
    /* Configuración de Modo de Salida de Comparación (COM bits)
       WGM21 y WGM20 se mantienen en 0 para asegurar el Modo Normal. */
    TCCR2A = (mode_a << COM2A0) | (mode_b << COM2B0);
    
    /* Configuración de selección de Reloj (CS bits)
       WGM22 se mantiene en 0. Los bits 0:2 definen el prescaler. */
    TCCR2B = (prescaler & 0x07);
}

/**
 * @brief Habilita el Reloj Asíncrono para el Timer 2.
 * * @details Al activar el bit AS2, el Timer 2 deja de depender del reloj de E/S del CPU
 * y pasa a ser gobernado por un cristal de cuarzo externo (típicamente 32.768 kHz) 
 * conectado a los pines TOSC1 y TOSC2. 
 * * @warning Tras llamar a esta función, los registros TCNT2, OCR2A y OCR2B tardan 
 * varios ciclos de reloj en actualizarse. Se recomienda esperar a que los bits 
 * de actualización en ASSR (TCN2UB, OCR2AUB, etc.) se limpien antes de operar.
 */
void Timer2_Enable_Async(void) {
    /* Conecta el Timer 2 a la fuente de reloj externa (Pines TOSC) */
    ASSR |= (1 << AS2);
}

/**
 * @brief Configura la Alarma A (Match) y habilita su interrupción.
 * * @details Al escribir en OCR2A, el hardware comparará este valor con TCNT2. 
 * Si estamos en Modo Normal, la coincidencia NO resetea el timer. 
 * * @note Si se usa para tareas periódicas dentro de la ISR, se debe sumar el 
 * periodo al valor actual de OCR2A para agendar la próxima "cita".
 * * @param val Valor de 8 bits (0-255) para la comparación.
 */
void Timer2_Set_AlarmA(uint8_t val) {
    /* 1. Cargamos el valor de comparación en el registro de salida A */
    OCR2A = val;
    
    /* 2. Habilitamos la interrupción específica del Canal A (Output Compare Match A) */
    /* Nota: Esto permite que el CPU salte al vector TIMER2_COMPA_vect al haber coincidencia */
    TIMSK2 |= (1 << OCIE2A);
}

/**
 * @brief Configura la Alarma B (Match) y habilita su interrupción.
 * * @details Funciona de forma idéntica al Canal A, permitiendo tener una segunda
 * base de tiempo independiente sobre el mismo Timer 2.
 * * @param val Valor de 8 bits (0-255) para la comparación.
 */
void Timer2_Set_AlarmB(uint8_t val) {
    /* 1. Cargamos el valor de comparación en el registro de salida B */
    OCR2B = val;
    
    /* 2. Habilitamos la interrupción específica del Canal B (Output Compare Match B) */
    TIMSK2 |= (1 << OCIE2B);
}