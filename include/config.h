/* config.h
 * Default configuration values for OTA WiFi Manager
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "defaults.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DEFAULT_WIFI_SSID "OTA_WiFiManager"
#define DEFAULT_WIFI_PASSWORD "password1234"
#define DEFAULT_IMAGE_MODE_RANDOM 1
#define DEFAULT_IMAGE_CYCLE_INTERVAL 5000 // milliseconds

extern char wifi_ssid[33];
extern char wifi_password[65];
extern int image_mode_random;
extern int image_cycle_interval;

void config_load();
void config_save();
void config_reset();
void config_mark_changed();
void config_update();

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H
