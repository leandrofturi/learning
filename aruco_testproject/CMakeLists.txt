cmake_minimum_required(VERSION 2.8)
project(aruco_testproject)

find_package(OpenCV REQUIRED)
find_package(aruco REQUIRED)
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(img          img.cpp)
add_executable(board        board.cpp)
add_executable(camera       camera.cpp)
add_executable(camera_board camera_board.cpp)

target_link_libraries(img           aruco opencv_highgui) # Eigen3::Eigen
target_link_libraries(board         aruco opencv_highgui)
target_link_libraries(camera        aruco opencv_highgui)
target_link_libraries(camera_board  aruco opencv_highgui opencv_calib3d)
