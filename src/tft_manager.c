// tft_manager.c
#include "tft_manager.h"
#include <stdio.h>
#include <stdlib.h>

#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <SD.h>
#include <stdlib.h>


static TFT_eSPI tft = TFT_eSPI();

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


// TJpg_Decoder callback to push pixels to TFT
static bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    tft.pushImage(x, y, w, h, bitmap);
    return true;
}

void tft_display_random_image_from_sd(void) {
    printf("Displaying random image from SD card...\n");
    File root = SD.open("/");
    if (!root) return;
    char *images[32];
    int count = 0;
    File file = root.openNextFile();
    while (file && count < 32) {
        String name = file.name();
        name.toLowerCase();
        if (!file.isDirectory() && (name.endsWith(".jpg") || name.endsWith(".jpeg"))) {
            images[count] = strdup(file.name());
            count++;
        }
        file = root.openNextFile();
    }
    if (count == 0) return;
    int idx = rand() % count;
    tft.fillScreen(TFT_BLACK);
    TJpg_Decoder.setJpgScale(1);
    TJpg_Decoder.setCallback(tft_output);
    if (!TJpg_Decoder.drawSdJpg(0, 0, images[idx])) {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(10, 10);
        tft.print("JPG error: ");
        tft.println(images[idx]);
    }
    for (int i = 0; i < count; ++i) free(images[i]);
}
