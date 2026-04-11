/**************************************************************************************
* PROYECTO: 06_LCD_Generic_Driver
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Sistema de monitoreo con interfaz HMI (LCD 16x2). Implementa multitarea
* cooperativa para gestionar un reloj de tiempo real (Uptime), un indicador de estado
* con caracteres personalizados y un latido de corazón (Heartbeat).
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "main_project_06.h"

/* --- Configuración del Carácter Personalizado (CGRAM) --- */
uint8_t char_rayo[] = {
    0b00010, 0b00100, 0b01000, 0b11111,
    0b00100, 0b01000, 0b10000, 0b00000
};

/* --- Instancia de Configuración del Driver LCD --- */
LCD_Config_t lcd_main_cfg;

/* --- Variables de Control de Tareas (Timestamps y Estados) --- */
static uint32_t t_prev_heartbeat = 0;
static uint32_t t_prev_boton     = 0;
static uint32_t t_prev_segundos  = 0;
static uint16_t segundos         = 0;
static uint8_t  estado_led       = 0;

/* --- Bucle Principal (Scheduler Cooperativo) --- */
int main(void) {
    
    System_Init(); // Inicialización de hardware y periféricos

    while (1) {
        Task_Heartbeat(); // Parpadeo de LED indicador de CPU activa
        Task_Contador();  // Gestión del tiempo y actualización de pantalla
        Task_Boton();     // Escaneo de entrada y control de actuadores
    }
    
    return 0;
}

/* --- Inicialización del Sistema --- */
void System_Init(void) {

    /* 1. Mapeo de Hardware y Configuración del Driver LCD */
    // Mapeamos los puertos y pines definidos en hw_project_06.h a la estructura del driver
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

    /* 2. Inicialización de Capa 2 (Dispositivos) */
    LCD_Init(&lcd_main_cfg);
    LCD_CreateCustomChar(0, char_rayo); // Cargamos el símbolo de rayo en el slot 0

    /* 3. Inicialización de Capa 1 (HAL) */
    GPIO_InitPin(GPIO_B, 5, GPIO_OUTPUT); // LED Heartbeat
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT); // LED de respuesta
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);  // Pulsador
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);  // Pull-up activa
    
    Systick_Init(TIMER_0); // Base de tiempo de 1ms
    sei();                 // Habilitación global de interrupciones

    /* 4. Pantalla de Bienvenida */
    LCD_SetCursor(0, 3); LCD_Print("BIENVENIDOS");
    LCD_SetCursor(1, 1); LCD_Print("AVR Bare-Metal");
    delay_ms_tick(2000); // Retardo basado en Systick
    LCD_Clear();

    // Plantilla base para la visualización de datos
    LCD_SetCursor(0, 0); LCD_Print("Uptime: 00:00:00");
    LCD_SetCursor(1, 0); LCD_Print("Estado: OFF");
}

/* --- Tareas del Sistema --- */

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

        // Cálculo de horas, minutos y segundos
        uint8_t hh = segundos / 3600;
        uint8_t mm = (segundos % 3600) / 60;
        uint8_t ss = segundos % 60;

        // Actualización parcial de la pantalla (solo el tiempo) para evitar parpadeos
        LCD_SetCursor(0, 8);
        LCD_WriteChar((hh / 10) + '0'); LCD_WriteChar((hh % 10) + '0');
        LCD_WriteChar(':');
        LCD_WriteChar((mm / 10) + '0'); LCD_WriteChar((mm % 10) + '0');
        LCD_WriteChar(':');
        LCD_WriteChar((ss / 10) + '0'); LCD_WriteChar((ss % 10) + '0');
    }
}

void Task_Boton(void) {
    // Escaneo de entrada con lógica negativa
    if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW) {
        if (get_tick() - t_prev_boton >= 200) { // Debounce por software
            estado_led = !estado_led;
            
            LCD_SetCursor(1, 8);
            if (estado_led) {
                GPIO_WritePin(GPIO_B, 3, GPIO_HIGH);
                LCD_Print(": ON  ");
                LCD_WriteChar(0); // Imprimimos el rayo personalizado
            } else {
                GPIO_WritePin(GPIO_B, 3, GPIO_LOW);
                LCD_Print(": OFF   "); // Espacios extra para limpiar caracteres residuales
            }
            t_prev_boton = get_tick();
        }
    }
}