// wifi_manager.h
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

void wifi_manager_init(void);
bool wifi_manager_is_connected(void);
void wifi_manager_reconnect(void);
void wifi_manager_disconnect(void);

#endif // WIFI_MANAGER_H
