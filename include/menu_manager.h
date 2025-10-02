/* menu_manager.h
 * Handles touchscreen menu navigation and configuration for OTA WiFi Manager.
 */
#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H


void menu_manager_init(void);
void menu_manager_loop(void);
void menu_manager_system_reset(void);
void menu_manager_handle_touch(int x, int y);

#endif // MENU_MANAGER_H
