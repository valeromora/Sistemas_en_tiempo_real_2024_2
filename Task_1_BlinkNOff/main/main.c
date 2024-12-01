#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define LED_PIN 2
#define BUTTON_PIN GPIO_NUM_0

void app_main(void)
{
// Configurar GPIO 0 como entrada con pull-up
gpio_config_t io_conf = {};
io_conf.intr_type = GPIO_INTR_DISABLE;     // Sin interrupciones, GPIO_INTR_NEGEDGE Interrupción en flanco negativo (cuando se presiona)
io_conf.mode = GPIO_MODE_INPUT; 
io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
io_conf.pull_up_en = GPIO_PULLUP_ENABLE;   // Activar pull-up interno
gpio_config(&io_conf);

//configurar led como salida
gpio_reset_pin(LED_PIN);
gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
int last_button_state = 1; // Estado previo del botón (1 = no presionado)
int led_state = 0;  

typedef enum{
    STATE_1 = 0,
    STATE_2 = 1
}STATE_LED;
STATE_LED state = STATE_1;
while (1) {
        // Leer el estado del botón
        int current_button_state = gpio_get_level(BUTTON_PIN);
        vTaskDelay(100 / portTICK_PERIOD_MS); 
       if (last_button_state == 1 && current_button_state == 0) {
        state = !state;
            }
        if (state == 0) {
            gpio_set_level(LED_PIN, 1); // Encender el LED
            vTaskDelay(500 / portTICK_PERIOD_MS); // Esperar 500 ms
            gpio_set_level(LED_PIN, 0); // Apagar el LED
            vTaskDelay(500 / portTICK_PERIOD_MS); // Esperar 500 ms
            
        }
         if (state == 1){
            gpio_set_level(LED_PIN, 0);
            
        }
       last_button_state = current_button_state;
    }
}
