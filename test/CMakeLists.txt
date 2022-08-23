cmake_minimum_required(VERSION 2.8)
project(markermap)

SET(VERSION_REQ_CUDA "8.0")
find_package(ZED 2 REQUIRED)
SET(SPECIAL_OS_LIBS "pthread")
find_package(CUDA ${VERSION_REQ_CUDA} REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)

add_executable(create create.cpp)
add_executable(detect detect.cpp)

target_link_libraries(create aruco opencv_highgui opencv_calib3d ${ZED_LIBRARIES} ${SPECIAL_OS_LIBS} ${CUDA_LIBRARIES} ${CUDA_npps_LIBRARY} ${CUDA_nppi_LIBRARY})
target_link_libraries(detect aruco opencv_highgui opencv_calib3d ${ZED_LIBRARIES} ${SPECIAL_OS_LIBS} ${CUDA_LIBRARIES} ${CUDA_npps_LIBRARY} ${CUDA_nppi_LIBRARY})
