#include "library_NTC.h"
#include "task_library.h"
#include "library_RGB.h" 
#include "library_RGB.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "library_NTC.h"

#include "library_NTC.h"

static const char *TAG = "NTC";

void ntc_init(ntc_sensor_t *sensor) {
    if (sensor->is_initialized) {
        ESP_LOGW(TAG, "NTC ya inicializado");
        return;
    }

    ESP_LOGI(TAG, "Inicializando NTC...");
    
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = NTC_ADC_UNIT,
    };
    if (adc_oneshot_new_unit(&init_cfg, &sensor->adc_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Error al inicializar ADC");
        return;
    }

    adc_oneshot_chan_cfg_t channel_cfg = {
        .atten = NTC_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (adc_oneshot_config_channel(sensor->adc_handle, NTC_ADC_CHANNEL, &channel_cfg) != ESP_OK) {
        ESP_LOGE(TAG, "Error al configurar canal ADC");
        return;
    }

    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = NTC_ADC_UNIT,
        .atten = NTC_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    if (adc_cali_create_scheme_line_fitting(&cali_cfg, &sensor->cali_handle) != ESP_OK) {
        ESP_LOGE(TAG, "Error en la calibración del ADC");
        return;
    }

    sensor->is_initialized = true;
    ESP_LOGI(TAG, "NTC inicializado correctamente");
}

float ntc_read_voltage(ntc_sensor_t *sensor) {
    if (!sensor->is_initialized) {
        ESP_LOGE(TAG, "Intento de leer ADC sin inicialización");
        return -1;
    }

    int raw_value = 0;
    if (adc_oneshot_read(sensor->adc_handle, NTC_ADC_CHANNEL, &raw_value) != ESP_OK) {
        ESP_LOGE(TAG, "Error al leer el ADC");
        return -1;
    }

    int voltageMv = 0;
    if (adc_cali_raw_to_voltage(sensor->cali_handle, raw_value, &voltageMv) != ESP_OK) {
        ESP_LOGE(TAG, "Error en la conversión de voltaje");
        return -1;
    }

    return voltageMv / 1000.0;
}

float ntc_read_temperature(ntc_sensor_t *sensor) {
    float voltage = ntc_read_voltage(sensor);
    if (voltage <= 0) {
        ESP_LOGW(TAG, "Voltaje inválido");
        return -999;
    }

    float resistance = (voltage * REFERENCE_RESISTOR) / (REFERENCE_VOLTAGE - voltage);
    float tempK = 1 / (((log(resistance / NOMINAL_RESISTANCE)) / BETA_COEFFICIENT) + (1 / (NOMINAL_TEMPERATURE + 273.15)));
    return tempK - 273.15;
}
