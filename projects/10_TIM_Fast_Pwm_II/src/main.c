/**
 * @file main.c
 * @brief Implementación de control de LEDs RGB mediante PWM, Systick .
 * @author Mamani Flores Carlos
 * @date 2026
 */

 /**************************************************************************************
* PROYECTO: 10_TIM_Fast_Pwm_II
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Aplicacion de Fast PWM mediante driver para LED RGB, systick y exti
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/
#include <avr/interrupt.h>
#include "systick.h"
#include "app_project_10.h"

int main(void) {
    /* Inicialización de base de tiempo global (Timer 1) */
    Systick_Init(TIMER_1); 

    /* Inicialización de la Aplicación */
    Sys_Init();

    sei(); // Habilitar interrupciones globales

    while (1) {
        /* Ejecución de Tareas Cooperativas */
        Task_Rainbow(30, 5);    //Tarea del LED RGB
        Task_Toggle(100);       //Tarea del Toggle con Systick
        task_button_led();      //Tarea del dimer PD3 con boton en PD2
    }
}