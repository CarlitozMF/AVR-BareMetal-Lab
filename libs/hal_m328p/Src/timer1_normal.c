/**
 * @file timer1_normal.c
 * @brief Implementación de las funciones para el Timer 1 (16-bits) en Modo Normal.
 * @author Mamani Flores Carlos
 * @date 2026
 */

#include "timer1_normal.h"

/**
 * @brief Inicializa el Timer 1 en Modo Normal.
 * @details Configura el contador para operar de 0 a 65535 (0xFFFF). 
 * Al alcanzar el valor máximo, se genera una interrupción por desbordamiento (Overflow).
 * Los pines OC1A (PB1) y OC1B (PB2) pueden ser controlados por hardware para generar
 * señales sin latencia de CPU.
 * @param prescaler Divisor de frecuencia o entrada externa por pin T1 (PD5).
 * @param mode_a Modo de salida para el Canal A (PB1).
 * @param mode_b Modo de salida para el Canal B (PB2).
 */
void Timer1_Normal_Init(t1_prescaler_t prescaler, t1_comp_mode_t mode_a, t1_comp_mode_t mode_b) {
    /* * TCCR1A – Timer/Counter1 Control Register A
     * [COM1A1][COM1A0][COM1B1][COM1B0][ – ][ – ][WGM11][WGM10]
     * * - COM1nx: Controlan los pines OC1A/B (PB1/PB2). 
     * - WGM11:10 = 0: Parte baja de la configuración del Modo Normal.
     */
    TCCR1A = (mode_a << COM1A0) | (mode_b << COM1B0);
    
    /* * TCCR1B – Timer/Counter1 Control Register B
     * [ICNC1][ICES1][ – ][WGM13][WGM12][CS12][CS11][CS10]
     * * - ICNC1/ICES1: Se configuran en la función de Input Capture.
     * - WGM13:12 = 0: Parte alta de la configuración del Modo Normal.
     * - CS12:10: Bits de selección de reloj (Prescaler).
     */
    TCCR1B = (prescaler & 0x07);
}

/**
 * @brief Configura la Unidad de Captura de Entrada (Input Capture Unit).
 * @details Esta unidad permite "congelar" el valor del Timer 1 en el registro ICR1 
 * ante un evento en el pin ICP1 (PB0). Es ideal para medición de anchos de pulso.
 * @param rising_edge Define si la captura ocurre en flanco de subida (true) o bajada (false).
 * @param noise_canceller Activa el filtrado digital que requiere 4 ciclos de reloj estables.
 */
void Timer1_InputCapture_Init(bool rising_edge, bool noise_canceller) {
    /* * ICES1 (Input Capture Edge Select): 
     * 1 = Captura en flanco de subida. 0 = Flanco de bajada.
     */
    if (rising_edge)    TCCR1B |= (1 << ICES1);
    else                TCCR1B &= ~(1 << ICES1);
    
    /* * ICNC1 (Input Capture Noise Canceler): 
     * 1 = Filtro activo (ignora pulsos menores a 4 ciclos de reloj).
     */
    if (noise_canceller) TCCR1B |= (1 << ICNC1);
    else                 TCCR1B &= ~(1 << ICNC1);
    
    /* * TIMSK1 – Timer/Counter1 Interrupt Mask Register
     * ICIE1: Habilita la interrupción por captura (Input Capture Interrupt Enable).
     */
    TIMSK1 |= (1 << ICIE1);
}

/**
 * @brief Escribe un valor de 16 bits en el registro contador.
 * @note El hardware AVR de 8 bits requiere que el byte ALTO se escriba primero. 
 * El compilador avr-gcc garantiza este orden automáticamente al usar el símbolo de 16 bits.
 * @param val Valor a cargar en TCNT1 (0 a 65535).
 */
void Timer1_Write_Counter(uint16_t val) {
    /* TCNT1H y TCNT1L se acceden mediante este registro de 16 bits.
       El hardware usa un buffer temporal para que la escritura sea coherente. */
    TCNT1 = val;
}

/**
 * @brief Lee el valor actual del contador de 16 bits.
 * @note Para lecturas fuera de una ISR, se recomienda deshabilitar interrupciones 
 * globales para evitar que el registro temporal se corrompa durante el acceso.
 * @return Valor actual de TCNT1.
 */
uint16_t Timer1_Read_Counter(void) {
    /* El compilador avr-gcc genera automáticamente la secuencia:
       1. Lectura del byte bajo (TCNT1L) -> El byte alto se mueve a TEMP.
       2. Lectura del byte alto (TCNT1H) desde TEMP. */
    return TCNT1;
}
/**
 * @brief Configura el punto de comparación del Canal A.
 * @details Al igualarse TCNT1 con OCR1A, se dispara la interrupción COMPA. 
 * En modo normal, el contador NO se detiene ni se resetea.
 * @param val Valor de comparación (16 bits).
 */
void Timer1_Set_AlarmA(uint16_t val) {
    /* Registro de comparación de salida A (Output Compare Register 1 A) */
    OCR1A = val;
    
    /* Habilitación de la interrupción local OCIE1A (Output Compare A Match Interrupt Enable) 
       ubicada en el registro de máscara TIMSK1. */
    TIMSK1 |= (1 << OCIE1A);
}

/**
 * @brief Configura el punto de comparación del Canal B.
 * @details Útil para programar una segunda tarea asíncrona (como polling de botones).
 * @param val Valor de comparación (16 bits).
 */
void Timer1_Set_AlarmB(uint16_t val) {
    /* Registro de comparación de salida B (Output Compare Register 1 B) */
    OCR1B = val;
    
    /* Habilitación de la interrupción local OCIE1B (Output Compare B Match Interrupt Enable) */
    TIMSK1 |= (1 << OCIE1B);
}