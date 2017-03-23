cmake_minimum_required(VERSION 2.8.8)
project(Gui C CXX)

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

include_directories(${CMAKE_SOURCE_DIR}/Gui/core/inc)
include_directories(${CMAKE_SOURCE_DIR}/Gui/gui/inc)

file(GLOB headersQtGui ${headersQtGui} ${CMAKE__SOURCE_DIR}/Gui/gui/inc/*.h )
file(GLOB sourcesQtGui ${sourcesQtGui} ${CMAKE_SOURCE_DIR}/Gui/gui/src/*.cpp )

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOCSrcs ${CMAKE_SOURCE_DIR}/Gui/gui/inc/HorizontalLayout.h 
${CMAKE_SOURCE_DIR}/Gui/gui/inc/DetectorLayout.h 
${CMAKE_SOURCE_DIR}/Gui/gui/inc/PlaneLayout.h
${CMAKE_SOURCE_DIR}/Gui/gui/inc/AllPlaneLayout.h
${CMAKE_SOURCE_DIR}/Gui/gui/inc/Run.h
)

add_library(qtlib SHARED ${headerQtGui} ${sourcesQtGui} ${MOCSrcs})
target_link_libraries(qtlib ${QT_LIBRARIES} guicore)

add_executable(TestProg ${CMAKE_CURRENT_SOURCE_DIR}/test/TestProg.cpp)
target_link_libraries(TestProg ${QT_LIBRARIES} guicore)

add_executable(TestQt ${CMAKE_CURRENT_SOURCE_DIR}/test/TestQt.cpp)
target_link_libraries(TestQt qtlib)

add_executable(TestQtXML ${CMAKE_CURRENT_SOURCE_DIR}/test/TestQtXML.cpp)
target_link_libraries(TestQtXML qtlib ${ROOT_LIBRARIES} Tomo Helpers)
