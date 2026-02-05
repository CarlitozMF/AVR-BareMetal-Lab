#ifndef BITS_H_
#define BITS_H_

/**
 * @file bits.h
 * @brief Macros fundamentales para la manipulación de bits.
 * @author Mamani Flores Carlos
 */

/* Operaciones de Escritura */
#define SET_BIT(REG, BIT)     ((REG) |= (1 << (BIT)))   // Pone el bit en 1
#define CLR_BIT(REG, BIT)     ((REG) &= ~(1 << (BIT)))  // Pone el bit en 0
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1 << (BIT)))   // Invierte el estado del bit

/* Operaciones de Lectura */
// Retorna el valor del bit desplazado a la posición 0 (resultado: 0 o 1)
#define READ_BIT(REG, BIT)    (((REG) >> (BIT)) & 1)

/* Macros de Control de Flujo (Robustez) */
#define BIT_IS_SET(REG, BIT)  ((REG) & (1 << (BIT)))    // Evalúa si el bit es 1
#define BIT_IS_CLR(REG, BIT)  (!((REG) & (1 << (BIT)))) // Evalúa si el bit es 0

#endif /* BITS_H_ */