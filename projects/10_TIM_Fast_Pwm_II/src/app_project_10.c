/**
 * @file app_project_10.c
 * @brief Implementación de la lógica de aplicación, servicios y tareas cooperativas.
 * @author Mamani Flores Carlos - CarlitozMF
 * @date 2026
 * * @details Este archivo orquesta el comportamiento del sistema utilizando una 
 * arquitectura de software por capas. Gestiona la coexistencia de:
 * - Generación de señales PWM para LED RGB y un Dimmer manual.
 * - Sincronización de tareas mediante el Systick (Timer 1).
 * - Procesamiento asincrónico de eventos mediante interrupciones externas (EXTI).
 */

#include "app_project_10.h"
#include "hw_project_10.h"
#include "rgb_led_driver.h"
#include "gpio.h"
#include "exti.h"
#include "systick.h"

/* --- 1. Variables Privadas (Encapsulamiento) --- */

/** * @brief Instancia del controlador de LED RGB.
 * @note Se declara estática para restringir su acceso únicamente a este módulo (Capa 3).
 */
static RGB_LED_t led_status;

/** * @brief Estados de la máquina de estados para la transición cromática.
 */
typedef enum {
    RAINBOW_G_IN,   /**< Incremento de Verde (Rojo fijo) */
    RAINBOW_R_OUT,  /**< Decremento de Rojo (Verde fijo) */
    RAINBOW_B_IN,   /**< Incremento de Azul (Verde fijo) */
    RAINBOW_G_OUT,  /**< Decremento de Verde (Azul fijo) */
    RAINBOW_R_IN,   /**< Incremento de Rojo (Azul fijo) */
    RAINBOW_B_OUT   /**< Decremento de Azul (Rojo fijo) */
} rainbow_state_t;

/** * @brief Bandera de señalización para el pulsador.
 * @details Declarada como 'volatile' para informar al compilador que su valor 
 * puede cambiar fuera del flujo normal del programa (dentro de una ISR).
 */
volatile uint8_t pulsador_presionado = 0;

/* --- 2. Implementación de Funciones Globales --- */

/**
 * @brief Inicialización de orquestación del sistema.
 * @details Configura secuencialmente el hardware HAL, los drivers de dispositivo
 * y las funciones de control de usuario.
 */
void Sys_Init(void) {
    /* --- 1. Inicialización de Periféricos HAL (Capa 1) --- */
    
    // Timer 0: Generación de PWM para Rojo (OC0A) y Verde (OC0B)
    Timer0_PWM_Init(T0_PWM_CLK_64);
    Timer0_PWM_EnableChannel(T0_PWM_CH_A, T0_PWM_NON_INVERTING);
    Timer0_PWM_EnableChannel(T0_PWM_CH_B, T0_PWM_NON_INVERTING);

    // Timer 2: Generación de PWM para Azul (OC2A) y LED de Usuario (OC2B)
    Timer2_PWM_Fast_Init(T2_PWM_CLK_64);
    Timer2_PWM_Fast_EnableChannel(T2_PWM_CH_A, T2_PWM_NON_INVERTING);

    /* --- 2. Inicialización de Drivers de Dispositivo (Capa 2) --- */
    
    // Inyección de dependencias: se pasan los wrappers de HW al driver RGB
    RGB_Init(&led_status, RGB_ANODE_COMMON, 255, set_r_hw, set_g_hw, set_b_hw);

    /* --- 3. Configuración de Interfaces de Usuario (Capa 3) --- */
    
    // LED de Sistema (Heartbeat) - Configurado como salida simple
    GPIO_InitPin(LED_SYS_PORT, LED_SYS_PIN, GPIO_OUTPUT);

    // Configuración del Pulsador: Entrada con Pull-up activo
    GPIO_InitPin(BTN_PORT, BTN_PIN, GPIO_INPUT);
    GPIO_WritePin(BTN_PORT, BTN_PIN, GPIO_HIGH); 
    
    // Configuración de EXTI: Disparo por flanco de bajada (lógica del pulsador)
    EXTI_Init(BTN_EXTI_LINE, EXTI_FALLING_EDGE);
}

