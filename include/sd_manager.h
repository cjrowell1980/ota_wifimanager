// sd_manager.h
#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

void sd_manager_init(void);
bool sd_manager_is_ready(void);
void sd_manager_list_files(void);

#ifdef __cplusplus
}
#endif

#endif // SD_MANAGER_H
