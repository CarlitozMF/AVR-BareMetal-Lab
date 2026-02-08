# 🔘 Lab 04: GPIO Polling & Debouncing

## 🎯 1. Título y Objetivos
- **Título:** Lectura de Entradas Digitales y Control de Estado (Flags).
- **Objetivos:** * 📥 Implementar la lectura de periféricos de entrada mediante **Polling**.
    * 🛡️ Dominar el concepto de **Debouncing** (antirrebote) por software.
    * 🔄 Implementar una lógica de **Toggle por flanco** (un pulso = un cambio de estado).

---

## 📖 2. Teoría de Operación
En este laboratorio pasamos de la ejecución lineal a la ejecución condicional basada en el estado del hardware. 

### El Registro PINx
A diferencia de `PORTx` (que usamos para escribir), el registro `PINx` es el que nos permite leer el nivel de voltaje real en el pin. Como el pulsador está conectado directamente a **GND**, es obligatorio utilizar la **Resistencia Pull-up interna** del ATmega328P para asegurar un estado `HIGH` (1) lógico mientras el botón no sea presionado.



### Detección de Flanco (Edge Detection)
Para evitar que el LED cambie de estado erráticamente mientras mantenemos presionado el botón, implementamos una **bandera (flag)** de estado. Esto garantiza que la acción de "Toggle" solo ocurra una vez por cada evento de presión.

---

## 🏗️ 3. Arquitectura del Software (Evolución Técnica)

Para demostrar el dominio del hardware, este laboratorio documenta la transición del acceso directo a la abstracción por capas.

### Fase A: Enfoque Bare Metal (Registro Directo)
Es el método rápido, pero difícil de escalar y leer:
```c
if (!(PIND & (1 << PD2))) { // Si el pin es LOW
    PORTB ^= (1 << PB0);    // Toggle LED
    _delay_ms(50);          // Antirrebote rústico
}
```

### Fase B: Enfoque Profesional (Capa 1 - GPIO Driver)

Este es el código final implementado en el proyecto, utilizando nuestra **HAL personalizada** para mejorar la semántica y facilitar el mantenimiento del firmware:

```c
// Lógica de lectura mediante Polling y detección de flanco
if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW) {
    if (!boton_presionado) { // Lógica de detección de flanco (Edge Detection)
        GPIO_TogglePin(GPIO_B, 0);
        boton_presionado = 1; // Bloqueo: evita toggles infinitos mientras se mantenga presionado
    }
} else {
    boton_presionado = 0; // Liberación: permite una nueva detección al soltar el pulsador
}
```

---

## 🛡️ 4. Detalles de Robustez

- **🧬 Máquina de Estados de 1-Bit:** La variable `boton_presionado` actúa como una memoria de estado simple, transformando un pulsador momentáneo en un interruptor de enclavamiento funcional.
- **💎 Debouncing Temporal:** Se introdujo un retardo de **50ms** (`_delay_ms`) para filtrar el ruido mecánico (rebotes) inherente a los contactos metálicos del pulsador.
- **⚡ Lógica Negativa:** Se trabaja con **lógica invertida** debido a la configuración de la resistencia Pull-up. Un `0` lógico (GND) representa la presencia de una acción del usuario.



---

## 🗺️ 5. Mapeo de Hardware

| Periférico | Pin AVR | Función | Configuración | Driver Service |
| :--- | :--- | :--- | :--- | :--- |
| 🔴 **LED** | PB0 | Salida Digital | Push-Pull | `GPIO_TogglePin` |
| 🔘 **Pulsador** | PD2 | Entrada Digital | Pull-up Interna | `GPIO_ReadPin` |

---

## 🏁 6. Conclusión
Este laboratorio marca el inicio de los **sistemas interactivos**. Hemos aprendido que el hardware tiene "ruido" físico que debe ser saneado por software mediante técnicas de debouncing. La transición exitosa del código Bare Metal al uso del **Driver GPIO** confirma que la arquitectura de capas no solo es más estética, sino más segura, profesional y menos propensa a errores de manipulación de registros manual.

---

## 📚 7. Referencias
* **AVR Input Pins:** [Microchip Technology - Getting Started with GPIO](https://ww1.microchip.com/downloads/en/Appnotes/AN2515-AVR-Hardware-Design-Considerations-00002515C.pdf)
* **Debouncing Guide:** [Ganssle Group - A Guide to Debouncing](http://www.ganssle.com/debouncing.pdf)