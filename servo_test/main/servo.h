#ifndef SERVO_H
#define SERVO_H

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_PIN GPIO_NUM_18 // Pin donde está conectado el servo
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // Resolución de 13 bits (0-8191)
#define LEDC_FREQUENCY 50  // Frecuencia de 50 Hz para servomotores

typedef enum {
    CLOSE = 0,
    OPEN = 180
} servo_state_t;

/**
 * @brief Inicializa el servo en el pin configurado
 */
void servo_init(void);

/**
 * @brief Establece el estado del servo en CLOSE (0°) o OPEN (180°)
 *
 * @param state Estado deseado del servo
 */
void servo_set_state(servo_state_t state);

#endif // SERVO_H
