

# OTA WiFi Manager — AI Coding Agent Instructions

## Project Overview
This project is an ESP32-based WiFi and OTA (Over-The-Air) update manager with SD card and TFT touchscreen support. It is structured for the Arduino ecosystem, using pre-built libraries for WiFi, OTA, TFT, touchscreen, and SD card access.

## Architecture & Key Components
- **Entry Point:** `ota_wifimanager.ino` (Arduino sketch) — initializes WiFi, OTA, SD, TFT, and menu system.
- **Modular Managers:**
	- `wifi_manager.[ch]`: WiFi connection logic (AP/client mode)
	- `ota_manager.[ch]`: OTA update logic
	- `sd_manager.[ch]`: SD card access and image file management
	- `tft_manager.[ch]`: TFT display and touchscreen event handling
	- `menu_manager.[ch]`: Touchscreen menu navigation, settings, and system reset
	- `config.[ch]` & `defaults.h`: Persistent settings (WiFi, image mode, intervals) via EEPROM
	- `lang.h`: UI language strings for localization
	- `diag_manager.[ch]`: Diagnostics/status page for device health

## Developer Workflows
- **Build/Upload:**
	1. Open `ota_wifimanager.ino` in Arduino IDE
	2. Install required libraries: WiFiManager, ArduinoOTA, TFT_eSPI, TouchScreen, SD
	3. Select ESP32 board and COM port
	4. Build and upload
- **Configuration:**
	- Default WiFi AP: SSID `OTA_WiFiManager`, password `password1234` (see `config.h`/`defaults.h`)
	- System reset restores defaults and random image cycling (see `menu_manager_system_reset`)
- **Menu/UI:**
	- Menu navigation and settings are handled in `menu_manager.c` (see `menu_manager_loop`)
	- UI text is defined in `lang.h` for easy localization
- **Persistent Storage:**
	- Settings are saved/loaded via EEPROM (`config.c`). Use `config_mark_changed()` after edits and call `config_update()` in the main loop.
- **Diagnostics:**
	- Device health/status is shown via `diag_manager_show_page()`

## Project-Specific Patterns
- **All hardware abstraction is modularized in `*_manager.[ch]` pairs.**
- **Touchscreen and TFT logic is stubbed for integration with your preferred libraries.**
- **Menu system is hierarchical and extensible; add new pages/options in `menu_manager.c`.**
- **Critical errors and LED codes are documented in menu/status pages.**
- **Localization:** All UI strings are in `lang.h` for multi-language support.

## Example: Adding a New Menu Option
1. Add a function in `menu_manager.c` to draw the new page.
2. Update the menu navigation logic to include the new option.
3. Add any new UI strings to `lang.h`.

## Key Files/Directories
- `ota_wifimanager.ino` — Arduino entry point
- `src/` — C source files for all managers
- `include/` — Header files for all managers and config
- `config/` — Default and persistent configuration
- `lang.h` — UI language strings

## Do Not:
- Do not hardcode UI text; use `lang.h`.
- Do not bypass manager modules for hardware access; use the provided abstraction.

---
If any section is unclear or incomplete, please request clarification or examples from the user.
