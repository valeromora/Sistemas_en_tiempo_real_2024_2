#include <stdio.h>
#include "library_ledc.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "library_adc.h"
#include "driver/adc.h"
#include "freertos/queue.h"



void app_main() {
    init_system();
    
    task_manager();

}
