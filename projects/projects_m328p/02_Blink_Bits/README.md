# 🔢 Lab 02: Blink Bits

## 🎯 1. Título y Objetivos
- **Título:** Abstracción de Bits mediante Macros de Preprocesador.
- **Objetivos:** * 🧩 Introducir la **Capa 0 (Common)** del proyecto.
    * 🛠️ Implementar macros de manipulación de bits (`SET_BIT`, `CLR_BIT`) para mejorar la legibilidad del código.
    * 📂 Gestionar inclusiones mediante rutas relativas hacia librerías compartidas.

---

## 📖 2. Teoría de Operación
En el desarrollo profesional de firmware, la legibilidad es tan importante como la eficiencia. En lugar de escribir `PORTB |= (1 << PB0)`, utilizamos **Macros de C**. Estas son instrucciones para el preprocesador que reemplazan el texto de la macro por la operación lógica correspondiente antes de la compilación.

Esto nos permite:
1. **Evitar errores humanos:** Es más fácil equivocarse en un desplazamiento de bits que al escribir `SET_BIT`.
2. **Portabilidad:** Si la sintaxis de manipulación cambiara, solo tendríamos que modificar el archivo `bits.h`.



---

## 🏗️ 3. Arquitectura del Software

Este laboratorio introduce la **Capa 0 (Common)**, el cimiento de nuestra HAL. El archivo `bits.h` actúa como un motor de abstracción aritmética que convierte operaciones lógicas en instrucciones de hardware eficientes.

**Contenido de la Capa 0:**
* **Escritura:** `SET_BIT`, `CLR_BIT`, `TOGGLE_BIT`.
* **Lectura y Control:** `READ_BIT`, `BIT_IS_SET`, `BIT_IS_CLR`.

**Estructura de archivos:**

```text
.
├── libs/
│   └── common/
│       └── bits.h  <-- Definición de macros
└── projects/
    └──projects_m328p
        └── 02_Blink_Bits/
            └── main.c  <-- Aplicación
```

---

## 🛡️ 4. Detalles de Robustez

- **🧬 Abstracción de Lectura:** A diferencia del Lab 01, aquí introducimos la capacidad de consultar el hardware de forma segura. `READ_BIT` desplaza el bit a la posición 0, normalizando el resultado a un valor booleano puro (**0 o 1**), lo que evita errores de comparación en lógica condicional compleja.
- **🛡️ Macros de Control de Flujo:** `BIT_IS_SET` y `BIT_IS_CLR` permiten escribir código mucho más descriptivo y menos propenso a errores de interpretación:
    * *Ej: `if (BIT_IS_SET(PINB, PB0))` es infinitamente más legible que `if (PINB & (1 << PB0))`.*
- **💎 Seguridad de Paréntesis:** Todas las macros en `bits.h` envuelven sus argumentos entre paréntesis `(REG)` y `(BIT)`. Esto es una **buena práctica de ingeniería** que evita errores de precedencia de operadores cuando se pasan expresiones matemáticas o lógicas complejas como argumentos.

---

## 🗺️ 5. Mapeo de Hardware

| Periférico | Pin AVR | Función | Macro Utilizada |
| :--- | :--- | :--- | :--- |
| 🔴 LED 1 | PB0 | Salida Digital | `SET_BIT` / `CLR_BIT` |
| 🔵 LED 2 | PB3 | Salida Digital | `SET_BIT` / `CLR_BIT` |

---

## 🏁 6. Conclusión
El código resultante es mucho más "limpio" y fácil de mantener. Hemos pasado de manipular bits manualmente a utilizar un **lenguaje de servicios**. Este es el cimiento necesario para el **Lab 03**, donde dejaremos de hablar de "pines y bits" para empezar a hablar de "Objetos GPIO".

---

## 📚 7. Referencias
* **AVR Libc - `<avr/io.h>`:** [Documentación Oficial](https://www.nongnu.org/avr-libc/user-manual/group__avr__io.html)
* **C Preprocessor Macros:** [GCC Reference](https://gcc.gnu.org/onlinedocs/cpp/Macros.html)