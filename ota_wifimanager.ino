
/* ota_wifimanager.ino
 * Main Arduino sketch for ESP32 OTA WiFi Manager with SD card and TFT touchscreen support.
 *
 * Required Arduino Libraries:
 *   - WiFiManager (by tzapu)
 *   - ArduinoOTA (by jandrassy)
 *   - TFT_eSPI (by Bodmer)
 *   - XPT2046_Touchscreen (by PaulStoffregen)
 *   - SD (built-in for ESP32/Arduino)
 *   - JPEGDecoder (by Bodmer)
 *
 * Install all libraries via Arduino Library Manager or PlatformIO before building.
 */
#include <WiFiManager.h>          // WiFiManager library (tzapu)
#include <ArduinoOTA.h>           // ArduinoOTA library (jandrassy)
#include <TFT_eSPI.h>             // TFT_eSPI library (Bodmer)
#include <XPT2046_Touchscreen.h> // XPT2046 touchscreen library (PaulStoffregen)
#include <SD.h>
#include <TJpg_Decoder.h> // For JPG image decoding (Bodmer)

// Pin configuration for XPT2046 (update as needed for your hardware)
#define CS_PIN  8   // Chip select pin for touchscreen
#define TIRQ_PIN  2 // Touch IRQ pin (optional, set to -1 if unused)
TFT_eSPI tft = TFT_eSPI(); // Uses Bodmer/TFT_eSPI
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);

#include "include/menu_manager.h"
#include "include/tft_manager.h"

// Timing for image cycling in normal mode
unsigned long lastImageChange = 0;
const unsigned long imageInterval = 5000; // 5 seconds between images

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("OTA_WiFiManager"); // Uses tzapu/WiFiManager
  ArduinoOTA.begin(); // Uses jandrassy/ArduinoOTA
  SD.begin();
  ts.begin(); // Initialize XPT2046 touchscreen
  ts.setRotation(1); // Set rotation as needed (0-3)
  tft.begin();
  tft.setRotation(1); // Set as needed for your display
  tft.fillScreen(TFT_BLACK);
  menu_manager_init();
  tft_manager_init();
  tft_display_random_image_from_sd();
  lastImageChange = millis(); // Initialize timing for image cycling
}


void loop() {
  ArduinoOTA.handle(); // Uses jandrassy/ArduinoOTA
  handleTouchEvent();
  menu_manager_loop();
  
  // Handle automatic image cycling in normal mode
  if (!menu_manager_is_in_menu_mode()) {
    unsigned long currentTime = millis();
    if (currentTime - lastImageChange >= imageInterval) {
      tft_display_random_image_from_sd();
      lastImageChange = currentTime;
    }
  }
}

void handleTouchEvent() {
  // Handle touchscreen events using XPT2046_Touchscreen
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    // Map raw touch coordinates to screen coordinates as needed
    // Example for 320x240 display:
    int x = p.x;
    int y = p.y;
    // TODO: Calibrate and map x/y if needed
    
    // Pass touch event to menu manager for navigation
    menu_manager_handle_touch(x, y);
  }
}
