#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define RED_PIN    21  
#define GREEN_PIN  22  
#define BLUE_PIN   23 

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 4 kHz
#define LEDC_CHANNEL_RED        LEDC_CHANNEL_0
#define LEDC_CHANNEL_GREEN      LEDC_CHANNEL_1
#define LEDC_CHANNEL_BLUE       LEDC_CHANNEL_2

void init_rgb() {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_DUTY_RES,  // Resolución de 8 bits 
        .freq_hz =  LEDC_FREQUENCY,                      
        .speed_mode = LEDC_MODE,   // Modo de alta velocidad
        .timer_num = LEDC_TIMER             
    };
    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel[3] = {
        {.gpio_num = RED_PIN,   .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_RED, .intr_type = LEDC_INTR_DISABLE, .timer_sel = LEDC_TIMER, .duty = 255, .hpoint = 0},
        {.gpio_num = GREEN_PIN, .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_GREEN, .intr_type = LEDC_INTR_DISABLE, .timer_sel = LEDC_TIMER, .duty = 255, .hpoint = 0},
        {.gpio_num = BLUE_PIN,  .speed_mode = LEDC_MODE, .channel = LEDC_CHANNEL_BLUE, .intr_type = LEDC_INTR_DISABLE, .timer_sel = LEDC_TIMER, .duty = 255, .hpoint = 0}
    };

    for (int i = 0; i < 3; i++) {
        ledc_channel_config(&ledc_channel[i]);
    }
}

void set_red_led(int duty) {
    // Invertir el duty para LED de ánodo común
    int inverted_duty = 255 - duty;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, inverted_duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);
}

void set_green_led(int duty) {
    // Invertir el duty para LED de ánodo común
    int inverted_duty = 255 - duty;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, inverted_duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);
}

void set_blue_led(int duty) {
    // Invertir el duty para LED de ánodo común
    int inverted_duty = 255 - duty;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, inverted_duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);
}

void app_main(void)
{
    init_rgb();
     set_green_led(0);
    // Prueba: encender el LED rojo al máximo brillo
    set_red_led(255);   // Rojo encendido completamente   // Verde apagado
    set_blue_led(0);    // Azul apagado

    vTaskDelay(pdMS_TO_TICKS(2000));  // Esperar 2 segundos
}