/**
 * @file main.c
 * @brief Integración final: Systick, Botón con Pull-up y Estado en LCD.
 * @author CarlitozMF
 */

#include "gpio.h"
#include "timer.h"
#include "lcd_driver.h"

/* --- Configuración del Hardware --- */
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

/* --- Variables de Control --- */
static uint32_t t_previo_heartbeat = 0;
static uint32_t t_previo_boton = 0;
static uint32_t t_previo_segundos = 0;
static uint16_t segundos = 0;

/* --- Variables de Estado --- */
static uint8_t estado_led = 0; 

int main(void) {
    
/* --- Configuración de Hardware --- */
    
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

/* 3. Inicialización de Capa 2 (Device) */
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

/* --- Tarea 1: Heartbeat (PB5) --- */
        if (get_tick() - t_previo_heartbeat >= 200) {
            GPIO_TogglePin(GPIO_B, 5);
            t_previo_heartbeat = get_tick();
        }

        /* --- Tarea 2: Contador de Segundos --- */
        if (get_tick() - t_previo_segundos >= 1000) {
            segundos++;
            LCD_SetCursor(0, 8);
            
            /* Conversión simple de número a texto (hasta 999s) */
            if (segundos > 99) LCD_WriteChar((segundos / 100) % 10 + '0');
            if (segundos > 9)  LCD_WriteChar((segundos / 10) % 10 + '0');
            LCD_WriteChar((segundos % 10) + '0');
            LCD_Print("s  "); // Espacios para limpiar sobrantes
            
            t_previo_segundos = get_tick();
        }

        /* --- Tarea 3: Botón y Rayo --- */
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW) {
            if (get_tick() - t_previo_boton >= 200) {
                estado_led = !estado_led;
                
                LCD_SetCursor(1, 8);
                if (estado_led) {
                    GPIO_WritePin(GPIO_B, 3, GPIO_HIGH);
                    LCD_Print(": ON  ");
                    LCD_WriteChar(0); // Imprimimos el rayo (carácter 0)
                } else {
                    GPIO_WritePin(GPIO_B, 3, GPIO_LOW);
                    LCD_Print(": OFF   "); // Espacios para borrar el rayo
                }
                
                t_previo_boton = get_tick();
            }
        }
    }
    return 0;
}