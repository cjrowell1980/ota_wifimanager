// wifi_manager.c
#include "wifi_manager.h"
#include <WiFiManager.h>
#include <WiFi.h>



static WiFiManager wifiManager;

void wifi_manager_init(void) {
    printf("WiFi Manager Initialized\n");
    wifiManager.autoConnect("OTA_WiFiManager");
    printf("WiFi connected. IP: %s\n", WiFi.localIP().toString().c_str());
}

bool wifi_manager_is_connected(void) {
    return WiFi.status() == WL_CONNECTED;
}

void wifi_manager_reconnect(void) {
    if (!wifi_manager_is_connected()) {
        printf("WiFi reconnecting...\n");
        WiFi.reconnect();
    }
}

void wifi_manager_disconnect(void) {
    printf("WiFi disconnecting...\n");
    WiFi.disconnect();
}
