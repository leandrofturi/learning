# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lume/Documents/learning/aruco_testproject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lume/Documents/learning/aruco_testproject/build

# Include any dependencies generated for this target.
include CMakeFiles/img.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/img.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/img.dir/flags.make

CMakeFiles/img.dir/img.cpp.o: CMakeFiles/img.dir/flags.make
CMakeFiles/img.dir/img.cpp.o: ../img.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lume/Documents/learning/aruco_testproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/img.dir/img.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/img.dir/img.cpp.o -c /home/lume/Documents/learning/aruco_testproject/img.cpp

CMakeFiles/img.dir/img.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/img.dir/img.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lume/Documents/learning/aruco_testproject/img.cpp > CMakeFiles/img.dir/img.cpp.i

CMakeFiles/img.dir/img.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/img.dir/img.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lume/Documents/learning/aruco_testproject/img.cpp -o CMakeFiles/img.dir/img.cpp.s

# Object files for target img
img_OBJECTS = \
"CMakeFiles/img.dir/img.cpp.o"

# External object files for target img
img_EXTERNAL_OBJECTS =

img: CMakeFiles/img.dir/img.cpp.o
img: CMakeFiles/img.dir/build.make
img: /usr/local/lib/libopencv_highgui.so.4.5.5
img: /usr/local/lib/libopencv_videoio.so.4.5.5
img: /usr/local/lib/libopencv_imgcodecs.so.4.5.5
img: /usr/local/lib/libopencv_imgproc.so.4.5.5
img: /usr/local/lib/libopencv_core.so.4.5.5
img: CMakeFiles/img.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lume/Documents/learning/aruco_testproject/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable img"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/img.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/img.dir/build: img

.PHONY : CMakeFiles/img.dir/build

CMakeFiles/img.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/img.dir/cmake_clean.cmake
.PHONY : CMakeFiles/img.dir/clean

CMakeFiles/img.dir/depend:
	cd /home/lume/Documents/learning/aruco_testproject/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lume/Documents/learning/aruco_testproject /home/lume/Documents/learning/aruco_testproject /home/lume/Documents/learning/aruco_testproject/build /home/lume/Documents/learning/aruco_testproject/build /home/lume/Documents/learning/aruco_testproject/build/CMakeFiles/img.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/img.dir/depend
