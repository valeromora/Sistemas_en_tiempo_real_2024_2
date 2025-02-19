#ifndef LIBRARY_NTC_H
#define LIBRARY_NTC_H

#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Definiciones del ADC
#define NTC_ADC_CHANNEL     ADC_CHANNEL_7  // Ajusta según tu hardware
#define NTC_ADC_UNIT        ADC_UNIT_1
#define NTC_ADC_ATTEN       ADC_ATTEN_DB_11

// Parámetros del NTC
#define REFERENCE_VOLTAGE   3.3
#define REFERENCE_RESISTOR  10000  // 10k Ohmios
#define BETA_COEFFICIENT    3950
#define NOMINAL_RESISTANCE  10000
#define NOMINAL_TEMPERATURE 25  // Temperatura de referencia en grados Celsius

// Estructura para manejar el NTC
typedef struct {
    adc_oneshot_unit_handle_t adc_handle;
    adc_cali_handle_t cali_handle;
    bool is_initialized;
} ntc_sensor_t;

// Funciones
void ntc_init(ntc_sensor_t *sensor);
float ntc_read_voltage(ntc_sensor_t *sensor);
float ntc_read_temperature(ntc_sensor_t *sensor);

#endif // LIBRARY_NTC_H
