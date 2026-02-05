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

### ⚙️ Configuración del Modo CTC (Clear Timer on Compare Match)

Utilizamos el modo **CTC** para establecer la base de tiempo. A diferencia del modo normal (*Overflow*), donde el contador debe llegar a su valor máximo ($255$ o $65,535$) para reiniciar, el hardware resetea el contador a cero automáticamente en el instante exacto en que alcanza el valor del registro de comparación (**`OCRnx`**).

Esto elimina el **error acumulado (*drift*)** que ocurriría si tuviéramos que resetear el contador manualmente por software dentro de una interrupción, garantizando un determinismo absoluto en el tiempo.

### 🧮 Matemática del Timer (Modo CTC) y Selección de Prescaler

Para obtener una base de tiempo precisa, nos basamos en la fórmula oficial del datasheet del ATmega328P para el modo **CTC**:

$$f_{OCnx} = \frac{f_{clk\_I/O}}{2 \cdot N \cdot (1 + OCRnx)}$$

Donde:
* $f_{clk\_I/O}$: Frecuencia del cristal (16 MHz).
* $N$: Factor del Prescaler.
* $OCRnx$: Valor del registro de comparación.

#### 1. ¿Por qué seleccionar un Prescaler ($N$) de 64?
El prescaler es necesario para que el contador de 8 bits (Timer 0) no se desborde antes de alcanzar el milisegundo deseado.
* Si $N=1$: El timer contaría a 16 MHz. Un registro de 8 bits (máx 255) se desbordaría cada $15.9 \mu s$. Imposible llegar a 1 ms.
* Si $N=64$: La frecuencia del timer baja a $250,000 \text{ Hz}$. 
    * Tiempo por cada tick del timer: $1 / 250,000 = 4 \mu s$.
    * Pasos para 1 ms: $1ms / 4 \mu s = 250$ pasos.
    * Como 250 es menor a 255, **cabe perfectamente en un registro de 8 bits**, optimizando la resolución sin saturar el periférico.

#### 2. Cálculo del Valor Final de Comparación (`OCRnx`)
Reorganizando la fórmula para obtener los "pasos" necesarios ($1 + OCRnx$) para una frecuencia de interrupción de $1,000 \text{ Hz}$ ($1 \text{ ms}$):

$$\text{Pasos} = \frac{f_{clk\_I/O}}{N \cdot f_{target}} = \frac{16,000,000 \text{ Hz}}{64 \cdot 1,000 \text{ Hz}} = 250$$

Dado que el hardware comienza a contar desde $0$, el valor que debemos cargar en el registro es $\text{Pasos} - 1$:

$$OCRnx = 250 - 1 = \mathbf{249}$$


#### 🕹️ Mecanismo de Registros:
* **`WGM` (Waveform Generation Mode):** Configura el modo de operación.
    * **Timer 0/2:** Bit `WGM01` (o `WGM21`) en `1` activa el modo CTC.
    * **Timer 1:** Bit `WGM12` en `1` activa el modo CTC.
* **`CSnx` (Clock Select):** Define el prescaler $N$. Para $N=64$:
    * **Timer 0/1:** `CSn1=1` y `CSn0=1`.
    * **Timer 2:** `CS22=1` (Nota: Timer 2 tiene un mapeo de bits distinto para el mismo valor de prescaler).
* **`OCRnx`:** Registro de comparación. El hardware resetea `TCNTn` a $0$ de forma **atómica** inmediatamente después de alcanzar este valor.

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
* **ATmega328P Datasheet:** Secciones 14, 15, 16 y 17 (Timers 0, 1 y 2).
* **Doxygen Documentation Standard:** [Manual de Estilo](https://www.doxygen.nl/manual/docblocks.html).