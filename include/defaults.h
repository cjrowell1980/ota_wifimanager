// defaults.h
// Default settings for OTA WiFi Manager
// Edit this file to adjust default configuration values

#ifndef DEFAULTS_H
#define DEFAULTS_H

#define DEFAULT_WIFI_SSID           "OTA_WiFiManager"
#define DEFAULT_WIFI_PASSWORD       "password1234"
#define DEFAULT_WIFI_MODE_AP        1   // 1 = AP, 0 = Client
#define DEFAULT_IMAGE_MODE_RANDOM   1   // 1 = Random, 0 = Sequential, 2 = Fixed
#define DEFAULT_IMAGE_CYCLE_INTERVAL 5000 // milliseconds
#define DEFAULT_IMAGE_FIXED_INDEX   0   // Index of default image in Fixed mode
#define DEFAULT_OTA_URL             "https://github.com/yourrepo/firmware.bin"
#define DEFAULT_SD_MIN_FREE_PERCENT 20  // Minimum SD free space percent before warning
#define DEFAULT_LANGUAGE            "en"

#endif // DEFAULTS_H
