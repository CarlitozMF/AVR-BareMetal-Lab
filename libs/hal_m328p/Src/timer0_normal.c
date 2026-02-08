/**
 * @file timer0_normal.c
 * @brief Implementación de las funciones del Timer 0 en Modo Normal.
 * @author CarlitozMF
 */

#include "timer0_normal.h"

/**
 * @brief Inicializa el Timer 0 en Modo Normal (8-bits).
 * * @details Este modo configura el contador para que incremente desde 0 hasta 255 (0xFF).
 * Al llegar al máximo, se produce un desbordamiento (Overflow) y reinicia desde 0.
 * La configuración de los pines OC0A/B permite que el hardware cambie el estado físico 
 * de los pines PD6 y PD5 de forma automática ante una coincidencia (Match).
 * * @param prescaler Fuente de reloj o entrada externa por el pin T0 (PD4).
 * @param mode_a Comportamiento del pin físico OC0A (PD6).
 * @param mode_b Comportamiento del pin físico OC0B (PD5).
 */
void Timer0_Normal_Init(t0_prescaler_t prescaler, t0_comp_mode_t mode_a, t0_comp_mode_t mode_b) {
    /* 1. Reset de registros de control:
       Aseguramos que los bits WGM02, WGM01 y WGM00 estén en 0. 
       Esto garantiza que el timer opere estrictamente en Modo Normal. */
    TCCR0A = 0;
    TCCR0B = 0;

    /* 2. Configurar el modo de los pines físicos OC0A y OC0B:
       Desplazamos los enums a las posiciones de los bits COM (Compare Output Mode).
       COM0A1:0 (bits 7:6) controla el Canal A.
       COM0B1:0 (bits 5:4) controla el Canal B. */
    TCCR0A |= (mode_a << COM0A0) | (mode_b << COM0B0);

    /* 3. Configurar la fuente de reloj (Prescaler):
       Aplicamos una máscara de 0x07 (00000111) para asegurar que solo modificamos 
       los bits CS02, CS01 y CS00 de TCCR0B. */
    TCCR0B |= (prescaler & 0x07);
}

/**
 * @brief Define el punto de interrupción del Canal A.
 * * @details Escribe en el registro OCR0A. Cuando TCNT0 iguala este valor, se levanta 
 * la bandera OCF0A. Al habilitar OCIE0A, el CPU ejecutará la ISR correspondiente.
 * * @note En Modo Normal, el contador NO se resetea tras el Match. Sigue hasta 255.
 * @param val Valor de comparación de 8 bits.
 */
void Timer0_Set_AlarmA(uint8_t val) {
    /* Carga del registro de comparación de salida A */
    OCR0A = val;
    /* Habilitación de la interrupción local por coincidencia de salida A */
    TIMSK0 |= (1 << OCIE0A); 
}

/**
 * @brief Define el punto de interrupción del Canal B.
 * * @details Similar al Canal A, pero opera sobre el registro OCR0B y el pin PD5.
 * Ideal para programar una segunda tarea independiente en la misma base de tiempo.
 * * @param val Valor de comparación de 8 bits.
 */
void Timer0_Set_AlarmB(uint8_t val) {
    /* Carga del registro de comparación de salida B */
    OCR0B = val;
    /* Habilitación de la interrupción local por coincidencia de salida B */
    TIMSK0 |= (1 << OCIE0B); 
}