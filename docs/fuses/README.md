# ⚙️ Lab 00: Fuses & Clock Configuration

## 🎯 1. Título y Objetivos
- **Título:** Configuración de Bits de Fusibles (Fuses) para Cristal Externo.
- **Objetivos:** * 🛠️ Entender la importancia de los Fuses en la arquitectura AVR.
    * 💎 Configurar el ATmega328P para operar con un cristal externo de 16 MHz.
    * 🚀 Deshabilitar el divisor de reloj interno (`CKDIV8`).

---

## 📖 2. Teoría de Operación
Los **Fuses** son registros persistentes (no se borran al quitar la energía) que definen el comportamiento fundamental del hardware. A diferencia de un registro de I/O, no se pueden cambiar en tiempo de ejecución desde el código C; deben grabarse mediante un programador ISP (como el USBASP).

### Los 3 Bytes Críticos:
* **Low Fuse:** Define la fuente de reloj y los tiempos de arranque.
* **High Fuse:** Controla el reset, el arranque (bootloader) y la interfaz SPI.
* **Extended Fuse:** Configura el Brown-out Detector (BOD) para evitar errores por bajo voltaje.



---

## 🏗️ 3. Configuración Técnica
Para este laboratorio (y el resto del curso), configuramos el micro para **Full Swing Crystal**:

* **Low Fuse (`0xFF`):** Selecciona cristal externo de alta frecuencia y deshabilita `CKDIV8`.
* **High Fuse (`0xDA`):** Preserva el EEPROM y habilita el pin de Reset.
* **Extended Fuse (`0xFD`):** Configura el BOD a 2.7V para estabilidad.

---

## 🛡️ 4. Detalles de Robustez
- **⚠️ Riesgo de Brickeo:** Una mala configuración de fuses (como deshabilitar el pin de Reset o el SPI) puede dejar el micro inaccesible. Por eso, este paso se realiza de forma aislada y controlada.
- **✅ Verificación:** Antes de escribir, siempre realizamos una lectura preventiva con `avrdude` para confirmar la comunicación con el programador.

---

## 💻 5. Comando de Grabación
Utilizamos el Toolchain portable para realizar la carga mediante el programador USBASP:

```bash
avrdude -c usbasp -p m328p -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0xfd:m
```

---

## 🏁 6. Conclusión

Sin este paso, el cálculo de `F_CPU 16000000UL` en nuestros laboratorios sería falso. Al completar el **Lab 00**, hemos "desbloqueado" la verdadera potencia del ATmega328P, asegurando que cada instrucción y cada delay se ejecuten exactamente cuando deben.