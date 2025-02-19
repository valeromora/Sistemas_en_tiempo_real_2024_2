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


void app_main(void) {
    init_system();
    task_manager();
 
}