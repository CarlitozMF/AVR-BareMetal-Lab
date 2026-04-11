/**************************************************************************************
* PROYECTO: 08_TIM_Normal_Mode
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Sistema de control de motor paso a paso con orquesta de Timers (T0, T1, T2).
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "main_project_08.h"

/* --- Instancias Globales (Capa 2) --- */
Stepper_t motor_principal;
LCD_Config_t lcd_main_cfg;

/* --- Variables de Control (Capa 3) --- */
volatile bool motor_running   = false;
volatile Step_Dir_t motor_dir = STEP_CW;

/* --- Timestamps para Scheduler Cooperativo --- */
static uint32_t last_lcd_update = 0;
static uint32_t last_led_toggle = 0;

/* --- Programa Principal --- */
int main(void) {
    
    System_Init(); // Configuración integral de hardware y software

    while (1) {
        uint32_t current_tick = get_tick();

        /* TAREA: Gestión de Interfaz HMI */
        if (current_tick - last_lcd_update >= T_REFRESH_LCD_MS) { 
            last_lcd_update = current_tick;
            Task_Update_HMI(motor_running, motor_dir);
        }

        /* TAREA: Latido de Sistema (Systick) */
        if (current_tick - last_led_toggle >= T_BLINK_SYS_MS){
            last_led_toggle = current_tick;
            GPIO_TogglePin(LED_SYS_PORT, LED_SYS_PIN);
        }
    }
}

/* --- Implementación de Funciones de Sistema --- */

void System_Init(void) {
    /* 1. Inicialización de Motor PAP */
    volatile uint8_t* m_ports[] = {M1_IN1_PORT, M1_IN2_PORT, M1_IN3_PORT, M1_IN4_PORT};
    uint8_t m_pins[] = {M1_IN1_PIN, M1_IN2_PIN, M1_IN3_PIN, M1_IN4_PIN};
    Stepper_Init(&motor_principal, m_ports, m_pins, MODE_FULL_STEP);
    
    /* 2. Configuración de Entradas y Salidas GPIO */
    GPIO_InitPin(GPIO_D, BTN_START_STOP, GPIO_INPUT);
    GPIO_WritePin(GPIO_D, BTN_START_STOP, GPIO_HIGH); // Pull-up
    GPIO_InitPin(GPIO_D, BTN_DIR, GPIO_INPUT);
    GPIO_WritePin(GPIO_D, BTN_DIR, GPIO_HIGH);        // Pull-up

    GPIO_InitPin(GPIO_B, LED_BREATH, GPIO_OUTPUT);
    GPIO_InitPin(LED_SYS_PORT, LED_SYS_PIN, GPIO_OUTPUT);

    /* 3. Inicialización de LCD (Bus compartido) */
    GPIO_InitPin(LCD_PORT_DIR, LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_PORT_DIR, LCD_EN_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_PORT_DIR, LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_PORT_DIR, LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_PORT_DIR, LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_InitPin(LCD_PORT_DIR, LCD_D7_PIN, GPIO_OUTPUT);

    lcd_main_cfg.port_rs = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_rs = LCD_RS_PIN;
    lcd_main_cfg.port_en = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_en = LCD_EN_PIN;
    lcd_main_cfg.port_d4 = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_d4 = LCD_D4_PIN;
    lcd_main_cfg.port_d5 = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_d5 = LCD_D5_PIN;
    lcd_main_cfg.port_d6 = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_d6 = LCD_D6_PIN;
    lcd_main_cfg.port_d7 = LCD_PORT_DIR.PORT; lcd_main_cfg.pin_d7 = LCD_D7_PIN;
    lcd_main_cfg.type    = LCD_16X2;
    LCD_Init(&lcd_main_cfg);

    /* 4. Configuración de Eventos Externos */
    EXTI_Init(EXTI_INT0, EXTI_FALLING_EDGE); 
    EXTI_Init(EXTI_INT1, EXTI_FALLING_EDGE); 

    /* 5. Inicialización de Timers (Orquesta) */
    Systick_Init(TIMER_0);                     // T0: Sistema
    Timer1_Normal_Init(T1_CLK_64, T1_OFF, T1_OFF); // T1: Motor
    Timer1_Set_AlarmA(MOTOR_SPEED_DEFAULT);
    Timer2_Normal_Init(T2_CLK_1024, T1_OFF, T1_OFF); // T2: Asíncrono
    Timer2_Enable_OVF_INT(); 

    sei(); // Habilitación global
}

void Task_Update_HMI(bool running, Step_Dir_t dir) {
    LCD_SetCursor(0, 0);
    LCD_Print("Motor: ");
    LCD_Print(running ? "En Marcha " : "Detenido ");
    LCD_SetCursor(1, 0);
    LCD_Print("Dir: ");
    LCD_Print(dir == STEP_CW ? "DERECHA (->)" : "IZQUIERDA(<-)");
}

/* --- Manejadores de Interrupción (Eventos de Hardware) --- */

ISR(INT0_vect) {
    motor_running = !motor_running;
}

ISR(INT1_vect) {
    motor_dir = (motor_dir == STEP_CW) ? STEP_CCW : STEP_CW;
}

ISR(TIMER1_COMPA_vect) {
    if (motor_running) {
        motor_principal.is_active = true;
        motor_principal.direction = motor_dir;
        Stepper_Step_Sequential(&motor_principal);
    } else {
        Stepper_Stop(&motor_principal);
    }
    OCR1A += MOTOR_SPEED_DEFAULT;
}

ISR(TIMER2_OVF_vect) {
    static uint8_t count = 0;
    if (++count >= T2_OVF_COUNT_1S) { 
        GPIO_TogglePin(GPIO_B, LED_BREATH);
        count = 0;
    }
}