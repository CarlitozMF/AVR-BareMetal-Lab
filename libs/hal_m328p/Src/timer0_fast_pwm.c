/**
 * @file timer0_fast_pwm.c
 * @brief Implementación de la Capa 1 (HAL) para Fast PWM en el Timer 0.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este archivo implementa el control de hardware para el modo Fast PWM de 8 bits.
 * Se enfoca en la manipulación directa de registros (TCCR0A/B, OCR0A/B, TIMSK0) para 
 * garantizar el menor consumo de recursos y la máxima previsibilidad en sistemas de tiempo real.
 */

#include "timer0_fast_pwm.h"
#include "gpio.h"

/**
 * @brief Inicializa el hardware del Timer 0 para Fast PWM.
 * @details 
 * 1. Setea el modo WGM01:0 a 3 (Fast PWM) mediante el registro TCCR0A.
 * 2. Limpia el registro TCCR0B para asegurar que WGM02 sea 0 (TOP en 0xFF).
 * 3. Carga la fuente de reloj (Prescaler) en TCCR0B.
 * @param prescaler Selección de reloj según Table 14-9 del datasheet.
 */
void Timer0_PWM_Init(t0_pwm_prescaler_t prescaler) {
    /* Configuración de Waveform Generation Mode (WGM):
     * WGM01=1, WGM00=1 -> Fast PWM, Modo 3.
     * En este modo el contador TCNT0 recorre de 0x00 a 0xFF. */
    TCCR0A = (1 << WGM01) | (1 << WGM00);
    
    /* Forzamos el registro B a cero para garantizar WGM02 = 0 (TOP fijo en 255)
     * y limpiar cualquier configuración previa de reloj. */
    TCCR0B = 0; 
    /*RESET DE SEGURIDAD:
     * Forzamos el Duty a 0 para evitar que el LED inicie con el "glitch" 
     * del último estado antes del reset. */
    OCR0A = 0;
    OCR0B = 0;
    /* Configuración del Clock Select (CS02:CS00):
     * Aplicamos una máscara (0x07) para asegurar que solo modificamos los 3 bits
     * de prescaler sin alterar el resto de la configuración. */
    TCCR0B = (TCCR0B & ~0x07) | (prescaler & 0x07);
}

/**
 * @brief Conecta el pin físico (OC0A o OC0B) a la lógica del Timer.
 * @details 
 * Utiliza los bits Compare Output Mode (COM).
 * - Non-Inverting: Pin HIGH en BOTTOM, LOW en Compare Match (Duty directo).
 * - Inverting: Pin LOW en BOTTOM, HIGH en Compare Match (Duty inverso).
 * @param channel Canal A (PD6) o B (PD5).
 * @param mode Modo de inversión según Table 14-3/14-6.
 */
void Timer0_PWM_EnableChannel(t0_pwm_channel_t channel, t0_pwm_mode_t mode) {
    if (channel == T0_PWM_CH_A) {
        /* Configurar Pin PD6 (OC0A) como SALIDA usando tu driver GPIO */
        GPIO_InitPin(GPIO_D, 6, GPIO_OUTPUT);
        
        /* Limpiamos COM0A1:0 (bits 7:6) y seteamos el nuevo modo. */
        TCCR0A = (TCCR0A & ~((1 << COM0A1) | (1 << COM0A0))) | (mode << COM0A0);
    } else {
        /* Configurar Pin PD5 (OC0B) como SALIDA usando tu driver GPIO */
        GPIO_InitPin(GPIO_D, 5, GPIO_OUTPUT);
        
        /* Limpiamos COM0B1:0 (bits 5:4) y seteamos el nuevo modo. */
        TCCR0A = (TCCR0A & ~((1 << COM0B1) | (1 << COM0B0))) | (mode << COM0B0);
    }
}

/**
 * @brief Desconecta el canal seleccionado.
 * @details 
 * Setea los bits COMnx en 0. El pin deja de ser controlado por el Timer
 * y vuelve a funcionar como un GPIO estándar controlado por el registro PORT.
 * @param channel Canal a desactivar.
 */
void Timer0_PWM_DisableChannel(t0_pwm_channel_t channel) {
    if (channel == T0_PWM_CH_A) {
        /* COM0A1:0 = 0 -> Normal port operation, OC0A disconnected. */
        TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));
    } else {
        TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0));
    }
}

/**
 * @brief Actualiza el registro de comparación para variar el Duty Cycle.
 * @param channel Canal seleccionado.
 * @param duty Valor entre 0 y 255 (0% a 100% en modo Non-Inverting).
 */
void Timer0_PWM_SetDuty(t0_pwm_channel_t channel, uint8_t duty) {
    if (channel == T0_PWM_CH_A) {
        OCR0A = duty;
    } else {
        OCR0B = duty;
    }
}

/**
 * @brief Control de la interrupción por desbordamiento.
 * @param state 1 para habilitar TOIE0, 0 para deshabilitar.
 */
void Timer0_PWM_IT_Overflow(uint8_t state) {
    if (state) {
        /* Setea bit Timer/Counter0 Overflow Interrupt Enable */
        TIMSK0 |= (1 << TOIE0);
    } else {
        TIMSK0 &= ~(1 << TOIE0);
    }
}

/**
 * @brief Control de interrupciones por coincidencia de comparación.
 * @param channel Canal A o B.
 * @param state 1 para habilitar OCIE0x, 0 para deshabilitar.
 */
void Timer0_PWM_IT_CompareMatch(t0_pwm_channel_t channel, uint8_t state) {
    if (channel == T0_PWM_CH_A) {
        if (state) TIMSK0 |= (1 << OCIE0A);
        else TIMSK0 &= ~(1 << OCIE0A);
    } else {
        if (state) TIMSK0 |= (1 << OCIE0B);
        else TIMSK0 &= ~(1 << OCIE0B);
    }
}