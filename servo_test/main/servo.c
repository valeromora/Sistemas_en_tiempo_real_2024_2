#include "servo.h"
#include "esp_log.h"

#define SERVO_MIN_DUTY 409  // 1ms de pulso (0°)
#define SERVO_MAX_DUTY 818  // 2ms de pulso (180°)

static const char *TAG = "SERVO";

/**
 * @brief Convierte un ángulo en el valor de duty cycle correspondiente
 */
static int angle_to_duty(int angle) {
    return (angle * (SERVO_MAX_DUTY - SERVO_MIN_DUTY) / 180) + SERVO_MIN_DUTY;
}

void servo_init() {
    ESP_LOGI(TAG, "Inicializando servo en GPIO %d", SERVO_PIN);

    // Configurar el temporizador LEDC
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    // Configurar el canal LEDC
    ledc_channel_config_t ch_conf = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&ch_conf);
}

void servo_set_state(servo_state_t state) {
    int duty = angle_to_duty(state);
    ESP_LOGI(TAG, "Moviendo servo a %d° (duty %d)", state, duty);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}
