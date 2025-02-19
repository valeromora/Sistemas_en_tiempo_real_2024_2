
#ifndef LIBRARY_RGB_H 
#define LIBRARY_RGB_H 

#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "library_NTC.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_FREQUENCY          (1000) // Frequency in Hertz. Set frequency at 4 kHz
#define DEFAULT_DUTY            0
#define MAX_BRIGHT              100

typedef enum{
    COMMON_CATHODE,
    COMMON_ANNODE,
}led_logic;

typedef struct{
    uint32_t channel;
    int gpio_num;
    uint32_t duty;
    led_logic logic_led;
}led_struct;

typedef struct{ 
    led_struct *red_led;
    led_struct *green_led;
    led_struct *blue_led;
}led_rgb_struct;

void init_rgb(led_rgb_struct *recived_rgb_struct, 
              uint32_t gpio_red_led, uint32_t gpio_green_led, uint32_t gpio_blue_led, 
              uint8_t red_channel, uint8_t green_channel, uint8_t blue_channel,
              led_logic recived_led_logic);
void change_rgb_DUTY(led_rgb_struct *led_struct_recived, uint8_t duty_led_red, uint8_t duty_led_green, uint8_t duty_led_blue);

#endif 