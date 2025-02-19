#ifndef LIBRARY_NTC_H
#define LIBRARY_NTC_H

#include <stdio.h>
#include <math.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "library_RGB.h"
#include "task_library.h"
#include "driver/adc.h"
#include "freertos/timers.h"

// Declarar extern para acceder al `adc_handle` definido en task_library.c
extern adc_oneshot_unit_handle_t adc_handle;
extern TimerHandle_t ntc_timer;

// Definiciones del ADC
#define ADC_CHANNEL     ADC_CHANNEL_7
#define ADC_ATTEN       ADC_ATTEN_DB_12
#define ADC_UNIT        ADC_UNIT_1

// Parámetros del NTC
#define REFERENCE_VOLTAGE 3.3
#define REFERENCE_RESISTOR 10000
#define BETA 3950
#define NOMINAL_TEMPERATURE 21
#define NOMINAL_RESISTANCE 10000

// **Nueva frecuencia de muestreo en milisegundos**
#define NTC_SAMPLING_PERIOD_MS 3000 

// Funciones
void init_NTC(void);
float read_NTC_voltage(void);
float read_NTC_temperature(void);
float get_last_NTC_temperature(void);
void ntc_read_task(void *pvParameters); // Nueva función para lectura en tarea separada

#endif // LIBRARY_NTC_H