/**
 * @brief Tarea de actualización del efecto Arcoíris.
 * @param interval Tiempo en ms entre actualizaciones de color.
 * @param step Salto de intensidad (0-255) por cada iteración.
 */
void Task_Rainbow(uint32_t interval, uint8_t step) {
    static uint32_t last_tick = 0;
    static rainbow_state_t state = RAINBOW_G_IN;
    static uint8_t r = 255, g = 0, b = 0;

    // Control de tiempo no bloqueante mediante Systick
    if ((get_tick() - last_tick) < interval) return;
    last_tick = get_tick();

    uint8_t max = led_status.max_brightness;

    /* Implementación de la transición cromática circular */
    switch (state) {
        case RAINBOW_G_IN:
            if ((uint16_t)g + step >= max) { g = max; state = RAINBOW_R_OUT; } else g += step;
            break;
        case RAINBOW_R_OUT:
            if (r <= step) { r = 0; state = RAINBOW_B_IN; } else r -= step;
            break;
        case RAINBOW_B_IN:
            if ((uint16_t)b + step >= max) { b = max; state = RAINBOW_G_OUT; } else b += step;
            break;
        case RAINBOW_G_OUT:
            if (g <= step) { g = 0; state = RAINBOW_R_IN; } else g -= step;
            break;
        case RAINBOW_R_IN:
            if ((uint16_t)r + step >= max) { r = max; state = RAINBOW_B_OUT; } else r += step;
            break;
        case RAINBOW_B_OUT:
            if (b <= step) { b = 0; state = RAINBOW_G_IN; } else b -= step;
            break;
    }

    // Actualización del hardware a través del driver genérico
    RGB_Set_Color_Direct(&led_status, r, g, b);
}

/**
 * @brief Tarea de Heartbeat del sistema.
 * @param interval Tiempo en ms para el cambio de estado del LED.
 */
void Task_Toggle(uint32_t interval){
    static uint32_t prev_heartbeat = 0;
    
    if (get_tick() - prev_heartbeat >= interval) {
        GPIO_TogglePin(LED_SYS_PORT, LED_SYS_PIN);
        prev_heartbeat = get_tick();
    }
}

/**
 * @brief Tarea de control manual del Dimmer.
 * @details Procesa la bandera levantada por la ISR del pulsador. Aplica 5 niveles 
 * de brillo (ciclos del 20%) y gestiona el apagado total del periférico.
 */
void task_button_led(void) {
    static uint8_t brillo_manual = 0;
    
    if (pulsador_presionado) {
        // Incremento circular en 5 pasos (0, 51, 102, 153, 204, 255 -> 0)
        if (brillo_manual >= 255) brillo_manual = 0; 
        else brillo_manual += 51; 
        
        if (brillo_manual == 0) {
            // Desactivación del periférico PWM para asegurar 0V en el pin
            Timer2_PWM_Fast_DisableChannel(LED_BTN_CH);
            GPIO_WritePin(LED_BTN_PORT, LED_BTN_PIN, GPIO_LOW);
        } else {
            // Reactivación y carga de nuevo Duty Cycle
            Timer2_PWM_Fast_EnableChannel(LED_BTN_CH, T2_PWM_NON_INVERTING);
            Timer2_PWM_Fast_SetDuty(LED_BTN_CH, brillo_manual);
        }
        pulsador_presionado = 0; // Consumo de la bandera de evento
    }
}

/* --- 3. Rutinas de Servicio de Interrupción (ISR) --- */

/**
 * @brief ISR para Interrupción Externa 0 (INT0).
 * @details Detecta la pulsación y aplica un filtro de rebotes (Debouncing) 
 * por software comparando el tiempo actual con el de la última interrupción.
 */
ISR(INT0_vect) {
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time = get_tick();
    
    // Ventana de inhibición de 200ms para evitar falsos disparos mecánicos
    if (current_time - last_interrupt_time > 200) {
        pulsador_presionado = 1;
        last_interrupt_time = current_time;
    }
}