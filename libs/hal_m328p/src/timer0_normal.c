/**
 * @file timer0_normal.c
 * @brief Implementación de las funciones del Timer 0 en Modo Normal para el ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este driver de Capa 1 permite operar el Timer 0 en su modo más básico y flexible.
 * A diferencia del modo CTC, aquí el contador (TCNT0) siempre recorre el rango completo (0-255).
 * Se utilizan los registros de comparación OCR0A y OCR0B como "alarmas" asíncronas para 
 * disparar eventos o conmutar pines físicos (PD6/PD5) sin resetear la base de tiempo.
 */

#include "timer0_normal.h"
#include <avr/io.h>

/**
 * @brief Inicializa el Timer 0 en Modo Normal (8-bits).
 * * @details Configura el contador para que incremente de 0 a 255. Permite definir el 
 * comportamiento de los pines OC0A (PD6) y OC0B (PD5) mediante hardware, lo que garantiza
 * una latencia de conmutación nula (jitter-free).
 * * @param prescaler Fuente de reloj o entrada de pulsos externos por T0 (PD4).
 * @param mode_a Comportamiento del pin físico OC0A ante un Match.
 * @param mode_b Comportamiento del pin físico OC0B ante un Match.
 */
void Timer0_Normal_Init(t0_prescaler_t prescaler, t0_comp_mode_t mode_a, t0_comp_mode_t mode_b) {
    /* 1. Reset de registros de control:
       Forzamos WGM02:0 a 0 para asegurar el funcionamiento en Modo Normal. */
    TCCR0A = 0;
    TCCR0B = 0;

    /* 2. Configurar el Compare Output Mode (COM):
       Mapeamos los modos de comparación a los bits COM0nx.
       COM0A1:0 (bits 7:6) -> Canal A.
       COM0B1:0 (bits 5:4) -> Canal B. */
    TCCR0A |= (mode_a << COM0A0) | (mode_b << COM0B0);

    /* 3. Configurar Prescaler y arranque:
       Aplicamos máscara de seguridad 0x07 (bits CS02:0) en TCCR0B. */
    TCCR0B |= (prescaler & 0x07);
}

/**
 * @brief Configura el punto de interrupción y evento para el Canal A.
 * * @details Carga el registro OCR0A. Al producirse la igualdad con TCNT0, se 
 * activa la interrupción OCIE0A y el hardware ejecuta la acción definida en mode_a.
 * * @note En este modo, el contador NO se detiene ni se resetea al alcanzar el valor.
 * @param val Valor de comparación (0-255).
 */
void Timer0_Set_AlarmA(uint8_t val) {
    /* Carga del registro de comparación */
    OCR0A = val;
    /* Habilitación de la interrupción local por Output Compare Match A */
    TIMSK0 |= (1 << OCIE0A); 
}

/**
 * @brief Configura el punto de interrupción y evento para el Canal B.
 * * @details Opera de forma independiente al Canal A sobre el registro OCR0B.
 * Permite gestionar una segunda tarea o señal PWM manual en la misma base de tiempo.
 * * @param val Valor de comparación (0-255).
 */
void Timer0_Set_AlarmB(uint8_t val) {
    /* Carga del registro de comparación */
    OCR0B = val;
    /* Habilitación de la interrupción local por Output Compare Match B */
    TIMSK0 |= (1 << OCIE0B); 
}