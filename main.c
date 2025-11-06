#include "i2c_helpers.h"
#include "InitFusion.h"
#include "Fusion.h"
#include "ism330dlc.h"
#include "tusb.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "settings.h"
#include "output.h"

// extern sh2_vector_list_t sh2_vector_list;

#define LOOP_DURATION_MS 6.252f

float apply_lpf(float new_val, float old_value) {
    return old_value*(1.0f-imu_reader_settings.lpf_alpha) + imu_reader_settings.lpf_alpha*new_val;
}

void update_loop_with_lpf(float sleep_time, float sensors_data[][7], Sensor* sensors) {
    while (true) {
    read_all_sensors(sensors);
    for (int i=0; i<imu_reader_settings.sensorCount;i++) {
        sensors[i].gyroscope_old.axis.x = apply_lpf(sensors[i].gyroscope.axis.x, sensors[i].gyroscope_old.axis.x);
        sensors[i].gyroscope_old.axis.y = apply_lpf(sensors[i].gyroscope.axis.y, sensors[i].gyroscope_old.axis.y);
        sensors[i].gyroscope_old.axis.z = apply_lpf(sensors[i].gyroscope.axis.z, sensors[i].gyroscope_old.axis.z);

        sensors[i].accelerometer_old.axis.x = apply_lpf(sensors[i].accelerometer.axis.x, sensors[i].accelerometer_old.axis.x);
        sensors[i].accelerometer_old.axis.y = apply_lpf(sensors[i].accelerometer.axis.y, sensors[i].accelerometer_old.axis.y);
        sensors[i].accelerometer_old.axis.z = apply_lpf(sensors[i].accelerometer.axis.z, sensors[i].accelerometer_old.axis.z);

        sensors[i].gyroscope = FusionCalibrationInertial(sensors[i].gyroscope_old, sensors[i].calibration.gyroscopeMisalignment, sensors[i].calibration.gyroscopeSensitivity, sensors[i].calibration.gyroscopeOffset);
        sensors[i].accelerometer = FusionCalibrationInertial(sensors[i].accelerometer_old, sensors[i].calibration.accelerometerMisalignment, sensors[i].calibration.accelerometerSensitivity, sensors[i].calibration.accelerometerOffset);
        sensors[i].gyroscope = FusionOffsetUpdate(&sensors[i].offset, sensors[i].gyroscope);

        const float deltaTime = (float) (sensors[i].timestamp - sensors[i].previousTimestamp) / 1e6f;
        sensors[i].previousTimestamp = sensors[i].timestamp;
        // FusionAhrsUpdateNoMagnetometer(&sensors[i].ahrs, sensors[i].gyroscope, sensors[i].accelerometer, deltaTime);

        FusionAhrsUpdateExternalHeading(&sensors[i].ahrs, sensors[i].gyroscope, sensors[i].accelerometer, 0.0f, deltaTime);
        const FusionQuaternion quat = FusionAhrsGetQuaternion(&sensors[i].ahrs);
        sensors_data[i][0] = quat.element.w;
        sensors_data[i][1] = quat.element.x;
        sensors_data[i][2] = quat.element.y;
        sensors_data[i][3] = quat.element.z;      
        sensors_data[i][4] = sensors[i].gyroscope_old.axis.x
        sensors_data[i][5] = sensors[i].gyroscope_old.axis.y
        sensors_data[i][6] = sensors[i].gyroscope_old.axis.z
    }
    print_output_data(sensors_data);
    // uint64_t loop_end = time_us_64();
    sleep_ms(sleep_time); // 120hz  
    }
}

void update_loop_no_lpf(float sleep_time, float sensors_data[][4], Sensor* sensors) {
     while (true) {
    read_all_sensors(sensors);
    for (int i=0; i<imu_reader_settings.sensorCount;i++) {
        sensors[i].gyroscope = FusionCalibrationInertial(sensors[i].gyroscope, sensors[i].calibration.gyroscopeMisalignment, sensors[i].calibration.gyroscopeSensitivity, sensors[i].calibration.gyroscopeOffset);
        sensors[i].accelerometer = FusionCalibrationInertial(sensors[i].accelerometer, sensors[i].calibration.accelerometerMisalignment, sensors[i].calibration.accelerometerSensitivity, sensors[i].calibration.accelerometerOffset);
        sensors[i].gyroscope = FusionOffsetUpdate(&sensors[i].offset, sensors[i].gyroscope);

        const float deltaTime = (float) (sensors[i].timestamp - sensors[i].previousTimestamp) / 1e6f;
        sensors[i].previousTimestamp = sensors[i].timestamp;
        // FusionAhrsUpdateNoMagnetometer(&sensors[i].ahrs, sensors[i].gyroscope, sensors[i].accelerometer, deltaTime);

        FusionAhrsUpdateExternalHeading(&sensors[i].ahrs, sensors[i].gyroscope, sensors[i].accelerometer, 0.0f, deltaTime);
        const FusionQuaternion quat = FusionAhrsGetQuaternion(&sensors[i].ahrs);
        sensors_data[i][0] = quat.element.w;
        sensors_data[i][1] = quat.element.x;
        sensors_data[i][2] = quat.element.y;
        sensors_data[i][3] = quat.element.z;           
    }
    print_output_data(sensors_data);
    // uint64_t loop_end = time_us_64();
    sleep_ms(sleep_time); // 120hz  
    }   
}

int main() {
    stdio_init_all();
    while (!tud_cdc_connected()) {
        sleep_ms(100);
    }
    wait_for_settings();
    // setup_sh2_service();
    
    float sensors_data[imu_reader_settings.sensorCount][7];
    float desired_loop_duration = 1000.0f/(float)imu_reader_settings.sampleRate;
    float sleep_time = desired_loop_duration - LOOP_DURATION_MS;
    int result = setup_I2C_pins();

    if (result != 1) {
		printf("I2C pin setup failed");
		return 1;
    }

	initialize_sensors();

    Sensor sensors[imu_reader_settings.sensorCount];
    initialize_sensors_values(sensors);
    initialize_calibrations(sensors); 
    initialize_algos(sensors);   

    if (imu_reader_settings.lpfEnabled == 1) {
        update_loop_with_lpf(sleep_time, sensors_data, sensors);
        printf("starting Update loop with lpf! \n");
    } else {
        // update_loop_no_lpf(sleep_time, sensors_data, sensors);
        printf("starting Update loop no lpf! \n");
    }

    return 0;
}