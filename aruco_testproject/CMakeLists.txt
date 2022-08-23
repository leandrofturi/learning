cmake_minimum_required(VERSION 2.8)
project(aruco_testproject)

SET(VERSION_REQ_CUDA "8.0")
find_package(ZED 2 REQUIRED)
SET(SPECIAL_OS_LIBS "pthread")

find_package(CUDA ${VERSION_REQ_CUDA} REQUIRED)
 
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(img          img.cpp)
add_executable(board        board.cpp)
add_executable(camera       camera.cpp)
add_executable(camera_board camera_board.cpp)
add_executable(camera_zed camera_zed.cpp)

target_link_libraries(img           aruco opencv_highgui) # Eigen3::Eigen
target_link_libraries(board         aruco opencv_highgui)
target_link_libraries(camera        aruco opencv_highgui)
target_link_libraries(camera_board  aruco opencv_highgui opencv_calib3d)
target_link_libraries(camera_zed  aruco opencv_highgui opencv_calib3d ${ZED_LIBRARIES} ${SPECIAL_OS_LIBS} ${CUDA_LIBRARIES} ${CUDA_npps_LIBRARY} ${CUDA_nppi_LIBRARY})
