#include "task_library.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "library_RGB.h"
#include "library_NTC.h"
#include "driver/uart.h"
#include "string.h"
#include "freertos/timers.h"
#include <stdlib.h>

extern TimerHandle_t ntc_timer;  
static led_rgb_struct my_led_rgb; 
adc_oneshot_unit_handle_t adc_handle; 

float red_threshold_min = 100;  // Inicialmente fuera de rango
float green_threshold_min = 100;
float green_threshold_max = -100;
float blue_threshold_max = -100;

void init_system(void) {
    init_rgb(&my_led_rgb, RED_PIN, GREEN_PIN, BLUE_PIN, 0, 1, 2, COMMON_CATHODE);

    adc_oneshot_unit_init_cfg_t adc_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_config, &adc_handle));

    //  Configuración del ADC para el NTC
    adc_oneshot_chan_cfg_t channel_config_ntc = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &channel_config_ntc));

    //  Configuración del ADC para el potenciómetro
    adc_oneshot_chan_cfg_t channel_config_pot = {
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_6, &channel_config_pot));

    init_NTC();
}


void uart_task(void *pvParameters) {
    uint8_t data[UART_BUF_SIZE];

    while (1) {

        int len = uart_read_bytes(UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(500));

        if (len > 0) {
            data[len] = '\0';  // Asegurar terminación correcta
            printf(" Comando recibido: [%s] (%d bytes)\n", data, len);

            if (strncmp((char *)data, "frec", 4) == 0) {
                int new_period = atoi((char *)&data[5]); 
                if (new_period >= 1000 && new_period <= 10000) { 
                    if (ntc_timer != NULL) {  
                        xTimerStop(ntc_timer, 0);
                        xTimerChangePeriod(ntc_timer, pdMS_TO_TICKS(new_period), 0);
                        xTimerStart(ntc_timer, 0);  
                        printf(" Frecuencia cambiada a %d ms\n", new_period);
                    } else {
                        printf("⚠ Error: Timer no inicializado.\n");
                    }
                } else {
                    printf("⚠ Valor fuera de rango. Usa entre 1000 y 10000 ms\n");
                }

            } else if (strncmp((char *)data, "red", 3) == 0) {
                red_threshold_min = atof((char *)&data[4]);  
                printf(" Umbral de LED Rojo ajustado a: %.2f°C\n", red_threshold_min);

            } else if (strncmp((char *)data, "green", 5) == 0) {
                sscanf((char *)&data[6], "%f %f", &green_threshold_min, &green_threshold_max);
                printf(" Umbral de LED Verde ajustado a: %.2f°C - %.2f°C\n", green_threshold_min, green_threshold_max);

            } else if (strncmp((char *)data, "blue", 4) == 0) {
                blue_threshold_max = atof((char *)&data[5]);  
                printf(" Umbral de LED Azul ajustado a: %.2f°C\n", blue_threshold_max);

            } else {
                printf(" Comando no reconocido: [%s]\n", data);
            }
        } 
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



void led_control_task(void *pvParameters) {
    while (1) {
        float current_temperature = get_last_NTC_temperature();
        int adc_value = 0;
        
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC1_CHANNEL_6, &adc_value)); // Leer potenciómetro
        uint8_t brightness = (adc_value * 100) / 4095;


        

        //  Mezcla de colores basada en los umbrales
        uint8_t red_duty = 0, green_duty = 0, blue_duty = 0;

        if (current_temperature < blue_threshold_max) {
            blue_duty = brightness; //  Azul
        }
        if (current_temperature >= green_threshold_min && current_temperature <= green_threshold_max) {
            green_duty = brightness; //  Verde
        }
        if (current_temperature > red_threshold_min) {
            red_duty = brightness; //  Rojo
        }

        // Configurar el LED con la mezcla de colores adecuada
        change_rgb_DUTY(&my_led_rgb, red_duty, green_duty, blue_duty);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}




void init_UART(void) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_param_config(UART_NUM, &uart_config);

    if (UART_NUM != UART_NUM_0) {
        uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}


    // Eliminar el driver solo si el UART no está inicializado
    if (uart_is_driver_installed(UART_NUM)) {
        uart_driver_delete(UART_NUM);
}

    

    if (uart_driver_install(UART_NUM, UART_BUF_SIZE * 2, UART_BUF_SIZE * 2, 10, NULL, 0) != ESP_OK) {
        printf(" Error al instalar el driver UART\n");
    } else {
        printf(" UART inicializada correctamente en TX=%d, RX=%d\n", TX_PIN, RX_PIN);
    }
}

