#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "hardware.h"
#include "timer.h"
#include "lcd_driver.h"

/* Carácter personalizado: Rayo ⚡ */
extern uint8_t char_rayo[8];

void System_Init(void);
void Task_Heartbeat(void);
void Task_Contador(void);
void Task_Boton(void);

#endif