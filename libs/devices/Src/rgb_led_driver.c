/**
 * @file rgb_led_driver.c
 * @brief Implementación de la lógica de control genérica para LEDs RGB.
 * @author Mamani Flores Carlos
 * @date 2026
 * * @details Este archivo contiene la implementación de las funciones de alto nivel
 * para el manejo de LEDs RGB. La arquitectura se basa en la separación de la 
 * lógica de color (Capa 2) y la manipulación de hardware (Capa 1), utilizando
 * callbacks para garantizar la portabilidad absoluta entre diferentes MCUs.
 */

#include "rgb_led_driver.h"

/**
 * @brief Inicializa la estructura de control y vincula el hardware.
 * * @details Realiza el "binding" de las funciones de PWM de bajo nivel con el
 * objeto LED RGB. Configura los parámetros iniciales y garantiza un estado
 * seguro (apagado) al finalizar la ejecución.
 * * @param led    Puntero al objeto de estado del LED.
 * @param type   Arquitectura del LED (@ref rgb_type_t).
 * @param max_br Límite de intensidad (0-255).
 * @param f_red  Puntero a la función PWM para el canal Rojo.
 * @param f_green Puntero a la función PWM para el canal Verde.
 * @param f_blue Puntero a la función PWM para el canal Azul.
 */
void RGB_Init(RGB_LED_t *led, rgb_type_t type, uint8_t max_br, 
              pwm_set_duty_t f_red, pwm_set_duty_t f_green, pwm_set_duty_t f_blue) {
    
    /* Validación de puntero de instancia */
    if (!led) return;

    /* Inyección de dependencias: Vinculación de funciones de Capa 1 */
    led->set_red   = f_red;
    led->set_green = f_green;
    led->set_blue  = f_blue;
    
    /* Configuración de parámetros de operación y límites */
    led->type = type;
    led->max_brightness = max_br;
    
    /** * @note Se invoca la actualización inicial para asegurar que el hardware 
     * refleje el estado de 'apagado' definido en la estructura. 
     */
    RGB_Set_Color_Direct(led, 0, 0, 0); 
}

/**
 * @brief Procesa y establece los valores de color en los periféricos.
 * * @details Esta función realiza tres pasos críticos:
 * 1. Clamping: Asegura que los valores no superen el brillo máximo definido.
 * 2. Inversión Lógica: Si el LED es Ánodo Común, invierte el Duty Cycle (255 - valor).
 * 3. Ejecución de Callbacks: Llama a las funciones de hardware inyectadas.
 * * @param led Puntero a la instancia del LED.
 * @param r   Intensidad lógica para el Rojo (0-255).
 * @param g   Intensidad lógica para el Verde (0-255).
 * @param b   Intensidad lógica para el Azul (0-255).
 */
void RGB_Set_Color_Direct(RGB_LED_t *led, uint8_t r, uint8_t g, uint8_t b) {
    
    if (!led) return;

    /* 1. Validación de límites (Clamping) contra max_brightness */
    if (r > led->max_brightness) r = led->max_brightness;
    if (g > led->max_brightness) g = led->max_brightness;
    if (b > led->max_brightness) b = led->max_brightness;

    /* 2. Procesamiento de Salida y Abstracción de Hardware */
    if (led->type == RGB_ANODE_COMMON) {
        /** * Ánodo Común: Lógica Invertida. 
         * Un valor de 0 en el registro PWM produce brillo máximo, 
         * por lo que se resta el valor deseado del rango máximo (255).
         */
        if(led->set_red)   led->set_red(255 - r);
        if(led->set_green) led->set_green(255 - g);
        if(led->set_blue)  led->set_blue(255 - b);
    } else {
        /** * Cátodo Común: Lógica Directa. 
         * El Duty Cycle es directamente proporcional a la intensidad lumínica.
         */
        if(led->set_red)   led->set_red(r);
        if(led->set_green) led->set_green(g);
        if(led->set_blue)  led->set_blue(b);
    }

    /* 3. Persistencia de estado para telemetría o efectos */
    led->current_r = r;
    led->current_g = g;
    led->current_b = b;
}

/**
 * @brief Aplica colores predefinidos mediante una interfaz simplificada.
 * * @details Facilita el uso desde la Capa de Aplicación permitiendo llamar a
 * colores estándar sin manipular valores RGB individuales. Utiliza el brillo
 * máximo configurado para los estados activos.
 * * @param led   Puntero a la instancia del LED.
 * @param color Selector de color predefinido (@ref RGB_PresetColor_t).
 */
void RGB_Set_Preset(RGB_LED_t *led, RGB_PresetColor_t color) {
    
    if (!led) return;
    
    uint8_t max = led->max_brightness;

    switch (color) {
        case COLOR_RED:     RGB_Set_Color_Direct(led, max, 0, 0);   break;
        case COLOR_GREEN:   RGB_Set_Color_Direct(led, 0, max, 0);   break;
        case COLOR_BLUE:    RGB_Set_Color_Direct(led, 0, 0, max);   break;
        case COLOR_YELLOW:  RGB_Set_Color_Direct(led, max, max, 0); break;
        case COLOR_CYAN:    RGB_Set_Color_Direct(led, 0, max, max); break;
        case COLOR_MAGENTA: RGB_Set_Color_Direct(led, max, 0, max); break;
        case COLOR_WHITE:   RGB_Set_Color_Direct(led, max, max, max); break;
        case COLOR_OFF:     RGB_Set_Color_Direct(led, 0, 0, 0);     break;
        default:                                                    break;
    }
}