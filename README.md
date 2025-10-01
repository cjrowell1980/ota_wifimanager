## System Reset Behavior
When a system reset is performed via the menu:
- WiFi is set to Access Point mode with default SSID `OTA_WiFiManager` and password `password1234` (these can be confirmed/changed in the menu)
- Image settings are reset to cycle randomly through all SD card images every 5 seconds
# ota_wifimanager

ESP32-based project for WiFi management and OTA (Over-The-Air) updates.

## Structure
- `src/` - Source code (C/C++ for ESP32)
- `include/` - Header files
- `config/` - Configuration files
- `docs/` - Documentation
- `tests/` - Test code


## Getting Started
1. Open `ota_wifimanager.ino` in the Arduino IDE.
2. Install the following libraries via Library Manager or PlatformIO:
	- WiFiManager
	- ArduinoOTA
	- TFT_eSPI (or your preferred TFT library)
	- TouchScreen (or your preferred touchscreen library)
	- SD
3. Select your ESP32 board and COM port.
4. Build and upload the sketch to your device.


## Features
- WiFi connection management (pre-built library)
- OTA firmware updates (pre-built library)
- Integrated SD card support
- TFT touchscreen integration (pre-built libraries)
- Touchscreen menu system with hierarchical navigation
	- Select images to display
	- Set duration between image changes
	- Choose image order (sequential/random)
	- Manage WiFi settings (AP/client, SSID, password)
	- Pull updates from GitHub repo
	- System reset option
- Randomly rotates and displays images from SD card on TFT screen


## Libraries Used
- WiFi Manager: pre-built library
- OTA Manager: pre-built library
- TFT Display: pre-built library
- Touchscreen: pre-built library

## License
MIT
