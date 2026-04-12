/**
 * @file timer2_fast_pwm.c
 * @brief Implementación de la Capa 1 (HAL) para Fast PWM en el Timer 2 (8-bits).
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este driver implementa el Modo 3 (Fast PWM con TOP fijo en 0xFF). 
 * El Timer 2 es único en el ATmega328P por su prescaler altamente flexible, 
 * que incluye divisores por 32 y 128, lo que permite frecuencias de PWM 
 * optimizadas para evitar ruidos audibles en motores o parpadeos en LEDs.
 */

#include "timer2_fast_pwm.h"
#include "gpio.h"

/**
 * @brief Inicializa el hardware del Timer 2 para Fast PWM.
 * @details 
 * 1. Configura WGM21:0 = 3 (Fast PWM, TOP=0xFF) mediante el registro TCCR2A.
 * 2. Limpia TCCR2B para asegurar que WGM22 = 0 y resetear el prescaler.
 * 3. Carga el prescaler seleccionado iniciando el conteo del TCNT2.
 * @param prescaler Fuente de reloj según Table 17-9 del datasheet.
 */
void Timer2_PWM_Fast_Init(t2_pwm_prescaler_t prescaler) {
    /* Configuración de Waveform Generation Mode (WGM):
     * Seteamos WGM21=1 y WGM20=1 (Modo 3). El contador TCNT2 
     * incrementará de 0x00 hasta 0xFF de forma cíclica. */
    TCCR2A = (1 << WGM21) | (1 << WGM20);
    
    /* Reset de registros de control B:
     * Garantizamos que el bit WGM22 sea 0 (TOP fijo en 255) y 
     * limpiamos configuraciones previas de los bits CS22:20. */
    TCCR2B = 0; 

    // Forzamos el Duty a 0 antes de cualquier otra cosa
    OCR2A = 0;
    OCR2B = 0;

    /* Configuración del Clock Select (CS22:20):
     * Aplicamos máscara 0x07 para proteger el resto del registro y 
     * cargamos el valor del enum para arrancar el temporizador. */
    TCCR2B = (TCCR2B & ~0x07) | (prescaler & 0x07);
}

/**
 * @brief Conecta el pin físico (PB3 o PD3) a la lógica del Timer.
 * @details 
 * Manipula los bits Compare Output Mode (COM2x1:0) del TCCR2A.
 * - Non-Inverting: Pin en HIGH al inicio, LOW al coincidir con el Duty.
 * - Inverting: Pin en LOW al inicio, HIGH al coincidir con el Duty.
 * @param channel Canal A (Pin PB3) o Canal B (Pin PD3).
 * @param mode Modo de inversión según Table 17-3 o 17-6.
 */
void Timer2_PWM_Fast_EnableChannel(t2_pwm_channel_t channel, t2_pwm_mode_t mode) {
    if (channel == T2_PWM_CH_A) {
        // Usamos tu driver para configurar PB3 como SALIDA
        GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT); 
        
        // Configuración de registros del Timer
        TCCR2A = (TCCR2A & ~((1 << COM2A1) | (1 << COM2A0))) | (mode << COM2A0);
    } 
    else {
        // Usamos tu driver para configurar PD3 como SALIDA
        GPIO_InitPin(GPIO_D, 3, GPIO_OUTPUT);
        
        TCCR2A = (TCCR2A & ~((1 << COM2B1) | (1 << COM2B0))) | (mode << COM2B0);
    }
}

/**
 * @brief Desconecta el canal de la lógica del temporizador.
 * @details 
 * Setea los bits COM2x1:0 = 0. El pin deja de estar bajo control del 
 * Timer y recupera su función de puerto digital estándar (GPIO).
 * @param channel Canal A o B a desactivar.
 */
void Timer2_PWM_Fast_DisableChannel(t2_pwm_channel_t channel) {
    if (channel == T2_PWM_CH_A) {
        /* COM2A1:0 = 00 -> Normal port operation, OC2A disconnected. */
        TCCR2A &= ~((1 << COM2A1) | (1 << COM2A0));
    } else {
        TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0));
    }
}

/**
 * @brief Actualiza el registro de comparación (Duty Cycle).
 * @param channel Canal seleccionado (A o B).
 * @param duty Valor de 8 bits (0-255).
 */
void Timer2_PWM_Fast_SetDuty(t2_pwm_channel_t channel, uint8_t duty) {
    if (channel == T2_PWM_CH_A) {
        OCR2A = duty;
    } else {
        OCR2B = duty;
    }
}

/* --- Gestión de Interrupciones (Registro TIMSK2) --- */

/**
 * @brief Control de la interrupción por desbordamiento (Overflow).
 * @param state 1 para habilitar el bit TOIE2, 0 para deshabilitar.
 */
void Timer2_PWM_IT_Overflow(uint8_t state) {
    if (state) {
        TIMSK2 |= (1 << TOIE2);
    } else {
        TIMSK2 &= ~(1 << TOIE2);
    }
}

/**
 * @brief Control de la interrupción por coincidencia de comparación.
 * @param channel Canal A o B.
 * @param state 1 para habilitar el bit OCIE2x, 0 para deshabilitar.
 */
void Timer2_PWM_IT_CompareMatch(t2_pwm_channel_t channel, uint8_t state) {
    if (channel == T2_PWM_CH_A) {
        if (state) TIMSK2 |= (1 << OCIE2A);
        else TIMSK2 &= ~(1 << OCIE2A);
    } else {
        if (state) TIMSK2 |= (1 << OCIE2B);
        else TIMSK2 &= ~(1 << OCIE2B);
    }
}