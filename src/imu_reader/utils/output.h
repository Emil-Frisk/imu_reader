#ifndef OUTPUT_H
#define OUTPUT_H
#include "imu_reader.h"
void print_output_data (float sensors_data[][7]);
extern imu_reader_settings_t imu_reader_settings;
#endif