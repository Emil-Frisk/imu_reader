#include "settings.h"

void extract_part(const char* part, const char* buf, settings_enum setting) {
        char *pos = strstr(buf, part);
        
        if (pos == 0) {
            printf("Error while extracting part %s not found in the buffer\n", part);
            while(1);
        } else {
            char part_buffer[64];//SR=jotain|
            memset(part_buffer, 0, sizeof(part_buffer));
            int cursor = 0;
            int part_index = (pos - buf) + strlen(part);
            for (int j=part_index;j<SETTINGS_BUF_LEN-part_index; j++) {
                if (buf[j] != '|') {
                    part_buffer[cursor++] = buf[j];
                    continue;
                }
                part_buffer[cursor++] = '\0';
                break;
            }
            switch (setting) {
                case S_SENSOR_COUNT:
                    imu_reader_settings.sensorCount = atoi(part_buffer);
                    break;
                case S_LPF_ENABLED:
                    imu_reader_settings.lpfEnabled = atoi(part_buffer);
                    break;
                case S_LPF_ALPHA:
                    sscanf(part_buffer, "%f", &imu_reader_settings.lpf_alpha);
                    break;
                case S_SAMPLE_RATE:
                    imu_reader_settings.sampleRate = atoi(part_buffer);
                    break;
                default:
                    printf("Error: Wrong setting %d\n", setting);
                    while(1);
            }
            printf("Part %s found and saved: %s\n", part, part_buffer);
        }
}

void excract_settings(const char* buf) {
    settings_option = S_SENSOR_COUNT;
    extract_part("SC=", buf, settings_option);

    settings_option = S_LPF_ENABLED;
    extract_part("LPF_ENABLED=", buf, settings_option);

    settings_option = S_LPF_ALPHA;
    extract_part("LPF_ALPHA=", buf, settings_option);

    settings_option = S_SAMPLE_RATE;
    extract_part("SR=", buf, settings_option);

    printf("Settings: Sensor Count: %d; LPF Enabled %d; LPF_ALPHA %f; Sample Rate: %d\n",imu_reader_settings.sensorCount, imu_reader_settings.lpfEnabled, imu_reader_settings.lpf_alpha, imu_reader_settings.sampleRate);
}

// Waits for the user to send in some settings through the serial port
void wait_for_settings() {
    char buf[SETTINGS_BUF_LEN];
    while (1) {
        if (tud_cdc_available()) {
            uint32_t count = tud_cdc_read(buf, sizeof(buf)-1);
            if (count > 0) {
                buf[count] = '\0';
                // Process input (e.g., echo back)

                printf("Received settings: %s\n", buf);
                excract_settings(buf);
                break;
            }
        }
        sleep_ms(300); // Avoid tight loop
    }
}