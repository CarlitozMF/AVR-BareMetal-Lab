/**
 * @file exti.h
 * @author CarlitozMF
 * @brief Driver de Capa 1 (HAL) para la gestión de Interrupciones Externas en ATmega328P.
 * @version 1.0
 * @date 2026-02-05
 * * @copyright Desarrollado como parte de la carpeta de Proyectos Integradores.
 */

#ifndef EXTI_H_
#define EXTI_H_

#include <stdint.h>
#include "bits.h"

/**
 * @brief Modos de disparo (Sense Control) para las interrupciones externas.
 * Los valores numéricos coinciden con la configuración de los bits ISCnx en EICRA.
 */
typedef enum {
    EXTI_LOW_LEVEL    = 0x00, /**< Interrupción se dispara mientras el pin esté en nivel bajo. */
    EXTI_ANY_LOGIC    = 0x01, /**< Cualquier cambio lógico (subida o bajada) genera la interrupción. */
    EXTI_FALLING_EDGE = 0x02, /**< Disparo únicamente en el flanco de bajada (1 a 0). */
    EXTI_RISING_EDGE  = 0x03  /**< Disparo únicamente en el flanco de subida (0 a 1). */
} exti_trigger_t;

/**
 * @brief Identificador de la línea de interrupción física (INTx).
 */
typedef enum {
    EXTI_INT0, /**< Pin PD2 del ATmega328P */
    EXTI_INT1  /**< Pin PD3 del ATmega328P */
} exti_line_t;

/**
 * @brief Configura e inicializa una línea de interrupción externa.
 * * Esta función ajusta los bits de control de sentido (Trigger) y habilita la máscara
 * local en el registro EIMSK. Finalmente activa el bit global de interrupciones.
 * * @param line Línea de interrupción a configurar (@ref exti_line_t).
 * @param trigger Modo de flanco o nivel deseado (@ref exti_trigger_t).
 */
void EXTI_Init(exti_line_t line, exti_trigger_t trigger);

/**
 * @brief Deshabilita la interrupción externa especificada.
 * * @param line Línea a deshabilitar (INT0 o INT1).
 */
void EXTI_Disable(exti_line_t line);

#endif /* EXTI_H_ */