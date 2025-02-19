#include <stdio.h>
#include "library_RGB.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "task_library.h"
#include "driver/adc.h"
#include "freertos/queue.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "library_NTC.h"
#include <math.h>
#include "task_library.h"

static const char *TAG = "MAIN";

void app_main(void) {
    ESP_LOGI("MAIN", " Iniciando sistema...");
    
    init_system();
    init_UART();
    

    xTaskCreate(led_control_task, "LED_Control_Task", 4096, NULL, 2, NULL);
    xTaskCreate(uart_task, "UART_Task", 4096, NULL, 2, NULL);

    
    vTaskDelay(pdMS_TO_TICKS(2000));
}
