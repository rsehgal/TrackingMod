# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /home/rsehgal/BackBoneSoftwares/cmake-3.4.3/install/bin/cmake

# The command to remove a file.
RM = /home/rsehgal/BackBoneSoftwares/cmake-3.4.3/install/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rsehgal/Tomo/TrackingMod/Tree_V4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rsehgal/Tomo/TrackingMod/Tree_V4/build

# Include any dependencies generated for this target.
include CMakeFiles/track.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/track.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/track.dir/flags.make

CMakeFiles/track.dir/src/Track.cpp.o: CMakeFiles/track.dir/flags.make
CMakeFiles/track.dir/src/Track.cpp.o: ../src/Track.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rsehgal/Tomo/TrackingMod/Tree_V4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/track.dir/src/Track.cpp.o"
	ccache /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/track.dir/src/Track.cpp.o -c /home/rsehgal/Tomo/TrackingMod/Tree_V4/src/Track.cpp

CMakeFiles/track.dir/src/Track.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/track.dir/src/Track.cpp.i"
	ccache /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rsehgal/Tomo/TrackingMod/Tree_V4/src/Track.cpp > CMakeFiles/track.dir/src/Track.cpp.i

CMakeFiles/track.dir/src/Track.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/track.dir/src/Track.cpp.s"
	ccache /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rsehgal/Tomo/TrackingMod/Tree_V4/src/Track.cpp -o CMakeFiles/track.dir/src/Track.cpp.s

CMakeFiles/track.dir/src/Track.cpp.o.requires:

.PHONY : CMakeFiles/track.dir/src/Track.cpp.o.requires

CMakeFiles/track.dir/src/Track.cpp.o.provides: CMakeFiles/track.dir/src/Track.cpp.o.requires
	$(MAKE) -f CMakeFiles/track.dir/build.make CMakeFiles/track.dir/src/Track.cpp.o.provides.build
.PHONY : CMakeFiles/track.dir/src/Track.cpp.o.provides

CMakeFiles/track.dir/src/Track.cpp.o.provides.build: CMakeFiles/track.dir/src/Track.cpp.o


CMakeFiles/track.dir/inc/Track_rflx.cpp.o: CMakeFiles/track.dir/flags.make
CMakeFiles/track.dir/inc/Track_rflx.cpp.o: ../inc/Track_rflx.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rsehgal/Tomo/TrackingMod/Tree_V4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/track.dir/inc/Track_rflx.cpp.o"
	ccache /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/track.dir/inc/Track_rflx.cpp.o -c /home/rsehgal/Tomo/TrackingMod/Tree_V4/inc/Track_rflx.cpp

CMakeFiles/track.dir/inc/Track_rflx.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/track.dir/inc/Track_rflx.cpp.i"
	ccache /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rsehgal/Tomo/TrackingMod/Tree_V4/inc/Track_rflx.cpp > CMakeFiles/track.dir/inc/Track_rflx.cpp.i

CMakeFiles/track.dir/inc/Track_rflx.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/track.dir/inc/Track_rflx.cpp.s"
	ccache /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rsehgal/Tomo/TrackingMod/Tree_V4/inc/Track_rflx.cpp -o CMakeFiles/track.dir/inc/Track_rflx.cpp.s

CMakeFiles/track.dir/inc/Track_rflx.cpp.o.requires:

.PHONY : CMakeFiles/track.dir/inc/Track_rflx.cpp.o.requires

CMakeFiles/track.dir/inc/Track_rflx.cpp.o.provides: CMakeFiles/track.dir/inc/Track_rflx.cpp.o.requires
	$(MAKE) -f CMakeFiles/track.dir/build.make CMakeFiles/track.dir/inc/Track_rflx.cpp.o.provides.build
.PHONY : CMakeFiles/track.dir/inc/Track_rflx.cpp.o.provides

CMakeFiles/track.dir/inc/Track_rflx.cpp.o.provides.build: CMakeFiles/track.dir/inc/Track_rflx.cpp.o


# Object files for target track
track_OBJECTS = \
"CMakeFiles/track.dir/src/Track.cpp.o" \
"CMakeFiles/track.dir/inc/Track_rflx.cpp.o"

# External object files for target track
track_EXTERNAL_OBJECTS =

libtrack.so: CMakeFiles/track.dir/src/Track.cpp.o
libtrack.so: CMakeFiles/track.dir/inc/Track_rflx.cpp.o
libtrack.so: CMakeFiles/track.dir/build.make
libtrack.so: CMakeFiles/track.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rsehgal/Tomo/TrackingMod/Tree_V4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libtrack.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/track.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/track.dir/build: libtrack.so

.PHONY : CMakeFiles/track.dir/build

CMakeFiles/track.dir/requires: CMakeFiles/track.dir/src/Track.cpp.o.requires
CMakeFiles/track.dir/requires: CMakeFiles/track.dir/inc/Track_rflx.cpp.o.requires

.PHONY : CMakeFiles/track.dir/requires

CMakeFiles/track.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/track.dir/cmake_clean.cmake
.PHONY : CMakeFiles/track.dir/clean

CMakeFiles/track.dir/depend:
	cd /home/rsehgal/Tomo/TrackingMod/Tree_V4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rsehgal/Tomo/TrackingMod/Tree_V4 /home/rsehgal/Tomo/TrackingMod/Tree_V4 /home/rsehgal/Tomo/TrackingMod/Tree_V4/build /home/rsehgal/Tomo/TrackingMod/Tree_V4/build /home/rsehgal/Tomo/TrackingMod/Tree_V4/build/CMakeFiles/track.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/track.dir/depend
