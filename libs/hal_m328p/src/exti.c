/**
 * @file exti.c
 * @author Mamani Flores Carlos
 * @brief Implementación del driver de Capa 1 (HAL) para Interrupciones Externas.
 * @date 2026
 * * @details Este módulo gestiona la configuración de los registros EICRA (Sense Control) 
 * y EIMSK (Interrupt Mask) del ATmega328P. Permite la transición de un modelo de 
 * Polling a uno orientado a eventos por hardware, garantizando una respuesta 
 * determinística ante estímulos externos en los pines PD2 y PD3.
 */

#include "exti.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Inicializa y habilita una línea de interrupción externa (INTx).
 * * @details Realiza una operación de bit-masking para configurar el sentido de disparo 
 * en EICRA sin alterar la configuración de la otra línea INTx. Posteriormente,
 * habilita la máscara local y activa las interrupciones globales.
 * * @param line   Identificador de la línea física (EXTI_INT0 o EXTI_INT1).
 * @param trigger Modo de detección (Nivel, Flanco de Subida, Bajada o Toggle).
 * * @note Esta función invoca sei() al finalizar. Asegúrese de que las ISR correspondientes
 * (ISR(INT0_vect) o ISR(INT1_vect)) estén implementadas para evitar un reset del sistema.
 */
void EXTI_Init(exti_line_t line, exti_trigger_t trigger) {
    
    if (line == EXTI_INT0) {
        /**
         * Configuración de INT0 (Pin PD2):
         * 1. Limpiar bits ISC01 e ISC00 en EICRA mediante máscara AND (~).
         * 2. Aplicar el valor del trigger mediante OR (|) desplazado a la posición ISC00.
         */
        EICRA = (EICRA & ~((1 << ISC01) | (1 << ISC00))) | (trigger << ISC00);
        
        /**
         * Habilitación de la máscara local en el registro EIMSK.
         * Se utiliza la macro SET_BIT de la Capa 0 (bits.h).
         */
        SET_BIT(EIMSK, INT0);
    } 
    else if (line == EXTI_INT1) {
        /**
         * Configuración de INT1 (Pin PD3):
         * 1. Limpiar bits ISC11 e ISC10 en EICRA.
         * 2. Aplicar el valor del trigger desplazado a la posición ISC10.
         */
        EICRA = (EICRA & ~((1 << ISC11) | (1 << ISC10))) | (trigger << ISC10);
        
        /**
         * Habilitación de la máscara local para INT1.
         */
        SET_BIT(EIMSK, INT1);
    }

    /**
     * @note sei() activa el bit I (Global Interrupt Enable) en el registro SREG.
     * Es imperativo que este bit esté en 1 para que el hardware procese cualquier ISR.
     */
    sei(); 
}

/**
 * @brief Deshabilita la línea de interrupción externa mediante máscara local.
 * * @param line Línea a enmascarar (INT0 o INT1).
 * * @details Esta función utiliza CLR_BIT para poner en 0 el bit correspondiente en EIMSK.
 * El evento de hardware se ignorará, pero el bit global de interrupciones permanecerá inalterado.
 */
void EXTI_Disable(exti_line_t line) {
    if (line == EXTI_INT0) {
        /** Limpia el bit de máscara local para INT0 */
        CLR_BIT(EIMSK, INT0);
    } else if (line == EXTI_INT1) {
        /** Limpia el bit de máscara local para INT1 */
        CLR_BIT(EIMSK, INT1);
    }
}