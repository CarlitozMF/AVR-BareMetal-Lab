/**
 * @file timer1_normal.c
 * @brief Implementación de las funciones para el Timer 1 (16-bits) en Modo Normal.
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este módulo gestiona el periférico más potente del ATmega328P. 
 * Al operar en 16 bits, permite un rango de conteo de 0 a 65535, facilitando 
 * temporizaciones prolongadas y mediciones de alta precisión. Incluye la 
 * lógica para la Unidad de Captura de Entrada (ICU) y Comparación de Salida.
 */

#include "timer1_normal.h"

/**
 * @brief Inicializa el Timer 1 en Modo Normal (16-bits).
 * @details Configura el contador para operar de 0 a 0xFFFF. Al desbordar, 
 * dispara el vector TIMER1_OVF_vect. Los pines OC1A (PB1) y OC1B (PB2) 
 * pueden conmutar por hardware, eliminando el jitter del software.
 * * @param prescaler Divisor de frecuencia o entrada externa (pin T1/PD5).
 * @param mode_a Comportamiento físico del pin OC1A (PB1) ante un Match.
 * @param mode_b Comportamiento físico del pin OC1B (PB2) ante un Match.
 */
void Timer1_Normal_Init(t1_prescaler_t prescaler, t1_comp_mode_t mode_a, t1_comp_mode_t mode_b) {
    /* TCCR1A – Timer/Counter1 Control Register A
     * Bits COM1A1:0 y COM1B1:0 controlan los pines físicos asociados.
     * WGM11:10 en 0 para Modo Normal. */
    TCCR1A = (mode_a << COM1A0) | (mode_b << COM1B0);
    
    /* TCCR1B – Timer/Counter1 Control Register B
     * WGM13:12 en 0 para completar la configuración de Modo Normal.
     * CS12:10 seleccionan la fuente de reloj (Prescaler). */
    TCCR1B = (prescaler & 0x07);
}

/**
 * @brief Configura la Unidad de Captura de Entrada (Input Capture Unit).
 * @details Permite capturar el valor de TCNT1 en el registro ICR1 ante un evento
 * en el pin ICP1 (PB0). Es la herramienta estándar para medir periodos y frecuencias.
 * * @param rising_edge true: captura en flanco de subida, false: flanco de bajada.
 * @param noise_canceller true: activa filtro digital (requiere 4 ciclos de reloj estables).
 */
void Timer1_InputCapture_Init(bool rising_edge, bool noise_canceller) {
    /* ICES1 (Input Capture Edge Select): 1=Subida, 0=Bajada */
    if (rising_edge)     TCCR1B |= (1 << ICES1);
    else                 TCCR1B &= ~(1 << ICES1);
    
    /* ICNC1 (Input Capture Noise Canceler): 1=Filtro activo */
    if (noise_canceller) TCCR1B |= (1 << ICNC1);
    else                 TCCR1B &= ~(1 << ICNC1);
    
    /* Habilitación de la interrupción ICIE1 en TIMSK1 */
    TIMSK1 |= (1 << ICIE1);
}

/**
 * @brief Escribe un valor de 16 bits en el registro contador TCNT1.
 * @note El hardware requiere que el byte ALTO se escriba antes que el bajo. 
 * avr-gcc garantiza este orden al operar sobre el símbolo de 16 bits 'TCNT1'.
 * @param val Valor de 16 bits (0-65535).
 */
void Timer1_Write_Counter(uint16_t val) {
    TCNT1 = val;
}

/**
 * @brief Lee el valor actual del contador de 16 bits.
 * @details El hardware utiliza un registro temporal (TEMP) para asegurar la 
 * coherencia de los dos bytes leídos. 
 * @return uint16_t Valor actual de TCNT1.
 */
uint16_t Timer1_Read_Counter(void) {
    return TCNT1;
}

/**
 * @brief Configura la Alarma A (Canal de comparación A).
 * @details Al producirse el Match (TCNT1 == OCR1A), se dispara la interrupción 
 * OCIE1A. En Modo Normal, el contador continúa su conteo sin resetearse.
 * @param val Valor de comparación de 16 bits.
 */
void Timer1_Set_AlarmA(uint16_t val) {
    OCR1A = val;
    /* Activa interrupción Output Compare A Match en TIMSK1 */
    TIMSK1 |= (1 << OCIE1A);
}

/**
 * @brief Configura la Alarma B (Canal de comparación B).
 * @details Provee una segunda base de tiempo independiente sobre el mismo Timer.
 * @param val Valor de comparación de 16 bits.
 */
void Timer1_Set_AlarmB(uint16_t val) {
    OCR1B = val;
    /* Activa interrupción Output Compare B Match en TIMSK1 */
    TIMSK1 |= (1 << OCIE1B);
}