cmake_minimum_required(VERSION 2.8)
project(MarkerMap)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)

include_directories($ENV{CARMEN_HOME}/include)

link_directories($ENV{CARMEN_HOME}/lib)

add_executable(arucoest rot_estimation_main.cpp)

target_link_libraries(arucoest aruco opencv_highgui opencv_calib3d global ipc param_interface bumblebee_basic_interface)
