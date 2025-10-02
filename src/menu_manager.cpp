/* menu_manager.c
 * Handles touchscreen menu navigation and configuration for OTA WiFi Manager.
 * This is a stub for menu logic. Integrate with TFT and touchscreen libraries.
 */
#include "include/menu_manager.h"
#include "include/tft_manager.h"
#include "include/diag_manager.h"
#include "include/config.h"
#include <stdio.h>
#include <TFT_eSPI.h>

typedef enum {
    STATUS_PAGE_WIFI = 0,
    STATUS_PAGE_IMAGES,
    STATUS_PAGE_FIRMWARE,
    STATUS_PAGE_OTHER,
    STATUS_PAGE_CREATORS,
    STATUS_PAGE_LED_CODES,
    STATUS_PAGE_DIAGNOSTICS,
    STATUS_PAGE_COUNT
} StatusPage;

static StatusPage currentStatusPage = STATUS_PAGE_WIFI;
static bool inMenuMode = false;

void menu_manager_init(void) {
    printf("Menu Manager Initialized\n");
    inMenuMode = false;  // Start in normal mode
    // Initialize menu structure, load settings, etc.
}

void menu_manager_enter_menu_mode(void) {
    inMenuMode = true;
    printf("Entering menu mode\n");
    menu_manager_show_status_page();
}

void menu_manager_exit_menu_mode(void) {
    inMenuMode = false;
    printf("Exiting menu mode\n");
    // Return to displaying images
    tft_display_random_image_from_sd();
}

bool menu_manager_is_in_menu_mode(void) {
    return inMenuMode;
}

// Simple touch region detection for Back/Next navigation
void menu_manager_handle_touch(int x, int y) {
    if (!inMenuMode) {
        // In normal mode - any touch enters menu mode
        menu_manager_enter_menu_mode();
        return;
    }
    
    // In menu mode - handle navigation
    // Back button: left third (0-107, 200-240)
    // Next button: middle third (107-213, 200-240)  
    // Exit button: right third (213-320, 200-240)
    if (y >= 200 && y <= 240) {
        if (x >= 0 && x < 107) {
            // Back
            currentStatusPage = (StatusPage)((currentStatusPage - 1 + STATUS_PAGE_COUNT) % STATUS_PAGE_COUNT);
            menu_manager_show_status_page();
        } else if (x >= 107 && x < 213) {
            // Next
            currentStatusPage = (StatusPage)((currentStatusPage + 1) % STATUS_PAGE_COUNT);
            menu_manager_show_status_page();
        } else if (x >= 213 && x < 320) {
            // Exit
            menu_manager_exit_menu_mode();
        }
    }
}

void menu_manager_loop(void) {
    // Only show status pages when in menu mode
    if (inMenuMode) {
        // Menu is already displayed when entering menu mode
        // Touch handling will manage navigation
    }
    // In normal mode, do nothing - images are handled by main loop
}

void menu_manager_system_reset(void) {
    config_reset();
    printf("System reset: WiFi AP mode, SSID: %s, Password: %s\n", wifi_ssid, wifi_password);
    printf("System reset: Image mode random, interval: %d ms\n", image_cycle_interval);
}

