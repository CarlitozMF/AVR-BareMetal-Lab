# 🛠️ Capa 1: Hardware Abstraction Layer (HAL) - ATmega328P

Este directorio constituye el núcleo de interacción directa con el silicio. La **HAL** actúa como un puente entre los registros de hardware del MCU y las capas superiores, transformando direcciones de memoria complejas en una API semántica, segura y eficiente.

## 🏛️ Filosofía de Diseño

A diferencia de las librerías convencionales, esta HAL se basa en tres pilares de ingeniería embebida:
1. **Acceso Atómico y Volátil:** Uso mandatorio de `volatile uint8_t*` para asegurar que el compilador no optimice los accesos a los registros de E/S.
2. **Abstracción por Estructuras:** Los puertos y periféricos se agrupan en `structs`, permitiendo un código más limpio y orientado a objetos en C.
3. **Soberanía del Hardware:** Acceso total a las capacidades del ATmega328P (Timers de 16 bits, Input Capture, EXTI) sin las limitaciones de frameworks genéricos.

## 📦 Periféricos Implementados

### 🔌 [GPIO (General Purpose I/O)](./Inc/gpio.h)
El driver de GPIO permite una gestión dinámica de los puertos B, C y D.
* **Estructuras de Puerto:** Cada puerto se maneja como un objeto [`gpio_port_t`](./gpio.h) que encapsula los registros DDR, PORT y PIN.
* **Funciones Clave:** `GPIO_InitPin`, `GPIO_WritePin`, `GPIO_TogglePin`, `GPIO_ReadPin`.

### ⚡ [EXTI (External Interrupts)](./Inc/exti.h)
Gestión reactiva de eventos asíncronos en los pines PD2 (INT0) y PD3 (INT1).
* **Triggers:** Configuración por nivel bajo, flanco de subida, bajada o cualquier cambio lógico.
* **Uso:** Fundamental para interfaces HMI y sensores de velocidad sin carga de polling para el CPU.

### ⏱️ Orquesta de Timers
Contamos con una suite completa de temporización para diferentes resoluciones y propósitos:

* **[Systick](./Inc/systick.h):** Base de tiempo maestra de 1ms utilizando el **Timer 0**. Incluye **Secciones Críticas** para lecturas atómicas de 32 bits en una arquitectura de 8 bits.
* **[Timer 0 (8-bits)](./Inc/timer0_normal.h):** Temporización de propósito general y generación de eventos en pines OC0A/B.
* **[Timer 1 (16-bits)](./Inc/timer1_normal.h):** Driver de alta resolución (0-65535) para control de precisión (tren de pulsos del motor PaP) y módulo de **Input Capture** para medición de señales.
* **[Timer 2 (8-bits)](./Inc/timer2_normal.h):** Driver especializado con prescalers extendidos (32, 128) y soporte para **Modo Asíncrono**. Ideal para mantener un RTC mediante un cristal externo de 32.768kHz.

---

## 🚀 Ejemplo de Implementación: Lectura Atómica

En arquitecturas de 8 bits, leer una variable de 32 bits (como el acumulador de milisegundos del Systick) es una operación no atómica. Nuestra HAL resuelve esto mediante la gestión del registro de estado (`SREG`):

```c
uint32_t get_tick(void) {
    uint32_t tick_copy;
    uint8_t sreg_save = SREG; // Guardar estado global de interrupciones
    cli();                    // Sección Crítica: Bloqueo de interrupciones
    tick_copy = ms_ticks;     // Lectura segura de los 4 bytes
    SREG = sreg_save;         // Restaurar estado original
    return tick_copy;
}
```

---

## 🏗️ Arquitectura de la Carpeta

```plaintext
libs/hal_m328p/
├── Inc/                  # Archivos de cabecera (.h) - Definiciones de Registros y APIs
├── Src/                  # Archivos de implementación (.c) - Lógica de Acceso al Hardware
└── README.md             # Documentación técnica del módulo
```
## 📂 Organización de Archivos

| Módulo | Responsabilidad |
| :--- | :--- |
| **`gpio.h / .c`** | Abstracción de puertos mediante estructuras de punteros volátiles. |
| **`exti.h / .c`** | Gestión de interrupciones externas reactivas (INT0, INT1). |
| **`systick.h / .c`** | Latido del sistema (1ms) y gestión de secciones críticas. |
| **`timerX_normal.h / .c`** | Drivers específicos para la orquesta de Timers (0, 1 y 2). |

> [!TIP]
> Todos los controladores están diseñados para operar a una frecuencia de **16MHz**. Si se modifica la frecuencia del cristal del sistema (**F_CPU**), es imperativo verificar y ajustar las constantes de los *prescalers* en los archivos de cabecera para garantizar la precisión de las bases de tiempo.
