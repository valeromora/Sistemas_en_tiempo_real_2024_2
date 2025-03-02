/*
 * wifi_app.h
 *
 *  Created on: Oct 17, 2021
 *      Author: kjagu
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"

// Callback typedef
typedef void (*wifi_connected_event_callback_t)(void);

// WiFi application settings
#define WIFI_AP_SSID				"ESP32_VM"			// AP name
#define WIFI_AP_PASSWORD			"vm123456789"		// AP password
#define WIFI_AP_CHANNEL				1					// AP channel
#define WIFI_AP_SSID_HIDDEN			0					// AP visibility
#define WIFI_AP_MAX_CONNECTIONS		5					// AP max clients
#define WIFI_AP_BEACON_INTERVAL		100					// AP beacon: 100 milliseconds recommended
#define WIFI_AP_IP					"192.168.0.1"		// AP default IP
#define WIFI_AP_GATEWAY				"192.168.0.1"		// AP default Gateway (should be the same as the IP)
#define WIFI_AP_NETMASK				"255.255.255.0"		// AP netmask
#define WIFI_AP_BANDWIDTH			WIFI_BW_HT20		// AP bandwidth 20 MHz (40 MHz is the other option)
#define WIFI_STA_POWER_SAVE			WIFI_PS_NONE		// Power save not used
#define MAX_SSID_LENGTH				32					// IEEE standard maximum
#define MAX_PASSWORD_LENGTH			64					// IEEE standard maximum
#define MAX_CONNECTION_RETRIES		5					// Retry number on disconnect


#define NUM_REGISTERS_AV 			10
// netif object for the Station and Access Point
extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;

/**
 * Message IDs for the WiFi application task
 * @note Expand this based on your application requirements.
 */
typedef enum wifi_app_message
{
	WIFI_APP_MSG_START_HTTP_SERVER = 0,
	WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
	WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
	WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT,
	WIFI_APP_MSG_LOAD_SAVED_CREDENTIALS,
	WIFI_APP_MSG_STA_DISCONNECTED,
	WIFI_APP_CONNECT_TO_STA,
} wifi_app_message_e;

/**
 * Structure for the message queue
 * @note Expand this based on application requirements e.g. add another type and parameter as required
 */
typedef struct wifi_app_queue_message
{
	wifi_app_message_e msgID;
} wifi_app_queue_message_t;


typedef struct register_saved
{
	uint8_t hour;
	uint8_t min;
	uint8_t monday;
	uint8_t tuesday;
	uint8_t wednesday;
	uint8_t thursday;
	uint8_t friday;
	uint8_t sunday;
	uint8_t saturday;

} register_saved_e;

static void obtain_time( void );
void connect_to_wifi(void);
void initialize_registers( void );
esp_err_t read_reg_data(char *str_to_save ,uint8_t register_num);
void init_obtain_time( void );
bool get_state_time_was_synchronized( void );

void save_reg_data(uint8_t register, char *str) ;
void save_wifi_credentials(const char *ssid, const char *password);
void load_wifi_credentials(char *ssid, char *password);
static void wifi_app_connect_sta(void);
void check_sta_connection_state( void );
void update_register(int reg_to_update);

/**
 * Sends a message to the queue
 * @param msgID message ID from the wifi_app_message_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE.
 * @note Expand the parameter list based on your requirements e.g. how you've expanded the wifi_app_queue_message_t.
 */
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/**
 * Starts the WiFi RTOS task
 */
void wifi_app_start(void);

/**
 * Gets the wifi configuration
 */
wifi_config_t* wifi_app_get_wifi_config(void);

/**
 * Sets the callback function.
 */
void wifi_app_set_callback(wifi_connected_event_callback_t cb);

/**
 * Calls the callback function.
 */
void wifi_app_call_callback(void);

/**
 * Gets the RSSI value of the Wifi connection.
 * @return current RSSI level.
 */
int8_t wifi_app_get_rssi(void);

#endif /* MAIN_WIFI_APP_H_ */




























