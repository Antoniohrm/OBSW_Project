# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/antonio/Documents/OB_SW/OBSW_Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/antonio/Documents/OB_SW/OBSW_Project/build

# Include any dependencies generated for this target.
include CMakeFiles/test_arduino_code.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_arduino_code.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_arduino_code.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_arduino_code.dir/flags.make

CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o: CMakeFiles/test_arduino_code.dir/flags.make
CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o: ../test_arduino_code.cpp
CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o: CMakeFiles/test_arduino_code.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/antonio/Documents/OB_SW/OBSW_Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o -MF CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o.d -o CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o -c /home/antonio/Documents/OB_SW/OBSW_Project/test_arduino_code.cpp

CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/antonio/Documents/OB_SW/OBSW_Project/test_arduino_code.cpp > CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.i

CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/antonio/Documents/OB_SW/OBSW_Project/test_arduino_code.cpp -o CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.s

CMakeFiles/test_arduino_code.dir/arduino_code.c.o: CMakeFiles/test_arduino_code.dir/flags.make
CMakeFiles/test_arduino_code.dir/arduino_code.c.o: ../arduino_code.c
CMakeFiles/test_arduino_code.dir/arduino_code.c.o: CMakeFiles/test_arduino_code.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/antonio/Documents/OB_SW/OBSW_Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test_arduino_code.dir/arduino_code.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_arduino_code.dir/arduino_code.c.o -MF CMakeFiles/test_arduino_code.dir/arduino_code.c.o.d -o CMakeFiles/test_arduino_code.dir/arduino_code.c.o -c /home/antonio/Documents/OB_SW/OBSW_Project/arduino_code.c

CMakeFiles/test_arduino_code.dir/arduino_code.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_arduino_code.dir/arduino_code.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/antonio/Documents/OB_SW/OBSW_Project/arduino_code.c > CMakeFiles/test_arduino_code.dir/arduino_code.c.i

CMakeFiles/test_arduino_code.dir/arduino_code.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_arduino_code.dir/arduino_code.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/antonio/Documents/OB_SW/OBSW_Project/arduino_code.c -o CMakeFiles/test_arduino_code.dir/arduino_code.c.s

# Object files for target test_arduino_code
test_arduino_code_OBJECTS = \
"CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o" \
"CMakeFiles/test_arduino_code.dir/arduino_code.c.o"

# External object files for target test_arduino_code
test_arduino_code_EXTERNAL_OBJECTS =

test_arduino_code: CMakeFiles/test_arduino_code.dir/test_arduino_code.cpp.o
test_arduino_code: CMakeFiles/test_arduino_code.dir/arduino_code.c.o
test_arduino_code: CMakeFiles/test_arduino_code.dir/build.make
test_arduino_code: lib/libgtest.a
test_arduino_code: CMakeFiles/test_arduino_code.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/antonio/Documents/OB_SW/OBSW_Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable test_arduino_code"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_arduino_code.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_arduino_code.dir/build: test_arduino_code
.PHONY : CMakeFiles/test_arduino_code.dir/build

CMakeFiles/test_arduino_code.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_arduino_code.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_arduino_code.dir/clean

CMakeFiles/test_arduino_code.dir/depend:
	cd /home/antonio/Documents/OB_SW/OBSW_Project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antonio/Documents/OB_SW/OBSW_Project /home/antonio/Documents/OB_SW/OBSW_Project /home/antonio/Documents/OB_SW/OBSW_Project/build /home/antonio/Documents/OB_SW/OBSW_Project/build /home/antonio/Documents/OB_SW/OBSW_Project/build/CMakeFiles/test_arduino_code.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_arduino_code.dir/depend

