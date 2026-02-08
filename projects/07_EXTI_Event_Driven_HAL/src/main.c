/**
 * @file main.c
 * @author CarlitozMF
 * @brief Laboratorio 07: Sistema de Entradas Reactivo mediante EXTI.
 * @details Implementación de interrupciones externas con debounce no bloqueante
 * para el control de interfaz en un entorno multitarea.
 */

#include "main.h"

// Variable de configuración global para el driver
LCD_Config_t lcd_main_cfg;

/* --- Datos del Carácter --- */
uint8_t char_rayo[] = {
    0b00010, 0b00100, 0b01000, 0b11111,
    0b00100, 0b01000, 0b10000, 0b00000
};

/* --- Variables de Control de Tiempo (Multitarea) --- */
static uint32_t t_prev_heartbeat = 0;
static uint32_t t_prev_segundos = 0;
static uint16_t segundos = 0;
static uint8_t estado_led = 0;


/* --- Variables para Debounce Atómico (ISR) --- */
volatile uint32_t t_ultimo_evento = 0;  // Almacena el tick del último disparo válido
#define DEBOUNCE_TIME_MS 200            // Tiempo de guardia para el filtro de ruido

/* --- Flags de Eventos --- */
volatile uint8_t flag_actualizar_rayo = 0; // Se levanta en la ISR, se baja en la App


int main(void) {

    System_Init();

    while (1) {
        Task_Heartbeat();
        Task_Contador();

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




void System_Init(void) {

/* 1. Configurar GPIOs mediante Capa 1 */
    GPIO_InitPin(LCD_CTRL_PORT, LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_CTRL_PORT, LCD_EN_PIN, GPIO_OUTPUT);
    
    GPIO_InitPin(LCD_DATA_PORT, LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_DATA_PORT, LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_DATA_PORT, LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_DATA_PORT, LCD_D7_PIN, GPIO_OUTPUT);

    /* 2. Configurar estructura del Driver LCD usando hardware.h */
    lcd_main_cfg.port_rs = LCD_CTRL_PORT.PORT;
    lcd_main_cfg.pin_rs  = LCD_RS_PIN;
    lcd_main_cfg.port_en = LCD_CTRL_PORT.PORT;
    lcd_main_cfg.pin_en  = LCD_EN_PIN;
    
    lcd_main_cfg.port_d4 = LCD_DATA_PORT.PORT;
    lcd_main_cfg.pin_d4  = LCD_D4_PIN;
    lcd_main_cfg.port_d5 = LCD_DATA_PORT.PORT;
    lcd_main_cfg.pin_d5  = LCD_D5_PIN;
    lcd_main_cfg.port_d6 = LCD_DATA_PORT.PORT;
    lcd_main_cfg.pin_d6  = LCD_D6_PIN;
    lcd_main_cfg.port_d7 = LCD_DATA_PORT.PORT;
    lcd_main_cfg.pin_d7  = LCD_D7_PIN;
    
    lcd_main_cfg.type    = LCD_16X2;

    /* 3. Inicializar Display */
    LCD_Init(&lcd_main_cfg);
    LCD_CreateCustomChar(0, char_rayo);


    // 1. GPIO Init (Usando tu HAL)
    GPIO_InitPin(GPIO_B, 5, GPIO_OUTPUT); // Heartbeat
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT); // Response LED
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);  // Botón
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);  // Pull-up

    // 2. Timer & Interrupciones
    TIMER_Init(TIMER_0);
    sei(); 

    // Configuramos INT0 para disparar en flanco de bajada (Pulsador a GND)
    EXTI_Init(EXTI_INT0, EXTI_FALLING_EDGE);

    // Bienvenida
    LCD_SetCursor(0, 3);
    LCD_Print("BIENVENIDOS");
    LCD_SetCursor(1, 1);
    LCD_Print("AVR Bare-Metal");
    delay_ms_tick(2000);
    LCD_Clear();

    LCD_SetCursor(0, 0); LCD_Print("Uptime: 0s");
    LCD_SetCursor(1, 0); LCD_Print("Estado: OFF");
}


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
        // Formato 00:00:00
        LCD_WriteChar((hh / 10) + '0'); LCD_WriteChar((hh % 10) + '0');
        LCD_WriteChar(':');
        LCD_WriteChar((mm / 10) + '0'); LCD_WriteChar((mm % 10) + '0');
        LCD_WriteChar(':');
        LCD_WriteChar((ss / 10) + '0'); LCD_WriteChar((ss % 10) + '0');
    }
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