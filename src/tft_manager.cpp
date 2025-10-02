// tft_manager.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <SD.h>
#include <stdlib.h>

extern TFT_eSPI tft;
TJpg_Decoder tjpg;

// TJpg_Decoder callback to push pixels to TFT
static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

extern "C" {

void tft_manager_init(void) {
    printf("TFT Manager Initialized\n");
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}


void tft_touchscreen_init(void) {
    // Touchscreen logic handled elsewhere (see main sketch)
    printf("Touchscreen Initialized (handled in main sketch)\n");
}


void tft_handle_touch_event(void) {
    // Touch event logic handled in main sketch
}

void tft_display_random_image_from_sd(void) {
    Serial.println("Starting image display function...");
    
    File root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open root directory");
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(10, 100);
        tft.print("No SD Card");
        return;
    }
    
    Serial.println("Root directory opened successfully");
    char *images[32];
    int count = 0;
    File file = root.openNextFile();
    
    Serial.println("Scanning for image files...");
    while (file && count < 32) {
        String name = file.name();
        name.toLowerCase();
        if (!file.isDirectory() && (name.endsWith(".jpg") || name.endsWith(".jpeg"))) {
            images[count] = strdup(file.name());
            Serial.print("Found image: ");
            Serial.println(images[count]);
            count++;
        }
        file = root.openNextFile();
    }
    
    Serial.print("Found ");
    Serial.print(count);
    Serial.println(" image files");
    
    if (count == 0) {
        Serial.println("No image files found");
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(10, 100);
        tft.print("No Images Found");
        return;
    }
    
    int idx = rand() % count;
    Serial.print("Selected image index: ");
    Serial.print(idx);
    Serial.print(" (");
    Serial.print(images[idx]);
    Serial.println(")");
    
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("Setting up JPEG decoder...");
    tjpg.setJpgScale(1);
    tjpg.setCallback(tft_output);
    
    Serial.println("Attempting to draw JPEG...");
    if (!tjpg.drawSdJpg(0, 0, images[idx])) {
        Serial.println("JPEG decode failed!");
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(10, 10);
        tft.print("JPG error: ");
        tft.println(images[idx]);
    } else {
        Serial.println("JPEG displayed successfully");
    }
    
    // Clean up memory
    for (int i = 0; i < count; ++i) {
        if (images[i]) free(images[i]);
    }
    Serial.println("Image display function complete");
}

} // extern "C"
