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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/benty/workspace/Projects/ProdExpl/1.0.x

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf

# Include any dependencies generated for this target.
include ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/depend.make

# Include the progress variables for this target.
include ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/progress.make

# Include the compile flags for this target's objects.
include ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/flags.make

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/flags.make
ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o: ../../ProdExplUnits/source/ping_test.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o"
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source && /usr/bin/ccache /opt/ecos/gnutools/i386-elf/bin/i386-elf-gcc $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o   -c /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source/ping_test.c

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.i"
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source && /usr/bin/ccache /opt/ecos/gnutools/i386-elf/bin/i386-elf-gcc $(C_DEFINES) $(C_FLAGS) -E /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source/ping_test.c > CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.i

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.s"
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source && /usr/bin/ccache /opt/ecos/gnutools/i386-elf/bin/i386-elf-gcc $(C_DEFINES) $(C_FLAGS) -S /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source/ping_test.c -o CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.s

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.requires:
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.requires

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.provides: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.requires
	$(MAKE) -f ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/build.make ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.provides.build
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.provides

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.provides.build: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o

# Object files for target ProdTest_i386_elf
ProdTest_i386_elf_OBJECTS = \
"CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o"

# External object files for target ProdTest_i386_elf
ProdTest_i386_elf_EXTERNAL_OBJECTS =

ProdExplUnits/source/ProdTest_i386_elf: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o
ProdExplUnits/source/ProdTest_i386_elf: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/build.make
ProdExplUnits/source/ProdTest_i386_elf: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ProdTest_i386_elf"
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ProdTest_i386_elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/build: ProdExplUnits/source/ProdTest_i386_elf
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/build

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/requires: ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/ping_test.c.o.requires
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/requires

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/clean:
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source && $(CMAKE_COMMAND) -P CMakeFiles/ProdTest_i386_elf.dir/cmake_clean.cmake
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/clean

ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/depend:
	cd /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/benty/workspace/Projects/ProdExpl/1.0.x /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source /home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ProdExplUnits/source/CMakeFiles/ProdTest_i386_elf.dir/depend

