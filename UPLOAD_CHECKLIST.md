# OTA WiFi Manager: Upload Checklist

Use this checklist to ensure your OTA WiFi Manager firmware is ready to upload to your ESP32 device.

## 1. Library Installation
- [ ] WiFiManager (by tzapu) installed
- [ ] ArduinoOTA (by jandrassy) installed
- [ ] TFT_eSPI (by Bodmer) installed and configured for your display
- [ ] XPT2046_Touchscreen (by PaulStoffregen) installed
- [ ] SD library available (built-in for ESP32)
- [ ] JPEGDecoder (by Bodmer) installed

## 2. Hardware Connections
- [ ] ESP32 board connected via USB
- [ ] TFT display wired and powered
- [ ] XPT2046 touchscreen wired (CS and TIRQ pins set in code)
- [ ] SD card inserted with JPG images in root directory

## 3. Configuration
- [ ] `TFT_eSPI/User_Setup.h` matches your display wiring
- [ ] `CS_PIN` and `TIRQ_PIN` in `ota_wifimanager.ino` match your touchscreen wiring
- [ ] WiFi credentials and defaults set as needed in `config.h`/`defaults.h`

## 4. Build & Upload
- [ ] Open `ota_wifimanager.ino` in Arduino IDE
- [ ] Select correct ESP32 board and COM port
- [ ] Compile the sketch (no errors)
- [ ] Upload to device

## 5. First Boot
- [ ] Device starts in AP mode (SSID: OTA_WiFiManager, password: password1234)
- [ ] TFT displays a random JPG from SD card
- [ ] Touchscreen responds to input
- [ ] Menu navigation works
- [ ] OTA updates available via ArduinoOTA

---
If any step fails, check wiring, library versions, and serial output for errors.
