// ota_manager.c
#include "ota_manager.h"

#include <ArduinoOTA.h>
#include <WiFi.h>

void ota_manager_init(void) {
    printf("OTA Manager Initialized\n");
    ArduinoOTA.onStart([]() {
        printf("OTA Update Start\n");
    });
    ArduinoOTA.onEnd([]() {
        printf("OTA Update End\n");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        printf("OTA Progress: %u%%\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) printf("Auth Failed\n");
        else if (error == OTA_BEGIN_ERROR) printf("Begin Failed\n");
        else if (error == OTA_CONNECT_ERROR) printf("Connect Failed\n");
        else if (error == OTA_RECEIVE_ERROR) printf("Receive Failed\n");
        else if (error == OTA_END_ERROR) printf("End Failed\n");
    });
    ArduinoOTA.begin();
}

void ota_manager_handle(void) {
    ArduinoOTA.handle();
}
