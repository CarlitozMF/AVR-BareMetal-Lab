# 📂 Capa 0: Common & Utilities (`libs/common`)

Este directorio contiene las definiciones fundamentales y macros de utilidad que constituyen la base lógica de todo el firmware. Al ser la **Capa 0**, es una librería "header-only" de alta eficiencia, sin dependencias externas y diseñada para operaciones atómicas.

## 🛠️ Módulos y Documentación

Cada archivo cuenta con documentación detallada bajo el estándar **Doxygen**. Podés acceder directamente a las definiciones haciendo clic en los archivos:

| Módulo | Descripción Técnica | Enlace al Código |
| :--- | :--- | :--- |
| **`bits.h`** | Macros para manipulación de bits (`SET`, `CLR`, `TOG`, `GET`). Garantiza operaciones seguras sobre registros. | [📄 Ver bits.h](./bits.h) |

---

## 🚀 Guía de Referencia Rápida (Cheat Sheet)

Para una integración veloz en las capas superiores, utilizá las siguientes macros documentadas en [`bits.h`](./bits.h):

### 1. Manipulación de Registros
* **Poner en alto:** `SET_BIT(PORTB, 5);`  // Documentado como `@brief Sets a specific bit`
* **Poner en bajo:** `CLR_BIT(PORTB, 5);`  // Documentado como `@brief Clears a specific bit`
* **Alternar estado:** `TOG_BIT(PORTB, 5);` // Documentado como `@brief Toggles a specific bit`

### 2. Lectura y Evaluación
* **Obtener estado:** `GET_BIT(PINB, 5);`  // Retorna el valor del bit (0 o !=0)
* **Evaluar condición:** ```c
    if (BIT_IS_SET(PINB, 5)) { /* ... */ } // Retorna true si el bit es 1
    if (BIT_IS_LOW(PINB, 5)) { /* ... */ } // Retorna true si el bit es 0
    ```

---

## 🛡️ Robustez Bare-Metal

El uso de estas macros en lugar de operaciones manuales (`|=`, `&=`) tiene como objetivo:
1.  **Evitar errores de tipeo:** Reducir la posibilidad de equivocarse en el desplazamiento de bits.
2.  **Abstracción Semántica:** Leer `SET_BIT` es mucho más intuitivo en una revisión de código que leer una operación de máscara.
3.  **Mantenibilidad:** Toda la lógica de manipulación de bits del proyecto se centraliza en un solo archivo documentado.

> [!NOTE]
> Todos los parámetros (`@param`) y valores de retorno (`@return`) están especificados dentro de los archivos de cabecera para facilitar el autocompletado en **VS Code**.