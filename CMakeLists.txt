cmake_minimum_required(VERSION 2.8.8)
project(Tracking)

#To put all the executables in build directory
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) 

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

option(VISUALIZATION "Enable or Disable Visualization" ON)
option(OPENGL "Use OpenGL for Visualization" OFF)
option(EVE "Use Eve for Visualization" OFF)
option(UNITTEST "Enable various Unit tests" ON)
option(ROOT "Enable ROOT" ON)
option(EFF_AND "Efficiency will be calculated only if both plane fires" ON)
option(EFF_SETUP_AND "Efficiency w.r.t full Setup, i.e all the detector fires for an event" ON)
option(CLUSTER_SIZE "Allow to restrict the Cluster Size cut, default value will be 2 if switched ON." ON)
option(ACCUMULATE_TRACK "All the tracks will be displayed" OFF)
option(ANG_DIST "To calculate the angular distribution from tracks" OFF)
option(CGAL "CGAL library for geometrical algorithms" OFF)
option(EMULATOR "Building Emulator for HV Supply" OFF)
#option(CAENWRAPPER "Building Wrapper for CAEN Library" OFF)
option(CAEN "Enabling CAEN Libraries" OFF)
option(GNUPLOT "Enabling GnuPlot" OFF)
option(QT "Enabling QtGui" OFF)
option(CRY "Enable CRY library for cosmic event generation" OFF)

find_package(MYSQL REQUIRED)
include_directories(${MYSQL_INCLUDE_DIR})

if(CRY)
add_subdirectory(Cry)
endif(CRY)

#For Xml Processing
find_package(XERCES REQUIRED)
include_directories(${XERCES_INCLUDE_DIR})
add_subdirectory(xmldom)
#include_directories(${CMAKE_SOURCE_DIR}/xmldom)
find_package(XMLDOM REQUIRED)
include_directories(${XMLDOM_INCLUDE_DIR})

if(CGAL)
find_package(CGAL REQUIRED)
message(STATUS "************* Link CGAL **************")
include(${CGAL_USE_FILE})
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
${CGAL_LIBRARIES}
)
endif(CGAL)


if(ANG_DIST)
if(ACCUMULATE_TRACK)
else()
message(STATUS "You have switched ON ANG_DIST but ACCUMULATE_TRACK is OFF, so i am switching ON ACCUMULATE_TRACK")
set(ACCUMULATE_TRACK ON)
endif(ACCUMULATE_TRACK)
add_definitions(-DANG_DIST)
endif(ANG_DIST)

if(EMULATOR)
add_definitions(-DUSE_EMULATOR)
endif(EMULATOR)

if(ACCUMULATE_TRACK)
add_definitions(-DACCUMULATE_TRACK)
endif(ACCUMULATE_TRACK)

if(EFF_AND)
add_definitions(-DEFF_AND)
endif(EFF_AND)

if(EFF_SETUP_AND)
add_definitions(-DEFF_SETUP_AND)
endif(EFF_SETUP_AND)

if(CLUSTER_SIZE)
add_definitions(-DCLUSTER_SIZE)
endif(CLUSTER_SIZE)


if(VISUALIZATION)
add_definitions(-DSHOW_VISUALIZATION)
endif(VISUALIZATION)

if(VISUALIZATION AND EVE AND OPENGL)
message(STATUS "You have switched on both EVE and OPENGL, only one can work at a time, so i am Switching OFF both and using ROOTs default graphics")
set(EVE OFF)
set(OPENGL OFF)
endif()


if(VISUALIZATION AND EVE)
add_definitions(-DUSE_EVE)
else()
if(VISUALIZATION AND OPENGL)
add_definitions(-DUSE_OGL)
endif()
endif() 


if(ROOT)
add_definitions(-DUSE_ROOT)
find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIR})
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
${ROOT_LIBRARIES}
)

if(EVE)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
Eve Geom
)
else()
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
Geom
)
endif(EVE)

endif(ROOT)

#find_package(Geant4 REQUIRED)

