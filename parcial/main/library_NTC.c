#include "library_NTC.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

extern adc_oneshot_unit_handle_t adc_handle;  // ADC compartido

const static char *TAG = "NTC_SENSOR";

// Variables globales
static adc_cali_handle_t cali_handle;
TimerHandle_t ntc_timer = NULL;
static float last_temperature = -999;

// Tarea para leer la temperatura
void ntc_read_task(void *pvParameters) {
    last_temperature = read_NTC_temperature();

    if (last_temperature != -999) {  
        printf("🌡 Temp: %.2f°C\n", last_temperature);
    }

    vTaskDelete(NULL);  // 🔹 Elimina la tarea después de ejecutarse una vez
}

// Callback del Timer: Solo crea la tarea para leer temperatura
static void ntc_timer_callback(TimerHandle_t xTimer) {
    xTaskCreatePinnedToCore(ntc_read_task, "NTC_Read_Task", 2048, NULL, 2, NULL, 0);
}

// Inicializa el ADC y el timer
void init_NTC(void) {
    if (adc_handle == NULL) {  
        ESP_LOGE(TAG, "Error: `adc_handle` no está inicializado.");
        return;
    }

    adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &channel_cfg));

    ESP_LOGI(TAG, " NTC inicializado correctamente.");

    // Verificar si el timer ya existe antes de crearlo
    if (ntc_timer == NULL) {
        ntc_timer = xTimerCreate("NTC_Timer", pdMS_TO_TICKS(NTC_SAMPLING_PERIOD_MS), pdTRUE, NULL, ntc_timer_callback);
        if (ntc_timer != NULL) {
            xTimerStart(ntc_timer, 0);
            ESP_LOGI(TAG, " Timer de temperatura iniciado cada %d ms.", NTC_SAMPLING_PERIOD_MS);
        } else {
            ESP_LOGE(TAG, " Error al crear el timer.");
        }
    }
}

// Función para leer el voltaje del NTC
float read_NTC_voltage(void) {
    if (adc_handle == NULL) {  
        ESP_LOGE(TAG, " Error: `adc_handle` no está inicializado.");
        return -1;
    }

    int raw_value = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL, &raw_value));

    int voltageMv = 0;
    if (cali_handle != NULL) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_value, &voltageMv));
    } else {
        
        voltageMv = raw_value * (REFERENCE_VOLTAGE * 1000) / 4095;
    }

    float voltage = voltageMv / 1000.0;
    
    return voltage;
}

// Función para calcular la temperatura
float read_NTC_temperature(void) {
    float voltage = read_NTC_voltage();

    if (voltage <= 0) {
        ESP_LOGW(TAG, " Error: Voltaje inválido.");
        return -999;
    }

    float resistance = (voltage * REFERENCE_RESISTOR) / (REFERENCE_VOLTAGE - voltage);
    float tempK = 1 / (((log(resistance / NOMINAL_RESISTANCE)) / BETA) + (1 / (NOMINAL_TEMPERATURE + 273.15)));
    float tempC = tempK - 273.15;

    
    return tempC;
}

// Devuelve la última temperatura leída
float get_last_NTC_temperature(void) {
    return last_temperature;
}
