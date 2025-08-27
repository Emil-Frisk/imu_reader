#ifndef IMU_READER_H
#define IMU_READER_H
typedef struct imu_reader_settings_t {
    int sensorCount;
    int lpfEnabled;
    float lpf_alpha;
    int sampleRate;
} imu_reader_settings_t;

enum settings_enum_e {
    S_SENSOR_COUNT,
    S_LPF_ENABLED,
    S_LPF_ALPHA,
    S_SAMPLE_RATE
};

// Global imu reader settings for the application
imu_reader_settings_t imu_reader_settings = {0};
typedef enum settings_enum_e settings_enum;
settings_enum settings_option;

#endif

