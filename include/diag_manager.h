// diag_manager.h
// Diagnostics page and functions for OTA WiFi Manager
#ifndef DIAG_MANAGER_H
#define DIAG_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

void diag_manager_init(void);
void diag_manager_show_page(void);
void diag_manager_update(void);

#ifdef __cplusplus
}
#endif

#endif // DIAG_MANAGER_H
