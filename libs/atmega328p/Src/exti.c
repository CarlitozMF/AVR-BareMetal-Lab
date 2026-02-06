/**
 * @file exti.c
 * @author CarlitozMF
 * @brief Implementación del driver de Capa 1 (HAL) para Interrupciones Externas.
 * * Este módulo gestiona la configuración de los registros EICRA y EIMSK del 
 * ATmega328P, permitiendo una transición de un modelo de Polling a uno 
 * orientado a eventos por hardware.
 */

#include "exti.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Inicializa una línea de interrupción externa (INTx).
 * * Realiza la configuración del sentido de disparo (Sense Control) y habilita
 * la máscara local de interrupción. Finalmente, habilita las interrupciones
 * globales para permitir el salto al vector correspondiente.
 * * @param line  Identificador de la línea física (@ref EXTI_INT0 o @ref EXTI_INT1).
 * @param trigger Modo de detección (Nivel, Flanco de Subida, Bajada o Cualquier Cambio).
 */
void EXTI_Init(exti_line_t line, exti_trigger_t trigger) {
    
    if (line == EXTI_INT0) {
        /**
         * Configuración de INT0 (Pin PD2):
         * 1. Limpiar bits ISC01 e ISC00 en EICRA.
         * 2. Cargar el valor del trigger desplazado a la posición ISC00.
         */
        EICRA = (EICRA & ~((1 << ISC01) | (1 << ISC00))) | (trigger << ISC00);
        
        /**
         * Habilitación de la interrupción específica INT0.
         * Se utiliza la macro SET_BIT definida en bits.h.
         */
        SET_BIT(EIMSK, INT0);
    } 
    else if (line == EXTI_INT1) {
        /**
         * Configuración de INT1 (Pin PD3):
         * 1. Limpiar bits ISC11 e ISC10 en EICRA.
         * 2. Cargar el valor del trigger desplazado a la posición ISC10.
         */
        EICRA = (EICRA & ~((1 << ISC11) | (1 << ISC10))) | (trigger << ISC10);
        
        /**
         * Habilitación de la interrupción específica INT1.
         */
        SET_BIT(EIMSK, INT1);
    }

    /**
     * @note sei() habilita el bit I del registro SREG. Es crucial para que 
     * el controlador de interrupciones pueda desviar el flujo hacia la ISR.
     */
    sei(); 
}

/**
 * @brief Deshabilita la línea de interrupción externa.
 * * @param line Línea a enmascarar (INT0 o INT1).
 */
void EXTI_Disable(exti_line_t line) {
    if (line == EXTI_INT0) {
        /**
         * Se limpia el bit de máscara en EIMSK para ignorar eventos en INT0.
         */
        CLR_BIT(EIMSK, INT0);
    } else if (line == EXTI_INT1) {
        /**
         * Se limpia el bit de máscara en EIMSK para ignorar eventos en INT1.
         */
        CLR_BIT(EIMSK, INT1);
    }
}