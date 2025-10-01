// User_Setup_Custom.h for TFT_eSPI (Bodmer)
// Place this file in your project directory and reference it in platformio.ini or Arduino IDE as needed.
// Edit pin numbers and options to match your hardware.

#define ILI9341_DRIVER      // Example: ILI9341 display

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   2   // Data Command control pin
#define TFT_RST  4   // Reset pin (could connect to RST, or set to -1 if not used)

#define TOUCH_CS 8   // XPT2046 chip select
#define TOUCH_IRQ 2  // XPT2046 IRQ pin (optional)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SPI_FREQUENCY  40000000
#define SPI_TOUCH_FREQUENCY  2500000

// Comment out the next line to disable support for smooth fonts
#define SMOOTH_FONT
