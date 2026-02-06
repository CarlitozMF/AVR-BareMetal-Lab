/**
 * @file main.c
 * @author CarlitozMF
 * @brief Laboratorio 07: Sistema de Entradas Reactivo mediante EXTI.
 * @details Implementación de interrupciones externas con debounce no bloqueante
 * para el control de interfaz en un entorno multitarea.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "exti.h"
#include "lcd_driver.h"

/* --- Configuración del Hardware LCD --- */
const LCD_Config_t lcd_main = {
    .port_rs = GPIO_B, .pin_rs = 0,
    .port_en = GPIO_B, .pin_en = 1,
    .port_d4 = GPIO_D, .pin_d4 = 4,
    .port_d5 = GPIO_D, .pin_d5 = 5,
    .port_d6 = GPIO_D, .pin_d6 = 6,
    .port_d7 = GPIO_D, .pin_d7 = 7,
    .type    = LCD_16X2
};

/* Mapa de bits para el carácter del Rayo (⚡) */
uint8_t char_rayo[] = {
    0b00010,
    0b00100,
    0b01000,
    0b11111,
    0b00100,
    0b01000,
    0b10000,
    0b00000
};


/* --- Variables de Control de Tiempo (Multitarea) --- */
volatile uint32_t t_previo_heartbeat = 0;
volatile uint32_t t_previo_uptime = 0;
static uint16_t segundos = 0;

/* --- Variables de Estado --- */
static uint8_t estado_led = 0; 

/* --- Variables para Debounce Atómico (ISR) --- */
volatile uint32_t t_ultimo_evento = 0;  // Almacena el tick del último disparo válido
#define DEBOUNCE_TIME_MS 200            // Tiempo de guardia para el filtro de ruido

/* --- Flags de Eventos --- */
volatile uint8_t flag_actualizar_rayo = 0; // Se levanta en la ISR, se baja en la App


/* --- Tarea de Aplicación: Actualiza el cronómetro en el LCD cada 1s. ---*/
void tarea_uptime(void) {
    if (get_tick() - t_previo_uptime >= 1000) {

        segundos++;
        LCD_SetCursor(0, 8);
    /* Conversión simple de número a texto (hasta 999s) */
        if (segundos > 99) LCD_WriteChar((segundos / 100) % 10 + '0');
        if (segundos > 9)  LCD_WriteChar((segundos / 10) % 10 + '0');
        LCD_WriteChar((segundos % 10) + '0');
        LCD_Print("s  "); // Espacios para limpiar sobrantes

        t_previo_uptime = get_tick();
    }
}

/** --- Tarea de Aplicación: Heartbeat del sistema. --- */
void tarea_heartbeat(void) {
    if (get_tick() - t_previo_heartbeat >= 250) {
        GPIO_TogglePin(GPIO_B, 5); // LED de estado
        t_previo_heartbeat = get_tick();
    }
}

int main(void) {

/* --- Configuración de Pines --- */

    // LED 1: Heartbeat (Parpadeo constante por tiempo)
    GPIO_InitPin(GPIO_B, 5, GPIO_OUTPUT);

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

    /* --- Configuración de EXTI (Capa 1) --- */
    // Configuramos INT0 para disparar en flanco de bajada (Pulsador a GND)
    EXTI_Init(EXTI_INT0, EXTI_FALLING_EDGE);

/* --- Inicialización de Capa 2 (Device) --- */
    LCD_Init(&lcd_main);
    LCD_CreateCustomChar(0, char_rayo);

    /* --- PANTALLA DE BIENVENIDA --- */
    LCD_SetCursor(0, 3);
    LCD_Print("BIENVENIDOS");
    LCD_SetCursor(1, 1);
    LCD_Print("AVR Bare-Metal");
    delay_ms_tick(2000); // Pausa de 2 segundos para que se vea
    LCD_Clear();

    /* --- PANTALLA DE OPERACIÓN --- */
    LCD_SetCursor(0, 0);
    LCD_Print("Uptime: 0s");
    LCD_SetCursor(1, 0);
    LCD_Print("Estado: OFF");


    while (1) {
        tarea_heartbeat();
        tarea_uptime();

        /* Tarea de Interfaz: Solo actúa si la ISR levantó la bandera */
        if (flag_actualizar_rayo) {
            GPIO_TogglePin(GPIO_B, 3); // Cambiar estado LED de respuesta

            estado_led = !estado_led;
                
                LCD_SetCursor(1, 8);
                if (estado_led) {
                    LCD_Print(": ON  ");
                    LCD_WriteChar(0); // Imprimimos el rayo (carácter 0)
                } else {
                    LCD_Print(": OFF   "); // Espacios para borrar el rayo
                }
            
            flag_actualizar_rayo = 0;   // Bajar bandera (Evento atendido)
        }
    }

    return 0;
}

/**
 * @brief Vector de Interrupción para INT0 (Pin PD2).
 * @details Esta función se ejecuta por hardware cuando el pin cae a 0V.
 * Implementa un filtro de tiempo para ignorar los rebotes.
 */
ISR(INT0_vect) {
    uint32_t t_actual = get_tick();

    /**
     * Filtro de Debounce Atómico:
     * Solo procesamos la interrupción si han pasado más de 200ms desde
     * el último disparo aceptado.
     */
    if (t_actual - t_ultimo_evento >= DEBOUNCE_TIME_MS) {
        flag_actualizar_rayo = 1;      // Notificar a la aplicación
        t_ultimo_evento = t_actual;    // Actualizar tiempo de referencia
    }
}