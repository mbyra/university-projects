# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release

# Include any dependencies generated for this target.
include CMakeFiles/middle_ages.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/middle_ages.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/middle_ages.dir/flags.make

CMakeFiles/middle_ages.dir/src/engine.c.o: CMakeFiles/middle_ages.dir/flags.make
CMakeFiles/middle_ages.dir/src/engine.c.o: ../src/engine.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/middle_ages.dir/src/engine.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/middle_ages.dir/src/engine.c.o   -c /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/engine.c

CMakeFiles/middle_ages.dir/src/engine.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/middle_ages.dir/src/engine.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/engine.c > CMakeFiles/middle_ages.dir/src/engine.c.i

CMakeFiles/middle_ages.dir/src/engine.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/middle_ages.dir/src/engine.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/engine.c -o CMakeFiles/middle_ages.dir/src/engine.c.s

CMakeFiles/middle_ages.dir/src/engine.c.o.requires:
.PHONY : CMakeFiles/middle_ages.dir/src/engine.c.o.requires

CMakeFiles/middle_ages.dir/src/engine.c.o.provides: CMakeFiles/middle_ages.dir/src/engine.c.o.requires
	$(MAKE) -f CMakeFiles/middle_ages.dir/build.make CMakeFiles/middle_ages.dir/src/engine.c.o.provides.build
.PHONY : CMakeFiles/middle_ages.dir/src/engine.c.o.provides

CMakeFiles/middle_ages.dir/src/engine.c.o.provides.build: CMakeFiles/middle_ages.dir/src/engine.c.o

CMakeFiles/middle_ages.dir/src/middle_ages.c.o: CMakeFiles/middle_ages.dir/flags.make
CMakeFiles/middle_ages.dir/src/middle_ages.c.o: ../src/middle_ages.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/middle_ages.dir/src/middle_ages.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/middle_ages.dir/src/middle_ages.c.o   -c /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/middle_ages.c

CMakeFiles/middle_ages.dir/src/middle_ages.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/middle_ages.dir/src/middle_ages.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/middle_ages.c > CMakeFiles/middle_ages.dir/src/middle_ages.c.i

CMakeFiles/middle_ages.dir/src/middle_ages.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/middle_ages.dir/src/middle_ages.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/middle_ages.c -o CMakeFiles/middle_ages.dir/src/middle_ages.c.s

CMakeFiles/middle_ages.dir/src/middle_ages.c.o.requires:
.PHONY : CMakeFiles/middle_ages.dir/src/middle_ages.c.o.requires

CMakeFiles/middle_ages.dir/src/middle_ages.c.o.provides: CMakeFiles/middle_ages.dir/src/middle_ages.c.o.requires
	$(MAKE) -f CMakeFiles/middle_ages.dir/build.make CMakeFiles/middle_ages.dir/src/middle_ages.c.o.provides.build
.PHONY : CMakeFiles/middle_ages.dir/src/middle_ages.c.o.provides

CMakeFiles/middle_ages.dir/src/middle_ages.c.o.provides.build: CMakeFiles/middle_ages.dir/src/middle_ages.c.o

CMakeFiles/middle_ages.dir/src/parse.c.o: CMakeFiles/middle_ages.dir/flags.make
CMakeFiles/middle_ages.dir/src/parse.c.o: ../src/parse.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/middle_ages.dir/src/parse.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/middle_ages.dir/src/parse.c.o   -c /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/parse.c

CMakeFiles/middle_ages.dir/src/parse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/middle_ages.dir/src/parse.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/parse.c > CMakeFiles/middle_ages.dir/src/parse.c.i

CMakeFiles/middle_ages.dir/src/parse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/middle_ages.dir/src/parse.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/src/parse.c -o CMakeFiles/middle_ages.dir/src/parse.c.s

CMakeFiles/middle_ages.dir/src/parse.c.o.requires:
.PHONY : CMakeFiles/middle_ages.dir/src/parse.c.o.requires

CMakeFiles/middle_ages.dir/src/parse.c.o.provides: CMakeFiles/middle_ages.dir/src/parse.c.o.requires
	$(MAKE) -f CMakeFiles/middle_ages.dir/build.make CMakeFiles/middle_ages.dir/src/parse.c.o.provides.build
.PHONY : CMakeFiles/middle_ages.dir/src/parse.c.o.provides

CMakeFiles/middle_ages.dir/src/parse.c.o.provides.build: CMakeFiles/middle_ages.dir/src/parse.c.o

# Object files for target middle_ages
middle_ages_OBJECTS = \
"CMakeFiles/middle_ages.dir/src/engine.c.o" \
"CMakeFiles/middle_ages.dir/src/middle_ages.c.o" \
"CMakeFiles/middle_ages.dir/src/parse.c.o"

# External object files for target middle_ages
middle_ages_EXTERNAL_OBJECTS =

middle_ages: CMakeFiles/middle_ages.dir/src/engine.c.o
middle_ages: CMakeFiles/middle_ages.dir/src/middle_ages.c.o
middle_ages: CMakeFiles/middle_ages.dir/src/parse.c.o
middle_ages: CMakeFiles/middle_ages.dir/build.make
middle_ages: CMakeFiles/middle_ages.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable middle_ages"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/middle_ages.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/middle_ages.dir/build: middle_ages
.PHONY : CMakeFiles/middle_ages.dir/build

CMakeFiles/middle_ages.dir/requires: CMakeFiles/middle_ages.dir/src/engine.c.o.requires
CMakeFiles/middle_ages.dir/requires: CMakeFiles/middle_ages.dir/src/middle_ages.c.o.requires
CMakeFiles/middle_ages.dir/requires: CMakeFiles/middle_ages.dir/src/parse.c.o.requires
.PHONY : CMakeFiles/middle_ages.dir/requires

CMakeFiles/middle_ages.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/middle_ages.dir/cmake_clean.cmake
.PHONY : CMakeFiles/middle_ages.dir/clean

CMakeFiles/middle_ages.dir/depend:
	cd /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056 /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056 /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release /home/smutnyjoe/Zaliczeniowe2Git/IPP-mb347056/release/CMakeFiles/middle_ages.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/middle_ages.dir/depend

