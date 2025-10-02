/* config.c
 * Handles persistent storage and retrieval of settings for OTA WiFi Manager
 */
#include "include/config.h"
#include <EEPROM.h>
#include <Arduino.h>
#include <string.h>

#define EEPROM_SIZE 128

char wifi_ssid[33] = DEFAULT_WIFI_SSID;
char wifi_password[65] = DEFAULT_WIFI_PASSWORD;
int image_mode_random = DEFAULT_IMAGE_MODE_RANDOM;
int image_cycle_interval = DEFAULT_IMAGE_CYCLE_INTERVAL;

static unsigned long lastEditTime = 0;
static bool savePending = false;

void config_load() {
    EEPROM.begin(EEPROM_SIZE);
    if (EEPROM.read(0) == 0xA5) {
        EEPROM.get(1, wifi_ssid);
        EEPROM.get(34, wifi_password);
        EEPROM.get(99, image_mode_random);
        EEPROM.get(103, image_cycle_interval);
    }
    EEPROM.end();
}

void config_save() {
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(0, 0xA5);
    EEPROM.put(1, wifi_ssid);
    EEPROM.put(34, wifi_password);
    EEPROM.put(99, image_mode_random);
    EEPROM.put(103, image_cycle_interval);
    EEPROM.commit();
    EEPROM.end();
}

void config_reset() {
    strcpy(wifi_ssid, DEFAULT_WIFI_SSID);
    strcpy(wifi_password, DEFAULT_WIFI_PASSWORD);
    image_mode_random = DEFAULT_IMAGE_MODE_RANDOM;
    image_cycle_interval = DEFAULT_IMAGE_CYCLE_INTERVAL;
    config_save();
}

void config_mark_changed() {
    lastEditTime = millis();
    savePending = true;
}

void config_update() {
    if (savePending && (millis() - lastEditTime > 2000)) {
        config_save();
        savePending = false;
    }
}
// Call config_mark_changed() whenever a setting is edited.
// Call config_update() regularly in your main loop to process pending saves.
