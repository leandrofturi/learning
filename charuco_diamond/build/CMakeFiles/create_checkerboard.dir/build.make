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
CMAKE_SOURCE_DIR = /home/lume/Documents/learning/charuco_diamond

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lume/Documents/learning/charuco_diamond/build

# Include any dependencies generated for this target.
include CMakeFiles/create_checkerboard.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/create_checkerboard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/create_checkerboard.dir/flags.make

CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o: CMakeFiles/create_checkerboard.dir/flags.make
CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o: ../create_checkerboard.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lume/Documents/learning/charuco_diamond/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o -c /home/lume/Documents/learning/charuco_diamond/create_checkerboard.cpp

CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lume/Documents/learning/charuco_diamond/create_checkerboard.cpp > CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.i

CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lume/Documents/learning/charuco_diamond/create_checkerboard.cpp -o CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.s

# Object files for target create_checkerboard
create_checkerboard_OBJECTS = \
"CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o"

# External object files for target create_checkerboard
create_checkerboard_EXTERNAL_OBJECTS =

create_checkerboard: CMakeFiles/create_checkerboard.dir/create_checkerboard.cpp.o
create_checkerboard: CMakeFiles/create_checkerboard.dir/build.make
create_checkerboard: /usr/local/lib/libopencv_highgui.so.4.5.5
create_checkerboard: /usr/local/lib/libopencv_videoio.so.4.5.5
create_checkerboard: /usr/local/lib/libopencv_imgcodecs.so.4.5.5
create_checkerboard: /usr/local/lib/libopencv_imgproc.so.4.5.5
create_checkerboard: /usr/local/lib/libopencv_core.so.4.5.5
create_checkerboard: CMakeFiles/create_checkerboard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lume/Documents/learning/charuco_diamond/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable create_checkerboard"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/create_checkerboard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/create_checkerboard.dir/build: create_checkerboard

.PHONY : CMakeFiles/create_checkerboard.dir/build

CMakeFiles/create_checkerboard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/create_checkerboard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/create_checkerboard.dir/clean

CMakeFiles/create_checkerboard.dir/depend:
	cd /home/lume/Documents/learning/charuco_diamond/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lume/Documents/learning/charuco_diamond /home/lume/Documents/learning/charuco_diamond /home/lume/Documents/learning/charuco_diamond/build /home/lume/Documents/learning/charuco_diamond/build /home/lume/Documents/learning/charuco_diamond/build/CMakeFiles/create_checkerboard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/create_checkerboard.dir/depend
