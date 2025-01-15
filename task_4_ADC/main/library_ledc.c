#include "library_ledc.h"


void init_rgb(led_rgb_struct *recived_rgb_struct, uint32_t gpio_red_led, uint32_t gpio_green_led, uint32_t gpio_blue_led, led_logic recived_led_logic) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,   // Modo de alta velocidad
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,  // Resolución de 8 bits 
        .freq_hz =  LEDC_FREQUENCY,                      
        .clk_cfg = LEDC_AUTO_CLK     
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    led_struct red_led;
    red_led.gpio_num = gpio_red_led;
    red_led.channel = 0;
    red_led.duty = DEFAULT_DUTY;
    red_led.logic_led =  recived_led_logic;

    led_struct green_led;
    green_led.gpio_num = gpio_green_led;
    green_led.channel = 1;
    green_led.duty = DEFAULT_DUTY;
    green_led.logic_led =  recived_led_logic;

    led_struct blue_led;
    blue_led.gpio_num = gpio_blue_led;
    blue_led.channel = 2;
    blue_led.duty = DEFAULT_DUTY;
    blue_led.logic_led =  recived_led_logic;

    led_rgb_struct struct_led_rgb;
    recived_rgb_struct->red_led = &red_led;
    recived_rgb_struct->green_led = &green_led;
    recived_rgb_struct->blue_led = &blue_led;

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        =recived_rgb_struct->red_led->channel,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       =recived_rgb_struct->red_led->gpio_num,
        .duty           =recived_rgb_struct->red_led->duty, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    //green led config
    ledc_channel.channel =recived_rgb_struct->green_led->channel;
    ledc_channel.gpio_num =recived_rgb_struct->green_led->gpio_num;
    ledc_channel.duty =recived_rgb_struct->green_led->duty;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    //blue led config
    ledc_channel.channel =recived_rgb_struct->blue_led->channel;
    ledc_channel.gpio_num =recived_rgb_struct->blue_led->gpio_num;
    ledc_channel.duty =recived_rgb_struct->blue_led->duty;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    //return struct_led_rgb

}

void Change_RGB_LED_DUTY(led_rgb_struct *led_struct_recived, uint8_t duty_led_red, uint8_t duty_led_green,uint8_t duty_led_blue){
    if (duty_led_red > MAXIMUM_ALLOWABLE_BRITHNESS)
        duty_led_red = MAXIMUM_ALLOWABLE_BRITHNESS;

    if (duty_led_green > MAXIMUM_ALLOWABLE_BRITHNESS)
        duty_led_green = MAXIMUM_ALLOWABLE_BRITHNESS;

    if (duty_led_blue > MAXIMUM_ALLOWABLE_BRITHNESS)
        duty_led_blue = MAXIMUM_ALLOWABLE_BRITHNESS;
    
    //uptade red led
    led_struct_recived -> red_led -> duty = duty_led_red * 256/MAXIMUM_ALLOWABLE_BRITHNESS;
    if (led_struct_recived->red_led->logic_led == COMMON_ANODE)
        led_struct_recived-> red_led->duty = 255 - led_struct_recived-> red_led->duty;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, led_struct_recived -> red_led -> channel, led_struct_recived -> red_led -> duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, led_struct_recived -> red_led -> channel));

    //uptade green led
    led_struct_recived -> green_led -> duty = duty_led_green * 256/MAXIMUM_ALLOWABLE_BRITHNESS;
    if (led_struct_recived->green_led->logic_led == COMMON_ANODE)
        led_struct_recived-> green_led->duty = 255 - led_struct_recived-> green_led->duty;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, led_struct_recived -> green_led -> channel, led_struct_recived -> green_led -> duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, led_struct_recived -> green_led -> channel));

    //uptade blue led
    led_struct_recived -> blue_led -> duty = duty_led_blue * 256/MAXIMUM_ALLOWABLE_BRITHNESS;
    if (led_struct_recived->blue_led->logic_led == COMMON_ANODE)
        led_struct_recived-> blue_led->duty = 255 - led_struct_recived-> blue_led->duty;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, led_struct_recived -> blue_led -> channel, led_struct_recived -> blue_led -> duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, led_struct_recived -> blue_led -> channel));
}