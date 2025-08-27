message(STATUS "Imu reader has been added")

create_full_target(imu_reader)

add_subdirectory(ism330dlc)
add_subdirectory(settings)
add_subdirectory(utils)