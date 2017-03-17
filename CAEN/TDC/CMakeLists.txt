cmake_minimum_required(VERSION 2.8.8)
project(TDC)

#To put all the executables in build directory
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) 

enable_language(CXX)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} _build_type)

# Setting the C++ compiler to use
set("${CMAKE_CXX_COMPILER_ID}" TRUE CACHE STRING "C++ Compiler")
if (NOT GNU AND NOT Clang AND NOT Intel)
  message(WARNING "Unsupported compiler. Build will likely fail.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(MacroUtilities)
################################################################################

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

file(GLOB INI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/IniFiles/*)
message(STATUS ${INI_FILES})
file(COPY ${INI_FILES} DESTINATION ${CMAKE_SOURCE_DIR}/build)

#SET(ENV{LD_LIBRARY_PATH} "${CAEN_LIBRARY_DIR}:$ENV{LD_LIBRARY_PATH}")
#SET(CMAKE_LIBRARY_PATH ${CAEN_LIBRARY_DIR})
link_directories(${CAEN_LIBRARY_DIR})

find_package(CAEN REQUIRED)
message(STATUS "CAEN_LIBRARIES : ${CAEN_LIBRARIES}")
message(STATUS "CAEN_LIBRARY_DIR : ${CAEN_LIBRARY_DIR}")
include_directories(AFTER SYSTEM ${CAEN_INCLUDE_DIR})
find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)


file(GLOB headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hh)
file(GLOB sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_executable(daq904 ${CMAKE_CURRENT_SOURCE_DIR}/test/daq904.cc ${headers} ${sources})
target_link_libraries(daq904 "${ROOT_LIBRARIES} -L${CAEN_LIBRARY_DIR} -lCAENVME")
