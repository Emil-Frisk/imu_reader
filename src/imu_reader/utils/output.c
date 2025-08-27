#include "output.h"

void print_output_data (float sensors_data[][4]) {
    for (int i = 0; i < imu_reader_settings.sensorCount; i++) {
        printf("w%d: %.4f, x%d: %.4f, y%d: %.4f, z%d: %.4f | ",i, sensors_data[i][0],i, sensors_data[i][1],i, sensors_data[i][2],i, sensors_data[i][3] );
    }
    printf("\n");
}