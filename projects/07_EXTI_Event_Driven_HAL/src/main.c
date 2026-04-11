/**************************************************************************************
* PROYECTO: 07_EXTI_Event_Driven_HAL
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Sistema HMI orientado a eventos. Utiliza Interrupciones Externas (EXTI)
* para detectar pulsaciones en tiempo real con debounce atómico. La comunicación entre
* la ISR y el bucle principal se realiza mediante flags volátiles.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "main_project_07.h"

/* --- Configuración del Carácter Personalizado --- */
uint8_t char_rayo[] = {
    0b00010, 0b00100, 0b01000, 0b11111,
    0b00100, 0b01000, 0b10000, 0b00000
};

/* --- Instancia de Configuración del Driver LCD --- */
LCD_Config_t lcd_main_cfg;

/* --- Variables de Control de Tiempo (Multitarea) --- */
static uint32_t t_prev_heartbeat = 0;
static uint32_t t_prev_segundos  = 0;
static uint16_t segundos         = 0;
static uint8_t  estado_led       = 0;

/* --- Variables para Debounce Atómico (ISR) --- */
volatile uint32_t t_ultimo_evento = 0;  // Registro del tick del último disparo válido
#define DEBOUNCE_TIME_MS 200            // Tiempo de guardia (filtro de ruido)

/* --- Flags de Comunicación entre ISR y Aplicación --- */
// volatile es obligatorio para variables modificadas dentro de una ISR
volatile uint8_t flag_actualizar_rayo = 0; 

/* --- Bucle Principal (Arquitectura de Eventos) --- */
int main(void) {

    System_Init(); // Configuración de HAL, Drivers y EXTI

    while (1) {
        Task_Heartbeat(); // Tarea constante (Latido)
        Task_Contador();  // Tarea constante (Reloj Uptime)

        /* Tarea de Interfaz: Solo se ejecuta por demanda de evento (Event-Driven) */
        if (flag_actualizar_rayo) {
            GPIO_TogglePin(GPIO_B, 3); // Feedback físico inmediato

            estado_led = !estado_led;
            LCD_SetCursor(1, 8);
            
            if (estado_led) {
                LCD_Print(": ON  ");
                LCD_WriteChar(0); // Símbolo personalizado
            } else {
                LCD_Print(": OFF   "); // Limpieza de caracteres residuales
            }
            
            flag_actualizar_rayo = 0;   // Reset de bandera (Evento consumido)
        }
    }

    return 0;
}

/* --- Inicialización del Sistema --- */
void System_Init(void) {
    
    /* 1. Mapeo Físico del LCD (Drivers Capa 2) */
    lcd_main_cfg.port_rs = LCD_CTRL_PORT.PORT; lcd_main_cfg.pin_rs = LCD_RS_PIN;
    lcd_main_cfg.port_en = LCD_CTRL_PORT.PORT; lcd_main_cfg.pin_en = LCD_EN_PIN;
    lcd_main_cfg.port_d4 = LCD_DATA_PORT.PORT; lcd_main_cfg.pin_d4 = LCD_D4_PIN;
    lcd_main_cfg.port_d5 = LCD_DATA_PORT.PORT; lcd_main_cfg.pin_d5 = LCD_D5_PIN;
    lcd_main_cfg.port_d6 = LCD_DATA_PORT.PORT; lcd_main_cfg.pin_d6 = LCD_D6_PIN;
    lcd_main_cfg.port_d7 = LCD_DATA_PORT.PORT; lcd_main_cfg.pin_d7 = LCD_D7_PIN;
    lcd_main_cfg.type    = LCD_16X2;

    LCD_Init(&lcd_main_cfg);
    LCD_CreateCustomChar(0, char_rayo);

    /* 2. Configuración de Capa 1 (HAL) */
    GPIO_InitPin(GPIO_B, 5, GPIO_OUTPUT); // LED Heartbeat
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT); // LED Response
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);  // Pulsador (INT0)
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);  // Pull-up activa

    Systick_Init(TIMER_0); // Base de tiempo para debounce y tareas

    /* 3. Configuración de Interrupción Externa */
    // INT0 configurada para disparar en flanco de bajada
    EXTI_Init(EXTI_INT0, EXTI_FALLING_EDGE);

    sei(); // Activación global de interrupciones (Habilita Systick y EXTI)

    /* 4. Pantalla de Bienvenida */
    LCD_SetCursor(0, 3); LCD_Print("BIENVENIDOS");
    LCD_SetCursor(1, 1); LCD_Print("EVENT-DRIVEN");
    delay_ms_tick(2000);
    LCD_Clear();
    
    LCD_SetCursor(0, 0); LCD_Print("Uptime: 0s");
    LCD_SetCursor(1, 0); LCD_Print("Estado: OFF");
}

/* --- Implementación de Tareas Cooperativas --- */

void Task_Heartbeat(void) {
    if (get_tick() - t_prev_heartbeat >= 200) {
        GPIO_TogglePin(GPIO_B, 5);
        t_prev_heartbeat = get_tick();
    }
}

void Task_Contador(void) {
    if (get_tick() - t_prev_segundos >= 1000) {
        segundos++;
        t_prev_segundos = get_tick();
        
        uint8_t hh = segundos / 3600;
        uint8_t mm = (segundos % 3600) / 60;
        uint8_t ss = segundos % 60;

        LCD_SetCursor(0, 8);
        LCD_WriteChar((hh/10)+'0'); LCD_WriteChar((hh%10)+'0'); LCD_WriteChar(':');
        LCD_WriteChar((mm/10)+'0'); LCD_WriteChar((mm%10)+'0'); LCD_WriteChar(':');
        LCD_WriteChar((ss/10)+'0'); LCD_WriteChar((ss%10)+'0');
    }
}

/* --- Rutinas de Servicio de Interrupción (ISR) --- */

/**
 * @brief Vector de Interrupción para INT0 (Pin PD2).
 * @details Se ejecuta de forma asíncrona ante un flanco de bajada.
 * Implementa un filtro de debounce atómico comparando con el Systick.
 */
ISR(INT0_vect) {
    uint32_t t_actual = get_tick();

    // Filtro de Debounce: Ignora disparos si han pasado menos de 200ms
    if (t_actual - t_ultimo_evento >= DEBOUNCE_TIME_MS) {
        flag_actualizar_rayo = 1;      // Notificación para el bucle principal
        t_ultimo_evento = t_actual;    // Actualización de marca de tiempo
    }
}