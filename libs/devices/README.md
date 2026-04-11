# 📂 Capa 2: Device Drivers (`libs/devices`)

Este directorio contiene los controladores para componentes electrónicos externos al microcontrolador. Siguiendo la **Arquitectura de 3 Capas**, estos drivers son totalmente independientes de los pines físicos, delegando el control del hardware a la **Capa 1 (HAL)**.

## 📦 Dispositivos Soportados

| Dispositivo | Descripción Técnica | Enlace al Header |
| :--- | :--- | :--- |
| **LCD Hitachi HD44780** | Driver para pantallas de 16x2 y 20x4 en modo 4-bits. | [📄 lcd_driver.h](./Inc/lcd_driver.h) |
| **Motor PaP 28BYJ-48** | Control de motor paso a paso unipolar con driver ULN2003. | [📄 step_motor_28BYJ48.h](./Inc/step_motor_28BYJ48.h) |

---

## 📟 Driver LCD (HMI)

El driver de LCD implementa una abstracción que permite mapear los pines de datos y control en **cualquier combinación de puertos** del ATmega328P sin pérdida de performance.

### 🚀 Ejemplo de Inicialización
```c
LCD_Config_t lcd_cfg = {
    .port_rs = &PORTB, .pin_rs = 0,
    .port_en = &PORTB, .pin_en = 1,
    .port_d4 = &PORTD, .pin_d4 = 4,
    // ... rest of data pins
    .type = LCD_16X2
};

LCD_Init(&lcd_cfg);
LCD_Print("UTN - FRT");
```
> [!NOTE]
> Soporta la creación de hasta 8 caracteres personalizados en la CGRAM mediante [`LCD_CreateCustomChar`](./Inc/lcd_driver.h).

---

## ⚙️ Driver Motor Paso a Paso (Actuadores)

Diseñado con un enfoque **orientado a objetos** (encapsulamiento por estructuras), permite controlar múltiples motores de forma independiente y asíncrona dentro del mismo firmware.

### 🛠️ Características Destacadas
* **Modos de Paso:** Soporte para `MODE_FULL_STEP` (Torque máximo) y `MODE_HALF_STEP` (Máxima suavidad y resolución).
* **Control No Bloqueante:** La función `Stepper_Step_Sequential` no utiliza delays; está diseñada para ser llamada desde un **Scheduler** o una base de tiempo basada en **Timers**.
* **Protección Térmica:** Incluye la función `Stepper_Stop` para liberar las bobinas, una medida de seguridad crítica para evitar el sobrecalentamiento del integrado **ULN2003**.

### ⚙️ Configuración Atómica (Ejemplo de uso)
```c
/* Definición de la instancia del motor */
Stepper_t motor1;

/* Mapeo dinámico de hardware */
volatile uint8_t* puertos[] = {&PORTB, &PORTB, &PORTB, &PORTB};
uint8_t pines[] = {0, 1, 2, 3};

/* Inicialización del objeto */
Stepper_Init(&motor1, puertos, pines, MODE_FULL_STEP);
```

---

## 🏗️ Arquitectura de la Carpeta

```plaintext
libs/devices/
├── Inc/    # Archivos de cabecera (.h) - Definición de APIs y Estructuras
└── Src/    # Archivos de implementación (.c) - Lógica de los drivers
```

> [!IMPORTANT]
> Todos los drivers de esta capa asumen que la **Capa 1 (HAL)** ya ha inicializado los pines correspondientes como salida mediante `GPIO_InitPin`. Esta precondición garantiza una separación absoluta de responsabilidades: la **Capa 2** gestiona la lógica del dispositivo (protocolos, secuencias), mientras que la **Capa 1** gestiona el estado del silicio (registros DDR, PORT).