// sd_manager.c
#include "sd_manager.h"

#include <SD.h>


static bool sd_initialized = false;

void sd_manager_init(void) {
    printf("SD Card Manager Initialized\n");
    if (SD.begin()) {
        printf("SD card initialized successfully.\n");
        sd_initialized = true;
    } else {
        printf("SD card initialization failed!\n");
        sd_initialized = false;
    }
}

bool sd_manager_is_ready(void) {
    return sd_initialized;
}

void sd_manager_list_files(void) {
    if (!sd_initialized) {
        printf("SD card not initialized.\n");
        return;
    }
    File root = SD.open("/");
    if (!root) {
        printf("Failed to open SD root directory.\n");
        return;
    }
    File file = root.openNextFile();
    while (file) {
        printf("%s\n", file.name());
        file = root.openNextFile();
    }
}
