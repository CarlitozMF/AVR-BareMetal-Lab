/**
 * @file timer2_normal.c
 * @brief Implementación del driver de Capa 1 (HAL) para el Timer 2 en Modo Normal.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo gestiona el Timer 2 del ATmega328P. Su característica principal
 * es la capacidad de operar de forma asíncrona al reloj del CPU, lo que permite
 * implementar Relojes de Tiempo Real (RTC) o sistemas de temporización que 
 * sobrevivan a los modos de ahorro de energía (Sleep Modes).
 */

#include "timer2_normal.h"

/**
 * @brief Inicializa el Timer 2 en Modo Normal (8-bits).
 * @details Configura el contador para recorrer el rango completo de 0 a 255. 
 * Las salidas físicas OC2A (PB3) y OC2B (PD3) se gestionan por hardware 
 * según el modo de comparación elegido, garantizando una conmutación síncrona.
 * * @param prescaler Fuente de reloj (Interna o Asíncrona según estado de ASSR).
 * @param mode_a Comportamiento del pin físico OC2A (PB3).
 * @param mode_b Comportamiento del pin físico OC2B (PD3).
 */
void Timer2_Normal_Init(t2_prescaler_t prescaler, t2_comp_mode_t mode_a, t2_comp_mode_t mode_b) {
    /* 1. Configuración de Modo de Salida (COM2xx):
       Los bits WGM21:20 se mantienen en 0 para operar en Modo Normal. */
    TCCR2A = (mode_a << COM2A0) | (mode_b << COM2B0);
    
    /* 2. Configuración del Prescaler (CS2x):
       WGM22 se mantiene en 0. Se aplica una máscara de seguridad sobre los bits 0:2. */
    TCCR2B = (prescaler & 0x07);
}

/**
 * @brief Habilita el Reloj Asíncrono para el Timer 2.
 * @details Desconecta el Timer 2 del reloj interno de E/S y lo conecta a los 
 * pines TOSC1/2 (cristal de 32.768 kHz). 
 * * @warning Tras llamar a esta función, los registros TCNT2, OCR2A y OCR2B 
 * requieren varios ciclos del reloj externo para estabilizarse. Se debe 
 * monitorear el registro ASSR antes de realizar escrituras consecutivas.
 */
void Timer2_Enable_Async(void) {
    /* Habilita el bit AS2 (Asynchronous Timer/Counter2) */
    ASSR |= (1 << AS2);
}

/**
 * @brief Configura la Alarma A (Match) y habilita su interrupción.
 * @details Carga el registro OCR2A para disparar eventos determinísticos. 
 * En Modo Normal, la coincidencia no altera el flujo del contador TCNT2.
 * * @note Si se requiere periodicidad precisa dentro de una ISR, se debe emplear 
 * una técnica de acumulador sumando el periodo al valor previo de OCR2A.
 * * @param val Valor de 8 bits (0-255) para la comparación.
 */
void Timer2_Set_AlarmA(uint8_t val) {
    /* Carga del registro de comparación Output Compare Register A */
    OCR2A = val;
    
    /* Habilitación de la interrupción local OCIE2A en TIMSK2 */
    TIMSK2 |= (1 << OCIE2A);
}

/**
 * @brief Configura la Alarma B (Match) y habilita su interrupción.
 * @details Funciona de forma independiente al Canal A, permitiendo una segunda 
 * referencia temporal sobre el mismo hardware de 8 bits.
 * * @param val Valor de 8 bits (0-255) para la comparación.
 */
void Timer2_Set_AlarmB(uint8_t val) {
    /* Carga del registro de comparación Output Compare Register B */
    OCR2B = val;
    
    /* Habilitación de la interrupción local OCIE2B en TIMSK2 */
    TIMSK2 |= (1 << OCIE2B);
}