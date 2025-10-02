/* menu_manager.h
#include <stdbool.h>
 * Handles touchscreen menu navigation and configuration for OTA WiFi Manager.
 */
#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

void menu_manager_init(void);
void menu_manager_loop(void);
void menu_manager_system_reset(void);
void menu_manager_handle_touch(int x, int y);
void menu_manager_show_status_page(void);
void menu_manager_enter_menu_mode(void);
void menu_manager_exit_menu_mode(void);
bool menu_manager_is_in_menu_mode(void);

#ifdef __cplusplus
}
#endif

#endif // MENU_MANAGER_H
