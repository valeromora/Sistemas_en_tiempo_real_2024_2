#include <stdio.h>
#include "library_ledc.c"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RED_PIN    21  
#define GREEN_PIN  22  
#define BLUE_PIN   23 
#define BUTTON_PIN 0

// Definir colores del LED
#define NUM_COLORS 3

int colors[NUM_COLORS][3] = {
    {0, 100, 100},   // Rojo
    {100, 0, 100},   // Verde
    {100, 100, 0},   // Azul
};

void app_main(void)
{
    // Set-up
    led_rgb_struct my_led_rgb;
    init_rgb(&my_led_rgb, RED_PIN, GREEN_PIN, BLUE_PIN, COMMON_CATHODE);

    // Configurar el botón
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;     // Sin interrupciones, GPIO_INTR_NEGEDGE Interrupción en flanco negativo (cuando se presiona)
    io_conf.mode = GPIO_MODE_INPUT; 
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;   // Activar pull-up interno
    gpio_config(&io_conf);

    int current_color = 0;

    while (1) {
        // Leer el estado del botón
        if (gpio_get_level(BUTTON_PIN) == 0) {
            // Cambiar al siguiente color
            current_color = (current_color + 1) % NUM_COLORS;
            Change_RGB_LED_DUTY(&my_led_rgb,
                                colors[current_color][0], 
                                colors[current_color][1], 
                                colors[current_color][2]);

            // anti-rebote
            while (gpio_get_level(BUTTON_PIN) == 0) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