void menu_manager_draw_back_next_buttons() {
    // Back button (left third)
    tft.fillRect(0, 200, 107, 40, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextSize(1);
    tft.setCursor(35, 215);
    tft.print("Back");
    
    // Next button (middle third)
    tft.fillRect(107, 200, 106, 40, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setCursor(142, 215);
    tft.print("Next");
    
    // Exit button (right third)
    tft.fillRect(213, 200, 107, 40, TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setCursor(248, 215);
    tft.print("Exit");
}

void menu_manager_show_status_ap() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("Wifi Mode: Access Point");
    tft.setCursor(10, 50);
    tft.print("SSID: ");
    tft.println(wifi_ssid); // e.g. "PhotoFrame_AP"
    tft.setCursor(10, 80);
    tft.print("PWD: ");
    tft.println(wifi_password); // e.g. "password1234"
    tft.setCursor(10, 110);
    tft.print("IP: 192.168.4.1"); // Default ESP32 AP IP
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_wifi() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 20);
    tft.println("WiFi Status");
    tft.setTextSize(1);
    tft.setCursor(40, 60);
    tft.print("Mode: Access Point\nSSID: ");
    tft.println(wifi_ssid);
    tft.print("PWD: ");
    tft.println(wifi_password);
    tft.print("IP: 192.168.4.1");
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_images() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 20);
    tft.println("Image Status");
    tft.setTextSize(1);
    tft.setCursor(40, 60);
    tft.print("Mode: ");
    tft.println(image_mode_random ? "Random" : "Sequential");
    tft.print("Interval: ");
    tft.print(image_cycle_interval / 1000);
    tft.println("s");
    tft.print("Images: ");
    tft.println("N/A"); // Replace with actual image count
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_firmware() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 20);
    tft.println("Firmware");
    tft.setTextSize(1);
    tft.setCursor(40, 60);
    tft.println("Version: v1.0.0");
    tft.println("Last Update: Success");
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_other() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 20);
    tft.println("Other Info");
    tft.setTextSize(1);
    tft.setCursor(40, 60);
    tft.println("Uptime: 00:12:34");
    tft.println("Storage: 1.2GB free");
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_creators() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 40);
    tft.println("Creators");
    tft.setTextSize(1);
    tft.setCursor(40, 90);
    tft.println("Sofa Bear");
    tft.setCursor(40, 120);
    tft.println("Ingenio");
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_led_codes() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.println("LED Status Codes");
    tft.setTextSize(1);
    tft.setCursor(20, 50);
    tft.println("Green: Normal\nBlue: AP Mode\nYellow: SD Not Ready\nCyan: OTA Update\nMagenta: User Action\nWhite: Booting\nOff: Standby");
    tft.setCursor(20, 130);
    tft.println("1xR: SD missing\n2xR: SD error\n3xR: SD <20% free\n1xY: OTA fail\n1xM: Touch fail\n2xM: TFT fail\n1xG: Filesystem error\n1xW: Firmware check fail");
    tft.setCursor(20, 210);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.println("Critical errors (chirp every 5s):");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("- SD missing or error\n- SD <20% free\n- OTA fail\n- Touch fail\n- TFT fail\n- Filesystem error\n- Firmware check fail");
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_diagnostics() {
    diag_manager_show_page();
    menu_manager_draw_back_next_buttons();
}

void menu_manager_show_status_page() {
    // Draw Next button only (right side)
    tft.fillRect(290, 100, 30, 40, TFT_DARKGREY); // Next (right)
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextSize(2);
    tft.setCursor(305, 115);
    tft.print(">\n");

    // Show current page
    switch (currentStatusPage) {
        case STATUS_PAGE_WIFI:
            menu_manager_show_status_wifi();
            break;
        case STATUS_PAGE_IMAGES:
            menu_manager_show_status_images();
            break;
        case STATUS_PAGE_FIRMWARE:
            menu_manager_show_status_firmware();
            break;
        case STATUS_PAGE_OTHER:
            menu_manager_show_status_other();
            break;
        case STATUS_PAGE_CREATORS:
            menu_manager_show_status_creators();
            break;
        case STATUS_PAGE_LED_CODES:
            menu_manager_show_status_led_codes();
            break;
        case STATUS_PAGE_DIAGNOSTICS:
            menu_manager_show_status_diagnostics();
            break;
        default:
            break;
    }
}

void menu_manager_status_next() {
    currentStatusPage = (StatusPage)((currentStatusPage + 1) % STATUS_PAGE_COUNT);
    menu_manager_show_status_page();
}

// Add menu options for Backup/Restore, API, Firmware Rollback, and Language
void menu_manager_show_settings_menu() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(40, 30);
    tft.println("Settings");
    tft.setTextSize(1);
    tft.setCursor(40, 70);
    tft.println("1. WiFi Setup");
    tft.setCursor(40, 100);
    tft.println("2. OTA Update");
    tft.setCursor(40, 130);
    tft.println("3. System Reset");
    tft.setCursor(40, 160);
    tft.println("4. Backup/Restore");
    tft.setCursor(40, 190);
    tft.println("5. API Access");
    tft.setCursor(40, 220);
    tft.println("6. Firmware Rollback");
    tft.setCursor(180, 220);
    tft.println("7. Language");
    menu_manager_draw_back_next_buttons();
}
// Implement navigation and stubs for these menu options as needed.
