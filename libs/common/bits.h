/**
 * @file bits.h
 * @brief Macros fundamentales para la manipulación atómica lógica de bits.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este archivo constituye la Capa 0 de la arquitectura. Proporciona
 * una interfaz abstracta para la manipulación de registros de hardware,
 * mejorando la legibilidad del código y reduciendo errores manuales de bit-shifting.
 */

#ifndef BITS_H_
#define BITS_H_

/**
 * @name Operaciones de Escritura
 * Macros para modificar el estado de un bit específico en un registro.
 * @{
 */

/**
 * @brief Establece (pone a 1) un bit específico.
 * @param REG Registro de hardware (ej. PORTB).
 * @param BIT Posición del bit (0-7 para registros de 8 bits).
 */
#define SET_BIT(REG, BIT)     ((REG) |= (1 << (BIT)))

/**
 * @brief Limpia (pone a 0) un bit específico.
 * @param REG Registro de hardware (ej. DDRD).
 * @param BIT Posición del bit a limpiar.
 */
#define CLR_BIT(REG, BIT)     ((REG) &= ~(1 << (BIT)))

/**
 * @brief Invierte (toggle) el estado de un bit específico.
 * @param REG Registro de hardware.
 * @param BIT Posición del bit a conmutar.
 */
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1 << (BIT)))

/** @} */

/**
 * @name Operaciones de Lectura
 * Macros para consultar el estado de los bits.
 * @{
 */

/**
 * @brief Retorna el valor lógico de un bit.
 * @param REG Registro de entrada (ej. PINB).
 * @param BIT Posición del bit a leer.
 * @return 1 si el bit está en alto, 0 si está en bajo.
 */
#define READ_BIT(REG, BIT)     (((REG) >> (BIT)) & 1)

/** @} */

/**
 * @name Macros de Control de Flujo
 * Macros optimizadas para su uso en condicionales (if/while).
 * @{
 */

/**
 * @brief Evalúa si un bit específico está en 1 (Set).
 * @param REG Registro a evaluar.
 * @param BIT Posición del bit.
 * @return Valor distinto de cero si el bit está seteado.
 */
#define BIT_IS_SET(REG, BIT)  ((REG) & (1 << (BIT)))

/**
 * @brief Evalúa si un bit específico está en 0 (Clear).
 * @param REG Registro a evaluar.
 * @param BIT Posición del bit.
 * @return Valor distinto de cero si el bit está limpio.
 */
#define BIT_IS_CLR(REG, BIT)  (!((REG) & (1 << (BIT))))

/** @} */

#endif /* BITS_H_ */