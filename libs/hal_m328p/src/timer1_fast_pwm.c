/**
 * @file timer1_fast_pwm.c
 * @brief Implementación de la Capa 1 (HAL) para Fast PWM en el Timer 1 (16-bits).
 * @author Mamani Flores Carlos
 * @date 2026
 *
 * @details Este driver implementa el Modo 14 (Fast PWM con TOP en ICR1). A diferencia
 * de los modos de 8 bits, aquí se tiene control total sobre el periodo de la señal,
 * lo que permite generar frecuencias exactas para servomotores (50Hz) o aplicaciones
 * de control industrial. Se garantiza la integridad de los registros de 16 bits
 * mediante el uso de tipos uint16_t gestionados por el compilador avr-gcc.
 */

#include "timer1_fast_pwm.h"
#include "gpio.h"

/**
 * @brief Inicializa el Timer 1 para operar en modo Fast PWM (Modo 14).
 * @details 
 * 1. Configura WGM13:10 = 1110 (Modo 14) repartido en TCCR1A y TCCR1B.
 * 2. Carga el registro de captura (ICR1) que actúa como el valor TOP del contador.
 * 3. Aplica el prescaler seleccionado iniciando el conteo.
 * @param prescaler Fuente de reloj (N) según Table 15-5 del datasheet.
 * @param top_value Valor máximo del contador (Define la frecuencia: F = F_CPU / (N * (1 + TOP))).
 */
void Timer1_PWM_Fast_Init(t1_pwm_prescaler_t prescaler, uint16_t top_value) {
    /* Configuración de Waveform Generation Mode (WGM):
     * WGM11=1 (TCCR1A) y WGM13:12=11 (TCCR1B). 
     * El bit WGM10 queda en 0. Esto selecciona el Modo 14 (TOP en ICR1). */
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);
    
    /* Carga del registro de captura ICR1:
     * Al ser un registro de 16 bits, es vital cargarlo antes de activar las salidas
     * para definir el periodo desde el primer ciclo. El compilador maneja el 
     * acceso atómico a través del registro TEMP de 8 bits del microcontrolador. */
    ICR1 = top_value;

    /* RESET DE SEGURIDAD:
     * Forzamos los registros de comparación a 0 antes de arrancar el clock. */
    OCR1A = 0;
    OCR1B = 0;

    /* Configuración del Clock Select (CS12:10):
     * Se aplica una máscara (0x07) para modificar solo los bits de prescaler.
     * Esto arranca el temporizador con la fuente de reloj elegida. */
    TCCR1B = (TCCR1B & ~0x07) | (prescaler & 0x07);
}

/**
 * @brief Habilita y configura el comportamiento del pin físico (PB1 o PB2).
 * @details 
 * Utiliza los bits Compare Output Mode (COM1x1:0).
 * - Modo Non-Inverting: El pin se pone en HIGH al iniciar (BOTTOM) y en LOW al coincidir.
 * - Modo Inverting: El pin se pone en LOW al iniciar y en HIGH al coincidir.
 * @param channel Canal A (PB1) o Canal B (PB2).
 * @param mode Modo de inversión (Inverting/Non-Inverting) según Table 15-2.
 */
void Timer1_PWM_Fast_EnableChannel(t1_pwm_channel_t channel, t1_pwm_mode_t mode) {
    if (channel == T1_PWM_CH_A) {
        /* Configurar Pin PB1 como SALIDA usando tu HAL de GPIO */
        GPIO_InitPin(GPIO_B, 1, GPIO_OUTPUT);
        
        /* Limpiamos COM1A1:0 (bits 7:6) y cargamos el modo */
        TCCR1A = (TCCR1A & ~((1 << COM1A1) | (1 << COM1A0))) | (mode << COM1A0);
    } else {
        /* Configurar Pin PB2 como SALIDA usando tu HAL de GPIO */
        GPIO_InitPin(GPIO_B, 2, GPIO_OUTPUT);
        
        /* Limpiamos COM1B1:0 (bits 5:4) y cargamos el modo */
        TCCR1A = (TCCR1A & ~((1 << COM1B1) | (1 << COM1B0))) | (mode << COM1B0);
    }
}

/**
 * @brief Desconecta el canal de la lógica del Timer.
 * @details 
 * Setea COM1x1:0 = 0. El pin asociado (PB1 o PB2) vuelve a operar como 
 * entrada o salida digital estándar (GPIO) según la configuración del DDRB.
 * @param channel Canal a desactivar.
 */
void Timer1_PWM_Fast_DisableChannel(t1_pwm_channel_t channel) {
    if (channel == T1_PWM_CH_A) {
        /* COM1A1:0 = 00 -> Operación normal de puerto, pin desconectado del Timer. */
        TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
    } else {
        TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
    }
}

/**
 * @brief Establece el ancho de pulso (Duty Cycle) mediante el registro de comparación.
 * @details 
 * El valor cargado en OCR1A/B debe ser siempre menor o igual al TOP (ICR1).
 * Al usar Fast PWM, los registros OCR1x poseen doble buffer; el nuevo valor
 * solo se hace efectivo cuando el contador TCNT1 alcanza el valor TOP.
 * @param channel Canal seleccionado (A o B).
 * @param val Valor de comparación de 16 bits.
 */
void Timer1_PWM_Fast_SetCompare(t1_pwm_channel_t channel, uint16_t val) {
    if (channel == T1_PWM_CH_A) {
        OCR1A = val;
    } else {
        OCR1B = val;
    }
}

/* --- Gestión de Interrupciones (Registro TIMSK1) --- */

/**
 * @brief Habilita/Deshabilita la interrupción por desbordamiento del Timer 1.
 * @param state 1 para activar TOIE1, 0 para desactivar.
 */
void Timer1_PWM_IT_Overflow(uint8_t state) {
    if (state) TIMSK1 |= (1 << TOIE1);
    else TIMSK1 &= ~(1 << TOIE1);
}

/**
 * @brief Habilita/Deshabilita la interrupción por coincidencia de comparación.
 * @param channel Canal A o B.
 * @param state 1 para activar OCIE1x, 0 para desactivar.
 */
void Timer1_PWM_IT_CompareMatch(t1_pwm_channel_t channel, uint8_t state) {
    if (channel == T1_PWM_CH_A) {
        if (state) TIMSK1 |= (1 << OCIE1A);
        else TIMSK1 &= ~(1 << OCIE1A);
    } else {
        if (state) TIMSK1 |= (1 << OCIE1B);
        else TIMSK1 &= ~(1 << OCIE1B);
    }
}