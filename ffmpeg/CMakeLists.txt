cmake_minimum_required(VERSION 2.8)
project(ffmpeg)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libswscale
    libavutil
)

add_executable(opencv    opencv.cpp)
add_executable(simple    simple.cpp)
add_executable(ff2cv     ff2cv.cpp)

target_link_libraries(opencv    opencv_highgui)
target_link_libraries(simple    opencv_highgui)
target_link_libraries(ff2cv     opencv_highgui PkgConfig::LIBAV)
