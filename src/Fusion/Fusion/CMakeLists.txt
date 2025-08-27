file(GLOB_RECURSE files "*.c")

add_library(Fusion ${files})
create_headers_target(Fusion)

target_include_directories(Fusion INTERFACE Fusion_headers)

target_link_libraries(Fusion
PRIVATE 
pico_stdlib
hardware_i2c
hardware_rtc
imu_reader_headers
PUBLIC
i2c_helpers
)