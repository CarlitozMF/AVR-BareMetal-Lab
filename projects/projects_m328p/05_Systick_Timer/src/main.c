#include "gpio.h"
#include "timer.h"

int main(void) {
    /* --- Configuración de Hardware --- */
    
    // LED 1: Heartbeat (Parpadeo constante por tiempo)
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT);
    
    // LED 2: Respuesta (Controlado por pulsador)
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT);
    
    // Pulsador: Entrada con Pull-up activa
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);

/* --- Inicialización del Sistema --- */
    
    /** * Inicializamos el Systick usando la HAL genérica. 
     * Puedes cambiar TIMER_0 por TIMER_1 o TIMER_2 sin afectar el resto del código.
     */
    TIMER_Init(TIMER_0);
    sei();         // Habilita interrupciones globales (Vital para el Timer)
    
 /* --- Variables de Control de Tiempo (Flags y Timestamps) --- */
    uint32_t t_previo_heartbeat = 0;
    uint32_t t_ultimo_debounce = 0;
    uint8_t  boton_presionado = 0;

    while (1) {
        /* TAREA 1: Heartbeat (Toggle automático) */
        // No bloquea el flujo; solo entra cuando se cumple el tiempo.
        if (get_tick() - t_previo_heartbeat >= 100) {
            GPIO_TogglePin(GPIO_B, 0);
            t_previo_heartbeat = get_tick();
        }

        /* TAREA 2: Toggle por Pulsador (No Bloqueante) */
        // 1. Detectamos el flanco de bajada (Pulsación)
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW && !boton_presionado) {
            // Verificamos si ha pasado suficiente tiempo desde el último ruido (Debounce)
            if (get_tick() - t_ultimo_debounce >= 50) {
                GPIO_TogglePin(GPIO_B, 3); 
                boton_presionado = 1;     // Bloqueamos la acción
                t_ultimo_debounce = get_tick(); // Registramos el tiempo del evento
            }
        }

        // 2. Liberamos la bandera solo cuando el botón se suelta (Flanco de subida)
        // También aplicamos el debounce aquí para evitar ruidos al soltar.
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_HIGH && boton_presionado) {
            if (get_tick() - t_ultimo_debounce >= 50) {
                boton_presionado = 0;
                t_ultimo_debounce = get_tick();
            }
        }
    }

    return 0;
}