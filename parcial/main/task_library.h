#ifndef TASK_LIBRARY_H
#define TASK_LIBRARY_H
#include "library_RGB.h"
#include "library_NTC.h"





// Umbrales de temperatura para cada LED
extern float red_threshold_min;
extern float green_threshold_min;
extern float green_threshold_max;
extern float blue_threshold_max;


#define RED_PIN       21  
#define GREEN_PIN     22  
#define BLUE_PIN      23 
#define BUTTON_PIN    0


#define RED_PIN_2       21  
#define GREEN_PIN_2     22  
#define BLUE_PIN_2      23 


#define UART_NUM        UART_NUM_0  // Usa UART1 (Puedes cambiarlo)
#define TX_PIN          17
#define RX_PIN          16
#define UART_BUF_SIZE   2048


 typedef enum{
    RESET,
    RED,
    GREEN,
    BLUE
 }led_state_t;

void led_control_task(void *pvParameters);

void init_system(void);
void init_UART(void);
void uart_task(void *pvParameters);  
void led_control_task(void *pvParameters);


#endif
