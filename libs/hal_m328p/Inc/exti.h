/**
 * @file exti.h
 * @brief Driver de Capa 1 (HAL) para la gestión de Interrupciones Externas en ATmega328P.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este módulo permite configurar las líneas de interrupción por hardware (INT0 e INT1).
 * La programación reactiva mediante EXTI es fundamental para el manejo eficiente de eventos 
 * asíncronos como pulsadores, sensores de velocidad o señales de sincronismo, liberando 
 * al CPU de la carga de procesamiento del polling constante.
 */

#ifndef EXTI_H_
#define EXTI_H_

#include <stdint.h>
#include "bits.h"

/** * @name Configuraciones de Disparo
 * @{ 
 */

/**
 * @enum exti_trigger_t
 * @brief Modos de disparo (Sense Control) para las interrupciones externas.
 * @note Los valores numéricos están alineados con la configuración de los bits 
 * ISCnx en el registro EICRA para optimizar la escritura en el driver.
 */
typedef enum {  
    EXTI_LOW_LEVEL    = 0x00,   /**< Disparo continuo mientras el pin esté en LOW. */
    EXTI_ANY_LOGIC    = 0x01,   /**< Disparo por cualquier cambio de estado (Toggle). */
    EXTI_FALLING_EDGE = 0x02,   /**< Disparo por flanco de bajada (High to Low). */
    EXTI_RISING_EDGE  = 0x03    /**< Disparo por flanco de subida (Low to High). */
} exti_trigger_t;

/** @} */

/** * @name Definición de Líneas Hardware
 * @{ 
 */

/**
 * @enum exti_line_t
 * @brief Identificador de la línea de interrupción física disponible en el MCU.
 */
typedef enum { 
    EXTI_INT0,  /**< Interrupción externa 0 - Mapeada al pin PD2 */
    EXTI_INT1   /**< Interrupción externa 1 - Mapeada al pin PD3 */
} exti_line_t;

/** @} */

/* --- API Pública de Capa 1 --- */

/**
 * @brief Configura e habilita una línea de interrupción externa.
 * * @details Realiza la configuración del registro EICRA (External Interrupt Control Register A)
 * para definir el trigger y activa la máscara en EIMSK. 
 * * @param line Línea de interrupción a configurar (@ref exti_line_t).
 * @param trigger Modo de flanco o nivel deseado (@ref exti_trigger_t).
 * * @note Para que la interrupción sea efectiva, se deben habilitar las interrupciones 
 * globales mediante la instrucción `sei()` en la aplicación principal.
 */
void EXTI_Init(exti_line_t line, exti_trigger_t trigger);

/**
 * @brief Deshabilita la máscara local de la interrupción externa especificada.
 * * @param line Línea a deshabilitar (INT0 o INT1).
 * @note Esta función no afecta el bit global de interrupciones (I), solo la máscara local.
 */
void EXTI_Disable(exti_line_t line);

#endif /* EXTI_H_ */