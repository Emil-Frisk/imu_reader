#ifndef SETTINGS_H
#define SETTINGS_H
#include "imu_reader.h"
#define SETTINGS_BUF_LEN (256)
void wait_for_settings();
void extract_part(const char* part, const char* buf, settings_enum setting);
void excract_settings(const char* buf);
#endif //SETTINGS_H