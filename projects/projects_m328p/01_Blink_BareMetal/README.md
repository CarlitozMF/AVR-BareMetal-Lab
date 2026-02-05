# Lab 01: Blink Bare Metal

## 1. Título y Objetivos
- **Título:** Control de Registros mediante Bit-Shifting Manual.
- **Objetivos:** * Comprender la relación directa entre el código C y los registros de hardware del ATmega328P.
    * Implementar el encendido y apagado de periféricos sin capas de abstracción.
    * Dominar las operaciones a nivel de bit (OR, AND, NOT) para configurar I/O.

---

## 2. Teoría de Operación
En esta etapa inicial, la interacción con el microcontrolador se realiza mediante el acceso directo a sus registros de memoria mapeados. En la arquitectura AVR, el control de los pines de propósito general (GPIO) se basa en tres registros principales:

* **DDRx (Data Direction Register):** Determina si cada pin de un puerto es una entrada (0) o una salida (1).
* **PORTx (Data Register):** Si el pin es salida, este registro define el estado lógico: Alto (1) o Bajo (0).
* **F_CPU:** Es una macro esencial que informa a las funciones de tiempo (`_delay_ms`) la frecuencia del reloj del sistema (16 MHz en nuestro caso) para calcular los retardos de forma precisa.



---

## 3. Arquitectura del Software
- **Descripción:** Este proyecto implementa una **Capa Única**. No existen drivers, servicios ni abstracciones externas; toda la lógica de control y configuración reside dentro del archivo `main.c`.
- **Flujo de Ejecución:**
    1.  **Configuración:** Se establece la dirección de los pines PB0 y PB3 como salidas utilizando una operación `OR` con desplazamiento de bits.
    2.  **Bucle de Eventos:** Un ciclo `while(1)` infinito que ejecuta la secuencia de encendido, espera, apagado y espera.

---

## 4. Detalles de Robustez
- **Manipulación de Bits Atómica (Manual):** Se utiliza la técnica de "máscara de bits" para asegurar que solo los pines PB0 y PB3 sean modificados, preservando el estado actual de los demás pines del Puerto B.
    * **Set:** `PORTB |= (1 << PB0)` (Pone a 1 sin afectar el resto).
    * **Clear:** `PORTB &= ~(1 << PB0)` (Pone a 0 sin afectar el resto).
- **Control de Frecuencia:** La definición explícita de `F_CPU` garantiza que los tiempos de parpadeo sean exactos según el cristal de cuarzo de la placa.

---

## 5. Mapeo de Hardware
| Periférico | Pin AVR | Función | Registro |
| :--- | :--- | :--- | :--- |
| LED 1 | PB0 | Salida Digital | DDRB / PORTB |
| LED 2 | PB3 | Salida Digital | DDRB / PORTB |

---

## 6. Conclusión
Este laboratorio constituye el "Kilómetro 0" del desarrollo profesional. Aunque el código es eficiente en términos de ciclos de instrucción, se vuelve difícil de mantener y portar conforme el proyecto crece. Esta experiencia como autodidacta justifica la necesidad de evolucionar hacia la **Capa 0 (bits.h)**, que desarrollaremos en el siguiente laboratorio para mejorar la legibilidad del código.