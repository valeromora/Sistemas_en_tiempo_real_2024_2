#ifndef TASK_LIBRARY_H
#define TASK_LIBRARY_H
#include "library_RGB.h"
#include "library_NTC.h"
#include "library_NTC.h"



#define RED_PIN       21  
#define GREEN_PIN     22  
#define BLUE_PIN      23 
#define BUTTON_PIN    0




 typedef enum{
    RESET,
    RED,
    GREEN,
    BLUE
 }led_state_t;


void init_button_interrupt(void);
void init_system(void);
void task_manager(void);



#endif
