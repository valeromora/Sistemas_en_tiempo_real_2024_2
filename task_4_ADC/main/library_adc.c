#include "library_adc.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "library_ledc.h"

static QueueHandle_t     button_queue;
static led_rgb_struct    my_led_rgb;
static led_state_t       current_state = RESET; 
static uint8_t           red_duty = 0, green_duty = 0, blue_duty = 0;


void init_system(void){

    //inicializar el rgb
    init_rgb(&my_led_rgb, RED_PIN, GREEN_PIN, BLUE_PIN, COMMON_CATHODE);


    //configurar adc
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

    init_button_interrupt();
}

static uint8_t read_potenciometer(void){
    int adc_value = adc1_get_raw(ADC1_CHANNEL_6);    
    return (adc_value * 100)/1023;
}




void task_manager(void) {
    int button_event;

    while (1) {
        // Verificar si hay un evento en la cola para cambiar de estado
        if (xQueueReceive(button_queue, &button_event, 0)) {  // No bloquear la tarea
            printf("Button event received! Current state: %d\n", current_state);
            gpio_intr_enable(BUTTON_PIN);

            // Cambiar al siguiente estado
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

        // Ejecutar acciones según el estado actual (lectura continua del potenciómetro)
        switch (current_state) {
            case RESET:
                // Apagar todos los LEDs
                Change_RGB_LED_DUTY(&my_led_rgb, 0, 0, 0);
                red_duty = green_duty = blue_duty = 0;
                break;

            case RED:
                // Leer el potenciómetro y actualizar LED rojo
                red_duty = read_potenciometer();
                Change_RGB_LED_DUTY(&my_led_rgb, red_duty, 0, 0);
                printf("RED state active. red_duty: %d\n", red_duty);
                break;

            case GREEN:
                // Leer el potenciómetro y actualizar LED verde
                green_duty = read_potenciometer();
                Change_RGB_LED_DUTY(&my_led_rgb, red_duty, green_duty, 0);
                printf("GREEN state active. green_duty: %d\n", green_duty);
                break;

            case BLUE:
                // Leer el potenciómetro y actualizar LED azul
                blue_duty = read_potenciometer();
                Change_RGB_LED_DUTY(&my_led_rgb, red_duty, green_duty, blue_duty);
                printf("BLUE state active. blue_duty: %d\n", blue_duty);
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // Reducir carga de CPU
    }
}
void IRAM_ATTR button_isr_handler(void *arg){
    int button_state = gpio_get_level(BUTTON_PIN);
    xQueueSendFromISR(button_queue, &button_state, NULL);
    gpio_intr_disable(BUTTON_PIN);
}

// Inicializar interrupción del botón
void init_button_interrupt(void) {
    // Configuración del botón
    gpio_config_t btn_config = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE // Detectar flanco descendente
    };
    gpio_config(&btn_config);

    // Instalar servicio de ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Crear cola para manejar eventos del botón
    button_queue = xQueueCreate(10, sizeof(int));
    printf("Button interrupt initialized on GPIO %d\n", BUTTON_PIN);
}