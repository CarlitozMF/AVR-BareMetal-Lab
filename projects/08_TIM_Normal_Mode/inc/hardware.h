/**
 * @file hardware.h
 * @brief Mapeo de hardware específico para el Lab 08.
 * @author Mamani Flores Carlos
 * * @details Este archivo centraliza los pines del ATmega328P. 
 * Si se cambia la conexión física, solo se modifica aquí.
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>

/* --- ASIGNACIÓN DE PINES MOTOR STEPPER --- */
#define M1_IN1_PORT   &PORTB
#define M1_IN1_PIN    PB0
#define M1_IN2_PORT   &PORTB
#define M1_IN2_PIN    PB1
#define M1_IN3_PORT   &PORTB
#define M1_IN3_PIN    PB2
#define M1_IN4_PORT   &PORTB
#define M1_IN4_PIN    PB3

/* --- ASIGNACIÓN DE PINES INTERFAZ --- */
#define BTN_START_STOP  PD2   // Botón con interrupción o polling
#define BTN_DIR         PD3   // Botón de sentido
#define LED_BREATH      PB3   // Pin OC2A para PWM (Timer 2)

/* --- PARÁMETROS DE TIEMPO (TICKS) --- */
// F_CPU = 16MHz, Prescaler = 64 -> 1 tick = 4us
#define MOTOR_SPEED_DEFAULT  2500  // 10ms por paso
#define BUTTON_DEBOUNCE_TICKS 5000 // 20ms de muestreo

#endif