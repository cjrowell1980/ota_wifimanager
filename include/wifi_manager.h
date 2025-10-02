// wifi_manager.h
#include <stdbool.h>
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

void wifi_manager_init(void);
bool wifi_manager_is_connected(void);
void wifi_manager_reconnect(void);
void wifi_manager_disconnect(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_H
