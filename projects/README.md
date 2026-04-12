# 📂 Directorio de Laboratorios y Proyectos

Este directorio contiene la progresión técnica desarrollada para el **ATmega328P** bajo una filosofía **Bare-Metal** y **Arquitectura de 3 Capas**. Cada laboratorio añade una pieza al rompecabezas de los sistemas embebidos de tiempo real.

## 🚀 Hoja de Ruta Técnica

| ID | Proyecto | Periféricos Clave | Concepto de Arquitectura |
|:--:|:---|:---|:---|
| 00 | **Fuses Config** | Clock, Watchdog | Configuración de Silicio |
| 01 | **Blink BareMetal** | GPIO (Registros) | Bit-shifting Manual |
| 02 | **Blink Bits** | GPIO | Capa 0 (Macros de Bits) |
| 03 | **GPIO Driver** | GPIO (Structs/Pointers) | Capa 1 (HAL) |
| 04 | **Polling & Debouncing** | GPIO Input | Gestión de Flancos y Ruido |
| 05 | **Systick Timer** | Timer 0 (CTC) | Multitarea Cooperativa |
| 06 | **LCD Driver** | LCD 16x2 (GPIO) | Capa 2 (Device Driver) |
| 07 | **EXTI Event-Driven** | INT0, INT1 | Programación Reactiva (ISR) |
| 08 | **TIM Normal Mode** | Timer 0, 1 y 2 | Orquestación Multitarea |
| 09 | **TIM Fast Mode** | Timer 2 | Generacion de PWM |

---

## 🧪 Exploración por Periféricos y Conceptos

### 🛠️ Configuración de Sistema y Registro
0. **[00_Fuses_Config](./00_Fuses_Config):** Control de fusibles, osciladores y prescalers de clock del sistema.
1. **[01_Blink_BareMetal](./01_Blink_BareMetal):** Acceso directo a registros de memoria mediante manipulación de bits.
2. **[02_Blink_Bits](./02_Blink_Bits):** Abstracción lógica de bits mediante el uso de la Capa 0.

### 🔌 Dominio del GPIO (General Purpose I/O)
3. **[03_Blink_GPIO_Driver](./03_Blink_GPIO_Driver):** Desarrollo de una HAL robusta utilizando estructuras y punteros volátiles.
4. **[04_GPIO_Polling_&_Debouncing](./04_GPIO_Polling):** Gestión de entradas digitales, detección de flancos y filtrado de rebotes mecánicos.

### ⏱️ Gestión de Tiempos y Timers
5. **[05_Systick_Timer_HAL_Genérica](./05_Systick_Timer):** Configuración del Timer 0 para generar una base de tiempo (System Tick) de 1ms.
8. **[08_TIM_Normal_Mode](./08_TIM_Normal_Mode):** Orquestación avanzada de los tres timers internos (T0, T1, T2) para control de potencia y sincronismo.
9. **[09_TIM_Fast_Pwm](./09_TIM_Fast_Pwm):** Implementación de modulación por ancho de pulso (Fast PWM) utilizando el Timer 2.
10. **[10_TIM_Fast_Pwm_II](./10_TIM_Fast_Pwm_II/):** Control de Led RGB mediante PWM e Intoduccion a las Tareas Cooperativas (bases de RTOS).

### 📟 Interfaz de Usuario y Dispositivos (Capa 2)
6. **[06_LCD_Generic_Driver](./06_LCD_Generic_Driver):** Implementación del protocolo HD44780 en modo 4-bits para visualización de datos.

### ⚡ Interrupciones y Sistemas Reactivos
7. **[07_EXTI_Event_Driven_HAL](./07_EXTI_Event_Driven_HAL):** Configuración de interrupciones externas (INTx) para liberar al CPU de la carga de polling.

---

## 🏗️ Modelo de Arquitectura de 3 Capas

Todos los proyectos en este repositorio siguen un modelo de diseño estratificado. Esta separación de responsabilidades garantiza que el firmware sea **portable, testeable y escalable**, permitiendo cambiar el hardware sin reescribir la lógica de la aplicación.


1. **Capa 1: Hardware Abstraction Layer (HAL):** - *Módulos:* `gpio.c`, `systick.c`, `systick.c`, `exti.c`.
   - *Responsabilidad:* Es la única capa que "toca" los registros del **ATmega328P**. Traduce las funcionalidades del silicio (registros DDR, PORT, TCCR) en funciones legibles. 
   - *Portabilidad:* Si migramos a un STM32, solo se reescribe esta capa.

2. **Capa 2: Device Drivers (Drivers de Dispositivos):**
   - *Módulos:* `lcd_driver.c`, `step_motor_28BYJ48.c`.
   - *Responsabilidad:* Implementa protocolos de periféricos externos. No sabe en qué pines está conectado el LCD; solo sabe pedirle a la **Capa 1** que ponga un pin en alto o bajo. 
   - *Abstracción:* Maneja la lógica de "cómo hablar con un LCD" o "cómo mover un motor".

3. **Capa 3: Aplicación (Lógica de Negocio):**
   - *Módulos:* `main.c`.
   - *Responsabilidad:* Orquestar la lógica del usuario (ej: "Si presiono el botón, mover el motor y mostrarlo en el LCD"). 
   - *Independencia:* Es hardware-agnóstica. Se comunica exclusivamente con la **Capa 2** y servicios de la **Capa 1** (como el Systick).

---

## 🛡️ Pilares de Robustez
Cada módulo desarrollado en este laboratorio se rige por tres principios:
* **Atomicidad:** Gestión rigurosa del registro de estado `SREG` para evitar condiciones de carrera.
* **Encapsulamiento:** Uso mandatorio de `static` para encapsular variables y `volatile` para sincronización hardware-software.
* **Concurrencia:** Prioridad en el uso de periféricos de hardware sobre esperas activas (`busy-waiting`).

---

*"La ingeniería no consiste en escribir código, sino en diseñar sistemas donde el hardware y el software bailan al mismo ritmo."*