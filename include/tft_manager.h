// tft_manager.h
#ifndef TFT_MANAGER_H
#define TFT_MANAGER_H

#include <TFT_eSPI.h>

// TFT object needs C++ linkage since it's a C++ class
extern TFT_eSPI tft;

#ifdef __cplusplus
extern "C" {
#endif

void tft_manager_init(void);
void tft_touchscreen_init(void);
void tft_handle_touch_event(void);
void tft_display_random_image_from_sd(void);

#ifdef __cplusplus
}
#endif

#endif // TFT_MANAGER_H
