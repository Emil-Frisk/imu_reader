#include "output.h"

void print_output_data (float sensors_data[][7]) {
    for (int i = 0; i < imu_reader_settings.sensorCount; i++) {
        printf("w%d: %.4f, x%d: %.4f, y%d: %.4f, z%d: %.4f, gx%d: %.4f, gy%d: %.4f, gz%d: %.4f | ",i, sensors_data[i][0],i, sensors_data[i][1],i, sensors_data[i][2],i, sensors_data[i][3],i, sensors_data[i][4],i, sensors_data[i][5],i, sensors_data[i][6] );
    }
    printf("\n");
}