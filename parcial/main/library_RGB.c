#include <library_RGB.h>



void init_rgb(led_rgb_struct *recived_rgb_struct, 
              uint32_t gpio_red_led, uint32_t gpio_green_led, uint32_t gpio_blue_led, 
              uint8_t red_channel, uint8_t green_channel, uint8_t blue_channel,
              led_logic recived_led_logic) {
    // Reservar memoria dinámica para cada LED
    led_struct *red_led = (led_struct *)malloc(sizeof(led_struct));
    led_struct *green_led = (led_struct *)malloc(sizeof(led_struct));
    led_struct *blue_led = (led_struct *)malloc(sizeof(led_struct));

    // Configurar cada LED
    red_led->gpio_num = gpio_red_led;
    red_led->channel = red_channel;
    red_led->duty = DEFAULT_DUTY;
    red_led->logic_led = recived_led_logic;

    green_led->gpio_num = gpio_green_led;
    green_led->channel = green_channel;
    green_led->duty = DEFAULT_DUTY;
    green_led->logic_led = recived_led_logic;

    blue_led->gpio_num = gpio_blue_led;
    blue_led->channel = blue_channel;
    blue_led->duty = DEFAULT_DUTY;
    blue_led->logic_led = recived_led_logic;

    // Asignar punteros a la estructura RGB
    recived_rgb_struct->red_led = red_led;
    recived_rgb_struct->green_led = green_led;
    recived_rgb_struct->blue_led = blue_led;

    // Configuración del temporizador
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Configuración de canales
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .hpoint = 0
    };

    // Rojo
    ledc_channel.channel = red_led->channel;
    ledc_channel.gpio_num = red_led->gpio_num;
    ledc_channel.duty = red_led->duty;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Verde
    ledc_channel.channel = green_led->channel;
    ledc_channel.gpio_num = green_led->gpio_num;
    ledc_channel.duty = green_led->duty;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Azul
    ledc_channel.channel = blue_led->channel;
    ledc_channel.gpio_num = blue_led->gpio_num;
    ledc_channel.duty = blue_led->duty;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Depuración
    printf("RGB initialized: Red GPIO %lu, Green GPIO %lu, Blue GPIO %lu\n",
           (unsigned long)red_led->gpio_num,
           (unsigned long)green_led->gpio_num,
           (unsigned long)blue_led->gpio_num);
}




void change_rgb_DUTY(led_rgb_struct *led_struct_recived, uint8_t duty_led_red, uint8_t duty_led_green, uint8_t duty_led_blue) {
    
    

    // Ajustar valores dentro del rango permitido
    if (duty_led_red > MAX_BRIGHT) duty_led_red = MAX_BRIGHT;
    if (duty_led_green > MAX_BRIGHT) duty_led_green = MAX_BRIGHT;
    if (duty_led_blue > MAX_BRIGHT) duty_led_blue = MAX_BRIGHT;

    // **Si el LED es de CÁTODO COMÚN, invertir los valores**
    if (led_struct_recived->red_led->logic_led == COMMON_CATHODE) {
        duty_led_red = MAX_BRIGHT - duty_led_red;
        duty_led_green = MAX_BRIGHT - duty_led_green;
        duty_led_blue = MAX_BRIGHT - duty_led_blue;
    }

    // Apagar todos los LEDs antes de actualizar para evitar mezcla de colores
    ledc_set_duty(LEDC_MODE, led_struct_recived->red_led->channel, 0);
    ledc_update_duty(LEDC_MODE, led_struct_recived->red_led->channel);

    ledc_set_duty(LEDC_MODE, led_struct_recived->green_led->channel, 0);
    ledc_update_duty(LEDC_MODE, led_struct_recived->green_led->channel);

    ledc_set_duty(LEDC_MODE, led_struct_recived->blue_led->channel, 0);
    ledc_update_duty(LEDC_MODE, led_struct_recived->blue_led->channel);

    // Aplicar los nuevos valores de PWM
    ledc_set_duty(LEDC_MODE, led_struct_recived->red_led->channel, duty_led_red * 255 / MAX_BRIGHT);
    ledc_update_duty(LEDC_MODE, led_struct_recived->red_led->channel);

    ledc_set_duty(LEDC_MODE, led_struct_recived->green_led->channel, duty_led_green * 255 / MAX_BRIGHT);
    ledc_update_duty(LEDC_MODE, led_struct_recived->green_led->channel);

    ledc_set_duty(LEDC_MODE, led_struct_recived->blue_led->channel, duty_led_blue * 255 / MAX_BRIGHT);
    ledc_update_duty(LEDC_MODE, led_struct_recived->blue_led->channel);
}