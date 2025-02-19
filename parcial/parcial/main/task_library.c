#include "task_library.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "library_RGB.h"
#include "library_NTC.h"

static QueueHandle_t button_queue;
static led_rgb_struct my_led_rgb;
static led_state_t current_state = RESET; 
static uint8_t red_duty = 0, green_duty = 0, blue_duty = 0;
static ntc_sensor_t ntc_sensor;
static TimerHandle_t temperature_timer;
static uint32_t temperature_interval = 5000; // Intervalo en milisegundos

void temperature_timer_callback(TimerHandle_t xTimer) {
    float temperature = ntc_read_temperature(&ntc_sensor);
    printf("[Timer] Temperatura actual: %.2f °C\n", temperature);
}

void update_temperature_interval(uint32_t new_interval) {
    if (new_interval < 100) {
        new_interval = 100; // Evita valores demasiado bajos
    }
    temperature_interval = new_interval;
    xTimerChangePeriod(temperature_timer, pdMS_TO_TICKS(temperature_interval), 0);
    printf("Nuevo intervalo de temperatura: %lu ms\n", temperature_interval);
}

void init_system(void) {
    // Inicializar RGB
    init_rgb(&my_led_rgb, RED_PIN, GREEN_PIN, BLUE_PIN, 0, 1, 2, COMMON_CATHODE);
    
    // Inicializar el sensor NTC
    ntc_init(&ntc_sensor);
    
    // Configurar botón de interrupción
    init_button_interrupt();

    // Crear temporizador para la lectura de temperatura
    temperature_timer = xTimerCreate("TempTimer", pdMS_TO_TICKS(temperature_interval), pdTRUE, NULL, temperature_timer_callback);
    if (temperature_timer != NULL) {
        xTimerStart(temperature_timer, 0);
    }
}

static uint8_t read_potentiometer(void) {
    adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(ntc_sensor.adc_handle, ADC1_CHANNEL_6, &channel_cfg);
    
    int raw_value = 0;
    adc_oneshot_read(ntc_sensor.adc_handle, ADC1_CHANNEL_6, &raw_value);
    return (raw_value * 100) / 1023;
}

void task_manager(void) {
    int button_event;

    while (1) {
        if (xQueueReceive(button_queue, &button_event, 0)) {
            printf("Button event received! Current state: %d\n", current_state);
            gpio_intr_enable(BUTTON_PIN);

            switch (current_state) {
                case RESET:
                    current_state = RED;
                    break;
                case RED:
                    current_state = GREEN;
                    break;
                case GREEN:
                    current_state = BLUE;
                    break;
                case BLUE:
                    current_state = RESET;
                    break;
            }
            printf("Button pressed! New state: %d\n", current_state);
        }

        switch (current_state) {
            case RESET:
                change_rgb_DUTY(&my_led_rgb, 0, 0, 0);
                red_duty = green_duty = blue_duty = 0;
                break;

            case RED:
                red_duty = read_potentiometer();
                change_rgb_DUTY(&my_led_rgb, red_duty, 0, 0);
                break;

            case GREEN:
                green_duty = read_potentiometer();
                change_rgb_DUTY(&my_led_rgb, red_duty, green_duty, 0);
                break;

            case BLUE:
                blue_duty = read_potentiometer();
                change_rgb_DUTY(&my_led_rgb, red_duty, green_duty, blue_duty);
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void IRAM_ATTR button_isr_handler(void *arg) {
    int button_state = gpio_get_level(BUTTON_PIN);
    xQueueSendFromISR(button_queue, &button_state, NULL);
    gpio_intr_disable(BUTTON_PIN);
}

void init_button_interrupt(void) {
    gpio_config_t btn_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&btn_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    button_queue = xQueueCreate(10, sizeof(int));
}
