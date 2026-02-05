# ⏲️ Lab 05: Timer & System Tick (Systick) - HAL Genérica

## 🎯 1. Título y Objetivos
- **Título:** Implementación de HAL Genérica para Timers y Base de Tiempo No Bloqueante.
- **Objetivos:** * 🏗️ Implementar una **Capa 1 (HAL)** paramétrica que soporte Timer0, Timer1 y Timer2.
    * ⏱️ Crear un **Systick** unificado de 1ms independiente del hardware seleccionado.
    * ⚡ Lograr **Multitarea Cooperativa** mediante el uso de marcas de tiempo (Timestamps).

---

## 📖 2. Teoría de Operación

### Recursos de Temporización del ATmega328P
Este microcontrolador cuenta con tres temporizadores independientes. Nuestra HAL los unifica para que puedan cumplir la misma función de Systick:

| Timer | Resolución | Características Clave | Uso Común |
| :--- | :--- | :--- | :--- |
| **Timer 0** | 8 bits | 2 canales de comparación. | **Systick (Base de Tiempo)**. |
| **Timer 1** | 16 bits | Alta precisión, 16 bits reales. | Servomotores, medidas de frecuencia. |
| **Timer 2** | 8 bits | Capacidad asíncrona. | RTC, Low Power Modes. |



### Configuración del Modo CTC (Clear Timer on Compare Match)
Utilizamos el modo **CTC** para establecer la base de tiempo. A diferencia del modo normal, el hardware resetea el contador a cero automáticamente al alcanzar el valor del registro de comparación (`OCRnx`), eliminando el error acumulado por latencia de software.

**Cálculo de la precisión de 1ms (@16MHz):**
Para todos los timers, buscamos una frecuencia de interrupción de $1,000 \text{ Hz}$:
1. **Prescaler (64):** $16,000,000 / 64 = 250,000 \text{ Hz}$.
2. **Pasos:** $250,000 \text{ Hz} / 1,000 \text{ Hz} = 250 \text{ pasos}$.
3. **Valor de Registro:** $OCRnx = 250 - 1 = \mathbf{249}$.

---

## 🏗️ 3. Arquitectura del Software (HAL Genérica)

Hemos evolucionado hacia una **HAL Paramétrica**. El usuario decide qué instancia usar mediante la enumeración `timer_instance_t`, mientras que la lógica de aplicación permanece agnóstica al hardware.

**Estructura de la HAL:**
- **`TIMER_Init(instance)`:** Configura los registros específicos del timer elegido (manejando las sutiles diferencias de bits de prescaler entre el Timer 0/1 y el Timer 2).
- **`get_tick()`:** Retorna el conteo global de milisegundos de forma atómica.
- **`delay_ms_tick()`:** Retardo preciso basado en hardware (bloqueante pero preciso).



---

## 🛠️ 3.1. Detalle de Registros y Escalabilidad

El driver gestiona las variaciones de hardware de forma interna. A continuación se detallan los registros clave configurados en la HAL:

### ⚙️ Configuración por Instancia
* **Timer 0 / 2 (8-bit):** Se configuran `TCCRnxA/B` para modo CTC y prescaler de 64. 
* **Timer 1 (16-bit):** Se configura `TCCR1B` activando el bit `WGM12`. Al ser un registro de 16 bits, `OCR1A` permite una precisión mucho mayor si fuera necesaria.

### 🔄 Flexibilidad de Migración
Si el **Timer 0** es requerido para PWM, la migración es tan simple como cambiar una línea en el `main.c`:
`TIMER_Init(TIMER_1);` o `TIMER_Init(TIMER_2);`. 
La HAL se encarga de redireccionar las ISR correspondientes (`TIMER0_COMPA_vect`, `TIMER1_COMPA_vect`, `TIMER2_COMPA_vect`) al mismo contador de sistema.

---

## 🛡️ 4. Detalles de Robustez

- **🧬 Secciones Críticas (SREG):** Al leer una variable de 32 bits (`uint32_t`) en un micro de 8 bits, existe el riesgo de que una interrupción ocurra entre la lectura del primer y último byte. El driver respalda el registro `SREG`, deshabilita interrupciones (`cli`) y restaura el estado original para garantizar una lectura atómica.
- **💎 Palabra Clave `volatile`:** La variable `ms_ticks` se marca como `static volatile`, forzando al compilador a leer siempre el valor real de la RAM, evitando optimizaciones que ignorarían los cambios realizados por las ISR.
- **⚡ Multitarea No Bloqueante:** Se implementa lógica de comparación de tiempos en el `main.c`. Esto permite que el LED de Heartbeat y la detección de flanco con debounce del pulsador operen en "paralelo" sin detener el flujo del CPU.

---

## 🗺️ 5. Mapeo de Hardware

| Periférico | Pin AVR | Función | Configuración | Driver Service |
| :--- | :--- | :--- | :--- | :--- |
| 🔴 **LED 1** | PB0 | Heartbeat (500ms) | Salida Digital | `GPIO_TogglePin` |
| 🔵 **LED 2** | PB3 | Respuesta Botón | Salida Digital | `GPIO_TogglePin` |
| 🔘 **Botón** | PD2 | Entrada Interactiva | Pull-up Interna | `GPIO_ReadPin` |

---

## 🏁 6. Conclusión
Este laboratorio consolida una arquitectura profesional. Al unificar los tres timers en una única interfaz genérica, el sistema gana portabilidad y robustez. Hemos pasado de un firmware secuencial a un sistema multitarea cooperativo basado en Ticks de hardware, sentando las bases críticas para el manejo de periféricos externos como el **LCD 16x2**.

---

## 📚 7. Referencias
* **ATmega328P Datasheet:** Secciones 15, 16 y 18 (Timers 0, 1 y 2).
* **Doxygen Documentation Standard:** [Manual de Estilo](https://www.doxygen.nl/manual/docblocks.html).