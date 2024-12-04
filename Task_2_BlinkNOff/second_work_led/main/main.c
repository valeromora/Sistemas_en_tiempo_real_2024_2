#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define LED_PIN 2
#define BUTTON_PIN GPIO_NUM_0

// Cola para la comunicación entre tareas
QueueHandle_t xButtonQueue;

typedef enum {
    STATE_1 = 0, // LED parpadea
    STATE_2 = 1  // LED apagado
} STATE_LED;

// Tarea para manejar el botón
void vButton_task(void* parameter) {
    static int last_button_state = 1; // Botón inicialmente en estado alto (pull-up)
    static STATE_LED state = STATE_1;
    while (1) {
        int current_button_state = gpio_get_level(BUTTON_PIN);

        if (last_button_state == 1 && current_button_state == 0) { // Botón presionado
            // Cambiar el estado del LED
            state = (state == STATE_1) ? STATE_2 : STATE_1;

            // Enviar el nuevo estado a la cola
            xQueueSend(xButtonQueue, &state, portMAX_DELAY);
        }

        last_button_state = current_button_state; // Actualizar el estado anterior
        vTaskDelay(50 / portTICK_PERIOD_MS); // Retardo para evitar rebotes
    }
}

// Tarea para manejar el LED
void vLed_task(void* parameter) {
    STATE_LED button_state;
    while (1) {
        if (xQueueReceive(xButtonQueue, &button_state, portMAX_DELAY)) {
            if (button_state == STATE_1) {
                while (button_state == STATE_1) {
                    gpio_set_level(LED_PIN, 1);
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                    gpio_set_level(LED_PIN, 0);
                    vTaskDelay(500 / portTICK_PERIOD_MS);

                    // Revisar si hay un nuevo estado
                    if (xQueueReceive(xButtonQueue, &button_state, 0)) {
                        break; // Salir del ciclo si el estado cambia
                    }
                }
            } else {
                gpio_set_level(LED_PIN, 0); // Apagar el LED
            }
        }
    }
}

void app_main(void) {
    // Crear la cola
    xButtonQueue = xQueueCreate(10, sizeof(STATE_LED));
    if (xButtonQueue == NULL) {
        printf("Error al crear la cola.\n");
        return;
    }

    // Configurar GPIO del botón como entrada con pull-up
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;       // Sin interrupciones
    io_conf.mode = GPIO_MODE_INPUT;              // Modo entrada
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN); // Seleccionar el pin del botón
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;     // Activar pull-up interno
    gpio_config(&io_conf);

    // Configurar GPIO del LED como salida
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Crear las tareas
    xTaskCreate(vButton_task, "Button Task", 2048, NULL, 1, NULL);
    xTaskCreate(vLed_task, "LED Task", 2048, NULL, 1, NULL);
}
