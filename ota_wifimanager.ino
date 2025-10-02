
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
#include <WiFi.h>                 // ESP32 WiFi library
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
#include "include/config.h"

// Timing for image cycling in normal mode
unsigned long lastImageChange = 0;
const unsigned long imageInterval = 5000; // 5 seconds between images

// WiFi status tracking
bool wifiConnected = false;
bool otaInitialized = false;
unsigned long lastWiFiCheck = 0;
const unsigned long wifiCheckInterval = 10000; // Check WiFi every 10 seconds

void initWiFi() {
  Serial.println("Starting WiFi initialization...");
  
  // Try to connect to saved WiFi credentials first
  WiFi.mode(WIFI_STA);
  WiFi.begin(); // Use saved credentials from ESP32 memory
  
  Serial.println("Attempting WiFi connection with saved credentials...");
  
  // Wait up to 5 seconds for connection with saved credentials
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
    yield(); // Feed watchdog
  }
  
  // If saved credentials don't work, try config credentials
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nTrying configured credentials...");
    Serial.print("SSID: ");
    Serial.println(wifi_ssid);
    
    WiFi.begin(wifi_ssid, wifi_password);
    
    attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(500);
      Serial.print(".");
      attempts++;
      yield(); // Feed watchdog
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize OTA when WiFi is connected
    Serial.println("Initializing OTA...");
    ArduinoOTA.begin();
    otaInitialized = true;
    Serial.println("OTA initialized");
  } else {
    Serial.println("\nWiFi connection failed - continuing without WiFi");
    Serial.printf("Tried SSID: %s\n", wifi_ssid);
    wifiConnected = false;
  }
  
  Serial.println("WiFi initialization complete");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== OTA WiFi Manager Starting ===");
  
  // Load configuration first
  Serial.println("Loading configuration...");
  config_load();
  Serial.println("Configuration loaded");
  
  // Initialize hardware step by step with debugging
  Serial.println("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    // Continue without SD card
  } else {
    Serial.println("SD card initialized successfully");
  }
  
  Serial.println("Initializing TFT display...");
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  Serial.println("TFT display initialized");
  
  Serial.println("Initializing touchscreen...");
  ts.begin();
  ts.setRotation(1);
  Serial.println("Touchscreen initialized");
  
  // Initialize managers
  Serial.println("Initializing menu manager...");
  menu_manager_init();
  Serial.println("Menu manager initialized");
  
  Serial.println("Initializing TFT manager...");
  tft_manager_init();
  Serial.println("TFT manager initialized");
  
  // Show startup message
  Serial.println("Displaying startup message...");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("OTA WiFi Manager");
  tft.setCursor(80, 130);
  tft.print("Starting...");
  Serial.println("Startup message displayed");
  delay(3000);
  
  // Try to connect to WiFi non-blocking
  Serial.println("Initializing WiFi...");
  initWiFi();
  Serial.println("WiFi initialization complete");
  
  // Start displaying images regardless of WiFi status
  Serial.println("Displaying first image...");
  tft_display_random_image_from_sd();
  
  // If image display failed, show a simple fallback
  delay(1000);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 220);
  tft.print("Touch screen for menu");
  
  lastImageChange = millis();
  Serial.println("Setup complete!");
}


void loop() {
  // Feed the watchdog to prevent resets
  yield();
  
  // Handle configuration updates
  config_update();
  
  // Handle OTA updates only when WiFi is connected
  if (wifiConnected && otaInitialized) {
    ArduinoOTA.handle();
  }
  
  // Check WiFi status periodically
  unsigned long currentTime = millis();
  if (currentTime - lastWiFiCheck >= wifiCheckInterval) {
    checkWiFiStatus();
    lastWiFiCheck = currentTime;
  }
  
  handleTouchEvent();
  menu_manager_loop();
  
  // Handle automatic image cycling in normal mode
  if (!menu_manager_is_in_menu_mode()) {
    if (currentTime - lastImageChange >= imageInterval) {
      Serial.println("Time to cycle image...");
      tft_display_random_image_from_sd();
      lastImageChange = currentTime;
    }
  }
  
  // Small delay to prevent tight loop
  delay(10);
}

void checkWiFiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      // WiFi just connected
      wifiConnected = true;
      if (!otaInitialized) {
        ArduinoOTA.begin();
        otaInitialized = true;
      }
      Serial.println("WiFi reconnected");
    }
  } else {
    if (wifiConnected) {
      // WiFi just disconnected
      wifiConnected = false;
      Serial.println("WiFi disconnected");
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
