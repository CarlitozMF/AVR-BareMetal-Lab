/**************************************************************************************
* PROYECTO: 05_Systick_Timer
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Implementación de multitarea cooperativa mediante un Tick de sistema (1ms).
* Ejecuta un Heartbeat constante y un escaneo de pulsador con Debounce no bloqueante,
* permitiendo que ambas tareas coexistan sin interferencias.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "gpio.h"
#include "systick.h"
#include <avr/interrupt.h>

int main(void) {

    /* --- Configuración de Hardware (Capa 1) --- */
    
    // LED 1: Heartbeat (Indicador de vida del sistema)
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT);
    
    // LED 2: Respuesta (Feedback de interacción de usuario)
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT);
    
    // Pulsador: Entrada con Pull-up interna activa
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);

    /* --- Inicialización del Sistema --- */
    
    /** * Inicializamos el Systick usando la HAL genérica. 
     * El sistema ahora genera una base de tiempo de 1ms.
     */
    Systick_Init(TIMER_0);
    sei(); // Habilita interrupciones globales para permitir el Tick del Timer
    
    /* --- Variables de Control de Tiempo (Timestamps) --- */
    uint32_t t_previo_heartbeat = 0;
    uint32_t t_ultimo_debounce = 0;
    uint8_t  boton_presionado = 0;

    /* --- Bucle de Eventos (Super Loop No Bloqueante) --- */
    while (1) {

        /* TAREA 1: Heartbeat (Frecuencia: 10Hz) */
        // Comparamos el tick actual con el último registro para determinar si pasaron 100ms
        if (get_tick() - t_previo_heartbeat >= 100) {
            GPIO_TogglePin(GPIO_B, 0);
            t_previo_heartbeat = get_tick();
        }

        /* TAREA 2: Escaneo de Pulsador con Debounce No Bloqueante */
        
        // 1. Detección de Pulsación (Flanco de bajada)
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW && !boton_presionado) {
            // Filtro de ruido temporal (Debounce)
            if (get_tick() - t_ultimo_debounce >= 50) {
                GPIO_TogglePin(GPIO_B, 3); 
                boton_presionado = 1;           // Bloqueamos acción para el estado presionado
                t_ultimo_debounce = get_tick(); // Actualizamos marca de tiempo
            }
        }

        // 2. Detección de Liberación (Flanco de subida)
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_HIGH && boton_presionado) {
            // Aseguramos que el estado estable de reposo también pase el filtro
            if (get_tick() - t_ultimo_debounce >= 50) {
                boton_presionado = 0;
                t_ultimo_debounce = get_tick();
            }
        }

    } /* Fin del Super Loop */

    return 0;
}