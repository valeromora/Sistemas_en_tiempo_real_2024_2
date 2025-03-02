/**
 * Application entry point.
 */

 #include "nvs_flash.h"
 //#include "http_server.h"
 #include "wifi_app.h"
 #include "driver/gpio.h"
 #include "servo.h"
 
 
 #define BLINK_GPIO				2
 
 
 static void configure_led(void)
 {
     
     gpio_reset_pin(BLINK_GPIO);
     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
     
 }
 
 
 void app_main(void)
 {
     // Initialize NVS
     esp_err_t ret = nvs_flash_init();
     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
     {
         ESP_ERROR_CHECK(nvs_flash_erase());
         ret = nvs_flash_init();
     }
     ESP_ERROR_CHECK(ret);
 
     // Start Wifi
     servo_init();
     init_obtain_time();
     configure_led();
     wifi_app_start();
 }
 

// #include "servo.h"

// void app_main() {
//     servo_init();

//     while (1) {
//         servo_set_state(CLOSE);
//         vTaskDelay(pdMS_TO_TICKS(2000));

//         servo_set_state(OPEN);
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }

 