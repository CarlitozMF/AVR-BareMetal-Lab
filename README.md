<div align="center">
  <img src="assets/portada.png" alt="AVR Bare Metal Lab Logo" width="400">
  
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![MCU](https://img.shields.io/badge/MCU-ATmega328P-blue?style=for-the-badge&logo=microchip)](https://www.microchip.com/en-us/product/ATmega328P)
[![Toolchain](https://img.shields.io/badge/Toolchain-AVR--GCC_|_AVRDUDE-orange?style=for-the-badge)](https://www.microchip.com/en-us/development-tools-tools-and-software/gcc-compilers-for-avr-and-arm)
[![Architecture](https://img.shields.io/badge/Architecture-Bare_Metal_|_HAL-green?style=for-the-badge)](https://en.wikipedia.org/wiki/Bare_machine)

  # AVR-BareMetal-Lab
  
  *"Un viaje autodidacta hacia las profundidades del ATmega328P: Redescubriendo el hardware mediante arquitectura de capas y herramientas open-source."*
</div>

---

## 🚀 Descripción
Exploración profunda del **ATmega328P** mediante firmware **Bare Metal** y arquitectura de capas. Este repositorio despliega una **HAL personalizada** desde cero, utilizando un toolchain profesional (**avr-gcc + Makefiles**). Ideal para ingenieros que buscan dominar el control total del hardware sin dependencias, desde bits hasta drivers modulares.

## 🛠️ Toolchain & Entorno
Para mantener la portabilidad, este proyecto utiliza un conjunto de herramientas externas. Puedes obtener el entorno completo de compilación en el siguiente repositorio auxiliar:

👉 **[AVR-Toolchain-Portable](https://github.com/TuUsuario/AVR-Toolchain-Portable)**

> **Nota:** Para que los Makefiles funcionen por defecto, el contenido del repositorio de herramientas debe clonarse o copiarse en una carpeta llamada `tools/` en la raíz de este directorio.

- **Compilador:** avr-gcc 15.2.0 (Portable)
- **Carga de Firmware:** avrdude
- **Automatización:** GNU Make
- **Hardware:** USBASP + ATmega328P

## 📂 Estructura de Capas
- **Capa 0 (Common):** Macros de manipulación de bits atómicas (`bits.h`).
- **Capa 1 (Hardware/Drivers):** Abstracción de registros de periféricos (`gpio.h/c`).
- **Capa 3 (Aplicación):** Lógica de usuario basada en servicios de drivers.

## 🧪 Laboratorios
1. **01_Blink_BareMetal:** Control de registros mediante bit-shifting manual.
2. **02_Blink_Bits:** Uso de la Capa 0 para manipulación de bits.
3. **03_Blink_GPIO_Driver:** Implementación de driver GPIO con estructuras y punteros.