#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/inc)
#include_directories(${PROJECT_SOURCE_DIR}/visualizer)

#file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h)
#file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
if(VISUALIZATION)
if(EVE)
file(GLOB headers ${headers} ${CMAKE_SOURCE_DIR}/visualizer/Eve/*.h)
file(GLOB sources ${sources}  ${CMAKE_SOURCE_DIR}/visualizer/Eve/*.cpp )
else()
file(GLOB headers ${headers} ${CMAKE_SOURCE_DIR}/visualizer/TGeo/Visualizer.h)
file(GLOB sources ${sources} ${CMAKE_SOURCE_DIR}/visualizer/TGeo/Visualizer.cpp )
endif(EVE)
endif(VISUALIZATION)

file(GLOB CFG_FILES ${PROJECT_SOURCE_DIR}/CfgFiles/*)
file(COPY ${CFG_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)

#add_subdirectory(TreeExercise)
#add_subdirectory(test)
#add_subdirectory(TemplatizedInterface)
#add_subdirectory(CoordinateGenerator)

#COLLECTING ALL INCLUDE DIRECTORIES
set(INCLUDES ${INCLUDES}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/inc
${CMAKE_SOURCE_DIR}/TemplatizedInterface/inc
${CMAKE_SOURCE_DIR}/Helpers/inc
#${PROJECT_SOURCE_DIR}/visualizer
${CMAKE_SOURCE_DIR}/inc
${CMAKE_SOURCE_DIR}
)

if(EVE)
set(INCLUDES ${INCLUDES}
${CMAKE_SOURCE_DIR}/TemplatizedInterface/inc/visualization
)
endif(EVE)

if(VISUALIZATION)
set(INCLUDES ${INCLUDES} ${CMAKE_SOURCE_DIR}/visualizer)
endif(VISUALIZATION)

macro(include_dirs INCLUDES)
  foreach(INCLUDE ${INCLUDES})
    include_directories(${INCLUDE})
  endforeach()
endmacro()

#COLLECTING ALL HEADER FILES
macro(collect_headers INCLUDES)
  foreach(INCLUDE ${INCLUDES})
    file(GLOB headers ${headers} ${INCLUDE}/*.h)
  endforeach()
endmacro()

#COLLECTING ALL SOURCE FILES
set(SOURCES ${SOURCES}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/src
${CMAKE_SOURCE_DIR}/TemplatizedInterface/src
${CMAKE_SOURCE_DIR}/src
)

macro(collect_sources SOURCES)
  foreach(SOURCE ${SOURCES})
    file(GLOB sources ${sources} ${SOURCE}/*.cpp)
  endforeach()
endmacro()

#COLLECTING ALL HELPERS FILES
set(HELPERS ${HELPERS}
${CMAKE_SOURCE_DIR}/Helpers/src
)

macro(collect_helpers HELPERS)
  foreach(HELPER ${HELPERS})
    file(GLOB helpers ${helpers} ${HELPER}/*.cpp)
  endforeach()
endmacro()


SET(TESTSDIR ${TESTSDIR}
${CMAKE_SOURCE_DIR}/CoordinateGenerator/test
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test/core
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test/plots
${CMAKE_SOURCE_DIR}/Simulations/MST/test
${CMAKE_SOURCE_DIR}/visualizer/test
#${CMAKE_SOURCE_DIR}/CAEN/test
#${CMAKE_SOURCE_DIR}/test
)

if(CGAL)
SET(TESTSDIR ${TESTSDIR} 
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test/cgal
)
endif(CGAL)


if(EVE)
SET(TESTSDIR ${TESTSDIR}
${CMAKE_SOURCE_DIR}/TemplatizedInterface/test/visualization
)
endif(EVE)

#COLLECTING ALL TEST FILES
macro(collect_tests TESTS)
  foreach(TEST ${TESTS})
    file(GLOB tests ${tests} ${TEST}/*.cpp)
  endforeach()
endmacro()

macro(build_executables EXECUTABLES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    add_executable(${TARGET_NAME} ${EXECUTABLE} ${headers})
    target_link_libraries(${TARGET_NAME} ${TOMOLIB} ${HELPERSLIB} ${EMULATORLIB})
#  if(EVE)
#    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom Eve)
#  else()
#    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom)
#  endif(EVE)
  endforeach()
endmacro()


#if(CAENWRAPPER)
#macro(collect_wrapper Wrappers)
#  foreach(Wrapper ${Wrappers})
#    file(GLOB wrappers ${wrappers} ${Wrapper}/*.cpp)
#  endforeach()
#endmacro()

#set(CAENWrapper ${CAENWrapper}
#${CMAKE_SOURCE_DIR}/CAEN/src
#)

#set(INCLUDES ${INCLUDES}
#${CMAKE_SOURCE_DIR}/CAEN/inc
#)
#endif(CAENWRAPPER)

#if(EMULATOR)
#macro(collect_emulators Emulators)
#  foreach(Emulator ${Emulators})
#    file(GLOB emulators ${emulators} ${Emulator}/*.cpp)
#  endforeach()
#endmacro()

#set(HVEmulator ${HVEmulator}
#${CMAKE_SOURCE_DIR}/CAEN/Emulator/src
#)

#set(INCLUDES ${INCLUDES}
#${CMAKE_SOURCE_DIR}/CAEN/Emulator/inc
#)
#endif(EMULATOR)

message(STATUS ${headers})
include_dirs("${INCLUDES}")
collect_headers("${INCLUDES}")
collect_sources("${SOURCES}")
collect_tests("${TESTSDIR}")
collect_helpers("${HELPERS}")

#if(CAENWRAPPER)
#collect_wrapper("${CAENWrapper}")
#endif(CAENWRAPPER)

#if(EMULATOR)
#collect_emulators("${HVEmulator}")
#endif(EMULATOR)

message(STATUS "--------- Following source file will be use for the creation of TOMO Shared library -----------")
message(STATUS ${sources})
message(STATUS "------------------------------------------------------------------------------------------")
add_library(Tomo SHARED ${sources})
target_link_libraries(Tomo ${EXTERNAL_LIBS})
set(TOMOLIB ${TOMOLIB}
Tomo
)

message(STATUS "--------- Following source files will be use for the creation of HELPERS Shared library -----------")
message(STATUS ${helpers})
message(STATUS "------------------------------------------------------------------------------------------")
add_library(Helpers SHARED ${helpers})
target_link_libraries(Helpers  ${XERCES_LIBRARIES} ${MYSQL_LIBRARIES})
#target_link_libraries(Tomo ${EXTERNAL_LIBS})
set(HELPERSLIB ${HELPERSLIB}
Helpers
)

message(STATUS "HELPERS : ${HELPERSLIB}")

#if(EMULATOR)
#message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
#add_library(HVEmulator SHARED ${emulators})
#set(EMULATORLIB ${EMULATORLIB}
#HVEmulator
#)
#endif(EMULATOR)

if(GNUPLOT)
add_subdirectory(gnuplot)
endif(GNUPLOT)

if(QT)
add_subdirectory(Gui)
endif(QT)

if(CAEN)
add_subdirectory(CAEN)
endif(CAEN)


#if(CAENWRAPPER)
#add_library(CAENWrapper SHARED ${wrappers})
#set(WRAPPERLIB ${WRAPPERLIB}
#CAENWrapper
#)
#endif(CAENWRAPPER)

link_directories(${CMAKE_SOURCE_DIR}/build)
#link_directories(${CMAKE_SOURCE_DIR}/build/xmldom)

build_executables("${tests}")
message(STATUS ${headers})
message(STATUS "====================================================")
message(STATUS ${tests})
message(STATUS "====================================================")
add_subdirectory(Simulations/MST)

#Copying the test geom.root file to build directory for visualization testing
file(GLOB ROOT_FILES ${PROJECT_SOURCE_DIR}/RootFiles/*)
message(STATUS ${ROOT_FILES})
file(COPY ${ROOT_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)
