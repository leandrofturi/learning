cmake_minimum_required(VERSION 2.8)
project(charuco_diamond)

find_package(OpenCV REQUIRED)

set(SOURCES dict_4x4.hpp)

add_executable(create_checkerboard    create_checkerboard.cpp)
add_executable(create_diamond         create_diamond.cpp)
add_executable(detect_diamonds        detect_diamonds.cpp)

target_link_libraries(create_checkerboard    opencv_highgui)
target_link_libraries(create_diamond         opencv_highgui opencv_aruco)
target_link_libraries(detect_diamonds        opencv_highgui opencv_calib3d opencv_aruco)
