#include <stdio.h>
#include "library_ledc.c"
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define RED_PIN    21  
#define GREEN_PIN  22  
#define BLUE_PIN   23 

void app_main(void)
{
    led_rgb_struct my_led_rgb;
    init_rgb( &my_led_rgb , RED_PIN , GREEN_PIN , BLUE_PIN, COMMON_CATHODE);
    Change_RGB_LED_DUTY(&my_led_rgb, 0, 100, 0);
}