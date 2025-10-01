// ota_wifimanager.c
// Main entry point for ESP32 WiFi and OTA management

#include <stdio.h>

#include "wifi_manager.h"
#include "ota_manager.h"
#include "sd_manager.h"
#include "tft_manager.h"

void app_main(void) {
    printf("Starting OTA WiFi Manager...\n");
    wifi_manager_init();
    ota_manager_init();
    sd_manager_init();
    tft_manager_init();
    tft_display_random_image_from_sd();
    tft_touchscreen_init();
    while (1) {
        tft_handle_touch_event();
        // Add delay or event-driven logic as needed
    }
    // Main loop or event handler here
}
