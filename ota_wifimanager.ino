
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
unsigned long lastDisplayUpdate = 0;
const unsigned long imageInterval = 5000; // 5 seconds between images
const unsigned long displayUpdateInterval = 1000; // Update display every second

// WiFi status tracking
bool wifiConnected = false;
bool otaInitialized = false;
bool touchEnabled = false;  // Track if touchscreen is working
unsigned long lastWiFiCheck = 0;
const unsigned long wifiCheckInterval = 10000; // Check WiFi every 10 seconds

void initWiFi() {
  Serial.println("Starting WiFi initialization...");
  
  // Try to connect to saved WiFi credentials first
  WiFi.mode(WIFI_STA);
  WiFi.begin(); // Use saved credentials from ESP32 memory
  
  Serial.println("Attempting WiFi connection with saved credentials...");
  
  // Wait up to 3 seconds for connection with saved credentials
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 6) {
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
    while (WiFi.status() != WL_CONNECTED && attempts < 6) {
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
  yield();
  
  // Initialize hardware step by step with debugging
  Serial.println("Initializing SD card (CS=5)...");
  if (!SD.begin(5)) {
    Serial.println("SD card initialization failed on CS=5!");
    Serial.println("Check wiring, card format (FAT32), and CS pin.");
  } else {
    Serial.println("SD card initialized successfully on CS=5!");
  }
  yield();
  
  Serial.println("Initializing TFT display...");
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  Serial.println("TFT display initialized");
  yield();
  
  Serial.println("Initializing touchscreen...");
  // Temporarily disable touchscreen to isolate the crash
  Serial.println("Touchscreen initialization disabled for debugging");
  touchEnabled = false;
  
  // Add delay and yield to prevent watchdog reset
  delay(100);
  yield();
  
  // Initialize managers
  Serial.println("Initializing menu manager...");
  menu_manager_init();
  Serial.println("Menu manager initialized");
  yield();
  
  Serial.println("Initializing TFT manager...");
  tft_manager_init();
  Serial.println("TFT manager initialized");
  yield();
  
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
  yield();
  
  // Try to connect to WiFi non-blocking
  Serial.println("Initializing WiFi...");
  initWiFi();
  Serial.println("WiFi initialization complete");
  yield();
  
  // Start displaying images regardless of WiFi status
  Serial.println("Displaying test pattern instead of images...");
  
  // Show a simple test pattern instead of trying to load images
  Serial.println("Clearing screen...");
  tft.fillScreen(TFT_BLACK);
  Serial.println("Screen cleared");
  
  Serial.println("Drawing main text...");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(60, 80);
  tft.print("TFT WORKS!");
  Serial.println("Main text displayed");
  
  Serial.println("Drawing title...");
  tft.setTextSize(2);
  tft.setCursor(20, 120);
  tft.print("OTA WiFi Manager");
  Serial.println("Title displayed");
  
  Serial.println("Drawing status...");
  tft.setTextSize(1);
  tft.setCursor(10, 150);
  tft.print("SD Card: Failed");
  tft.setCursor(10, 170);
  tft.print("Touch: Disabled");
  Serial.println("Status displayed");
  
  // Draw some colored rectangles to test display
  Serial.println("Drawing color rectangles...");
  tft.fillRect(10, 190, 50, 20, TFT_RED);
  tft.fillRect(70, 190, 50, 20, TFT_GREEN);
  tft.fillRect(130, 190, 50, 20, TFT_BLUE);
  tft.fillRect(190, 190, 50, 20, TFT_YELLOW);
  tft.fillRect(250, 190, 50, 20, TFT_MAGENTA);
  Serial.println("Color rectangles displayed");
  
  Serial.println("Test pattern displayed");
  
  // If image display failed, show a simple fallback
  delay(1000);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 220);
  if (touchEnabled) {
    tft.print("Touch screen for menu");
  } else {
    tft.print("Basic display test - no touch");
  }
  
  lastImageChange = millis();
  lastDisplayUpdate = millis();
  Serial.println("Setup complete!");
  yield();
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
  
  // Handle automatic image cycling in normal mode (disabled for now)
  if (!menu_manager_is_in_menu_mode()) {
    // Instead of cycling images, update a simple counter on screen
    if (currentTime - lastDisplayUpdate >= displayUpdateInterval) {
      static int counter = 0;
      counter++;
      
      // Update counter display to show the device is alive
      tft.fillRect(250, 10, 60, 20, TFT_BLACK);
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setTextSize(1);
      tft.setCursor(250, 10);
      tft.print("Uptime:");
      tft.setCursor(250, 20);
      tft.print(counter);
      tft.print("s");
      
      lastDisplayUpdate = currentTime;
    }
    
    if (currentTime - lastImageChange >= imageInterval) {
      Serial.println("Image cycling disabled - SD card not working");
      // Don't try to display images since SD card failed
      // tft_display_random_image_from_sd();
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
  // Only handle touch events if touchscreen is enabled and working
  if (!touchEnabled) {
    return;
  }
  
  // Handle touchscreen events using XPT2046_Touchscreen
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    // Map raw touch coordinates to screen coordinates as needed
    // Example for 320x240 display:
    int x = p.x;
    int y = p.y;
    // TODO: Calibrate and map x/y if needed
    
    Serial.print("Touch detected at: ");
    Serial.print(x);
    Serial.print(", ");
    Serial.println(y);
    
    // Pass touch event to menu manager for navigation
    menu_manager_handle_touch(x, y);
  }
}
