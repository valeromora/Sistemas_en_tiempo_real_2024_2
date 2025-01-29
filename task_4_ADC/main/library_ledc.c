#include "library_ledc.h"

// Función para inicializar los LEDs RGB
void init_rgb(led_rgb_struct *recived_rgb_struct, uint32_t gpio_red_led, uint32_t gpio_green_led, uint32_t gpio_blue_led, led_logic recived_led_logic) {
    // Configuración del temporizador LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Inicialización de LEDs individuales
    static led_struct red_led = {0}, green_led = {0}, blue_led = {0};
    red_led = (led_struct){.channel = 0, .gpio_num = gpio_red_led, .duty = DEFAULT_DUTY, .logic_led = recived_led_logic};
    green_led = (led_struct){.channel = 1, .gpio_num = gpio_green_led, .duty = DEFAULT_DUTY, .logic_led = recived_led_logic};
    blue_led = (led_struct){.channel = 2, .gpio_num = gpio_blue_led, .duty = DEFAULT_DUTY, .logic_led = recived_led_logic};

    // Asignar las estructuras de LEDs al puntero RGB
    recived_rgb_struct->red_led = &red_led;
    recived_rgb_struct->green_led = &green_led;
    recived_rgb_struct->blue_led = &blue_led;

    // Configuración de canales LEDC
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .hpoint = 0
    };

    ledc_channel_config_t channels[] = {
        {.channel = red_led.channel, .gpio_num = red_led.gpio_num, .duty = red_led.duty},
        {.channel = green_led.channel, .gpio_num = green_led.gpio_num, .duty = green_led.duty},
        {.channel = blue_led.channel, .gpio_num = blue_led.gpio_num, .duty = blue_led.duty}
    };

    for (int i = 0; i < 3; ++i) {
        ledc_channel.channel = channels[i].channel;
        ledc_channel.gpio_num = channels[i].gpio_num;
        ledc_channel.duty = channels[i].duty;
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    }
}

// Función para cambiar el brillo de los LEDs RGB
void Change_RGB_LED_DUTY(led_rgb_struct *led_struct_recived, uint8_t duty_led_red, uint8_t duty_led_green, uint8_t duty_led_blue) {
    uint8_t duties[3] = {duty_led_red, duty_led_green, duty_led_blue};
    led_struct *leds[3] = {led_struct_recived->red_led, led_struct_recived->green_led, led_struct_recived->blue_led};

    for (int i = 0; i < 3; ++i) {
        // Limitar el brillo al valor máximo permitido
        if (duties[i] > MAXIMUM_ALLOWABLE_BRITHNESS)
            duties[i] = MAXIMUM_ALLOWABLE_BRITHNESS;

        // Calcular el duty
        leds[i]->duty = duties[i] * 256 / MAXIMUM_ALLOWABLE_BRITHNESS;

        // Ajustar según el tipo de lógica del LED
        if (leds[i]->logic_led == COMMON_ANODE)
            leds[i]->duty = 255 - leds[i]->duty;

        // Aplicar duty al canal LEDC
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, leds[i]->channel, leds[i]->duty));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, leds[i]->channel));
    }
}
