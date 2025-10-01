
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
#include <JPEGDecoder.h> // For JPG image decoding

// Pin configuration for XPT2046 (update as needed for your hardware)
#define CS_PIN  8   // Chip select pin for touchscreen
#define TIRQ_PIN  2 // Touch IRQ pin (optional, set to -1 if unused)
TFT_eSPI tft = TFT_eSPI(); // Uses Bodmer/TFT_eSPI
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);

#include "menu_manager.h"

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
  displayRandomImageFromSD();
}


void loop() {
  ArduinoOTA.handle(); // Uses jandrassy/ArduinoOTA
  handleTouchEvent();
  menu_manager_loop();
  // Add other logic as needed
}

void displayRandomImageFromSD() {
  // List JPG image files on SD, pick one at random, and display on TFT
  File root = SD.open("/");
  if (!root) return;
  std::vector<String> images;
  File file = root.openNextFile();
  while (file) {
    String name = file.name();
    name.toLowerCase();
    if (!file.isDirectory() && (name.endsWith(".jpg") || name.endsWith(".jpeg"))) {
      images.push_back(file.name());
    }
    file = root.openNextFile();
  }
  if (images.empty()) return;
  int idx = random(images.size());
  String imgName = images[idx];
  tft.fillScreen(TFT_BLACK);
  // Use JPEGDecoder to draw the image
  if (JpegDec.decodeSdFile(imgName.c_str())) {
    uint16_t *pImg;
    int mcu_w = JpegDec.MCUWidth;
    int mcu_h = JpegDec.MCUHeight;
    int max_x = JpegDec.width;
    int max_y = JpegDec.height;
    int x = 0, y = 0;
    while (y < max_y) {
      while (x < max_x) {
        if (!JpegDec.read()) break;
        pImg = JpegDec.pImage;
        tft.pushImage(x, y, JpegDec.MCUWidth, JpegDec.MCUHeight, pImg);
        x += mcu_w;
      }
      x = 0;
      y += mcu_h;
    }
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("JPG error: ");
    tft.println(imgName);
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
  // Pass touch event to menu_manager for navigation/settings
  menu_manager_handle_touch(x, y);
// Stub for menu_manager_handle_touch (implement in menu_manager.c for real logic)
void menu_manager_handle_touch(int x, int y) {
  // Example: print coordinates, real logic should handle menu navigation
  Serial.print("Touch at: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);
}
  }
}
  // Handle touchscreen events using XPT2046_Touchscreen
  // Example: if (ts.touched()) { ... }
  // Handle touchscreen events
  // Placeholder: implement using your touchscreen library
}
