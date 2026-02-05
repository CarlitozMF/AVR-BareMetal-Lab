# 🏛️ Lab 03: Blink GPIO Driver

## 🎯 1. Título y Objetivos
- **Título:** Implementación de Driver GPIO mediante Estructuras y Punteros.
- **Objetivos:** * 🏗️ Diseñar la **Capa 1 (Hardware Abstraction Layer)**.
    * 🧬 Utilizar **Estructuras y Punteros Volátiles** para mapear registros de hardware.
    * 🛡️ Implementar **Enumeraciones (Enums)** para garantizar la robustez de los argumentos de funciones.

---

## 📖 2. Teoría de Operación
La clave de este laboratorio es el **Mapeo Estructural de Memoria**. En lugar de acceder a `PORTB` como una macro global, definimos una estructura `gpio_port_t` que agrupa los tres registros relacionados (DDR, PORT, PIN).

### El uso de `volatile` y Punteros:
Al declarar `volatile uint8_t *PORT`, le indicamos al compilador que el valor en esa dirección de memoria puede cambiar fuera del flujo del programa (por el hardware). Esto evita que el optimizador del compilador elimine lecturas o escrituras que considera "redundantes", asegurando que cada instrucción llegue al silicio.



---

## 🏗️ 3. Arquitectura del Software
Este laboratorio consolida la arquitectura de tres capas:

- **Capa 0 (Common):** `bits.h` provee las operaciones atómicas de bits.
- **Capa 1 (Drivers):** `gpio.h/c` encapsula la complejidad del hardware en funciones semánticas como `GPIO_WritePin`.
- **Capa 3 (Aplicación):** El `main.c` ya no conoce direcciones de memoria, solo invoca servicios del driver.

**Estructura del Driver:**
* **Enums:** Definen estados estrictos (`GPIO_HIGH`, `GPIO_LOW`) evitando que el usuario pase valores inválidos.
* **Estructura de Puerto:** Mapea de forma genérica cualquier puerto del ATmega328P.

---

## 🛡️ 4. Detalles de Robustez
- **🧬 Encapsulamiento:** El uso de `port.DDR` dentro de las funciones del driver permite que el mismo código funcione para el Puerto B, C o D sin cambios.
- **💎 Tipado Fuerte:** Al usar `gpio_mode_t` en lugar de un simple `int` o `uint8_t`, el compilador ayuda a prevenir errores de lógica.
- **⚡ Optimización por Toggle:** Se implementó `GPIO_TogglePin` utilizando la macro `TOGGLE_BIT`, permitiendo alternar estados con una sola instrucción de procesador, ideal para señalización.

---

## 🗺️ 5. Mapeo de Hardware
| Periférico | Pin AVR | Función | Driver Service |
| :--- | :--- | :--- | :--- |
| 🔴 LED 1 | PB0 | Salida Digital | `GPIO_TogglePin` |
| 🔵 LED 2 | PB3 | Salida Digital | `GPIO_TogglePin` |

---

## 🏁 6. Conclusión
Este laboratorio transforma la placa de desarrollo en un sistema profesional. Hemos logrado que la aplicación (`main.c`) sea **independiente del hardware específico**. Si cambiáramos de microcontrolador, solo tendríamos que reescribir la Capa 1, manteniendo la lógica de la aplicación intacta. Este es el corazón de la **ingeniería soberana**.

---

## 📚 7. Referencias
* **Punteros Volátiles en C:** [Embedded.com Documentation](https://www.embedded.com/introduction-to-the-volatile-keyword/)
* **Arquitectura de Capas:** [ISO/OSI Model Concept applied to Embedded Systems](https://en.wikipedia.org/wiki/Abstraction_layer)