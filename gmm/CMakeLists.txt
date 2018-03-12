cmake_minimum_required(VERSION 2.8.8)
project(GMM C CXX)

#To put all the executables in build directory
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

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

################################################################################

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(MacroUtilities)

option(EIGEN "Use Eigen library for Matrix operations" OFF)


if(EIGEN)
add_definitions(-DUSE_EIGEN)
endif(EIGEN)

include_directories(/home/rsehgal/Tomo/TrackingMod)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB headersGMM ${headersGMM} ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h )
file(GLOB sourcesGMM ${sourcesGMM} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp )

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN_INCLUDE_DIR})

find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIR})

#add_library(gnuplot SHARED ${headersGnuplot} ${sourcesGnuplot})
add_executable(TestGaussian ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGaussian.cpp ${sourcesGMM})
target_link_libraries (TestGaussian Eigen3::Eigen ${ROOT_LIBRARIES})
#target_link_libraries(TestGnuplot gnuplot)

add_executable (TestEigen ${CMAKE_CURRENT_SOURCE_DIR}/test/TestEigen.cpp)
target_link_libraries (TestEigen Eigen3::Eigen)

add_executable (TestGmm ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGmm.cpp ${sourcesGMM})
target_link_libraries (TestGmm Eigen3::Eigen ${ROOT_LIBRARIES})

add_executable (TestGmmND ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGmmND.cpp ${sourcesGMM})
target_link_libraries (TestGmmND Eigen3::Eigen ${ROOT_LIBRARIES})

add_executable (TestGmm2D ${CMAKE_CURRENT_SOURCE_DIR}/test/TestGmm2D.cpp ${sourcesGMM})
target_link_libraries (TestGmm2D Eigen3::Eigen ${ROOT_LIBRARIES})
