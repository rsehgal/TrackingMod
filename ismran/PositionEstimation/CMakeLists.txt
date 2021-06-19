cmake_minimum_required(VERSION 2.8)
project(PositionEstimation)

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

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../cmake/modules)

#option(CRY "Enable CRY library for cosmic event generation" OFF)
#option(USE_CALIBRATION "Use Calibration data for Sampling of time" OFF)
#option(USE_FOR_SIMULATION "Use Analyzer for simulated data" OFF)
#option(STAGGERED_GEOM "Use Scattered geometry configurtion" OFF)
#option(SINGLE_POINT_CALIBRATION "Use Single Point Muon Calibration" ON)
#option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
#option(FINGER_PATTERN_CONDITION "Flag to see the finger plot of layers" OFF)

#if(FINGER_PATTERN_CONDITION)
#add_definitions(-DUSE_CRITICAL_COND)
#endif(FINGER_PATTERN_CONDITION)
#
#if(WITH_GEANT4_UIVIS)
#  find_package(Geant4 REQUIRED ui_all vis_all)
#else()
#  find_package(Geant4 REQUIRED)
#endif()
#
#if(SINGLE_POINT_CALIBRATION)
#add_definitions(-DSINGLE_POINT_CALIBRATION)
#endif(SINGLE_POINT_CALIBRATION)
#
#
#if(STAGGERED_GEOM)
#add_definitions(-DSTAGGERED_GEOM)
#endif(STAGGERED_GEOM)
#
#if(CRY)
#add_definitions(-DUSE_CRY)
#endif(CRY)
#
#if(USE_FOR_SIMULATION)
#add_definitions(-DUSE_FOR_SIMULATION)
#endif(USE_FOR_SIMULATION)
#
#if(USE_CALIBRATION)
#add_definitions(-DUSE_CALIBRATION)
#endif(USE_CALIBRATION)

#----------------------------------------------------------------------------
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

#file(GLOB MAC_FILES ${PROJECT_SOURCE_DIR}/MacFiles/*)
#message(STATUS ${MAC_FILES})
##file(COPY ${MAC_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)
#file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#find_package(Geant4 REQUIRED)
#include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(../..)
include_directories(../inc)
include_directories(../../base)
include_directories(../analysis_V2/inc)
include_directories(../ml)

#ROOT_GENERATE_DICTIONARY(G__Tomo ../inc/Point3D.h LINKDEF ../inc/DictLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__Tomo ../analysis_V2/inc/Point3D.h ../analysis_V2/inc/ScintillatorBar_V2.h ../analysis_V2/inc/SingleMuonTrack.h LINKDEF ../analysis_V2/inc/DictLinkDef.h)
add_library(Tracklib SHARED ../analysis_V2/src/ScintillatorBar_V2.cpp ../analysis_V2/src/SingleMuonTrack.cpp ../analysis_V2/src/Point3D.cpp G__Tomo.cxx)
target_link_libraries(Tracklib ${ROOT_LIBRARIES})

file(GLOB analysisSources ../analysis_V2/src/*.cpp) 
add_library(Analysis SHARED ${analysisSources})
target_link_libraries(Analysis ${ROOT_LIBRARIES} Tracklib)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp  ../inc/*.cpp ../ml/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h ../../base/Vector3D.h ../ml/ml.h) 

SET(TESTSDIR ${TESTSDIR}
${CMAKE_SOURCE_DIR}/test
)

#COLLECTING ALL TEST FILES
macro(collect_tests TESTS)
  foreach(TEST ${TESTS})
    file(GLOB tests ${tests} ${TEST}/*.cpp)
  endforeach()
endmacro()

macro(build_executables EXECUTABLES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    add_executable(${TARGET_NAME} ${EXECUTABLE} ${headers} ${sources})
    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Analysis)
 endforeach()
endmacro()

collect_tests("${TESTSDIR}")
build_executables("${tests}")

#add_executable(TestTimeToPositionResolution test/TestTimeToPositionResolution.cpp ${sources} ${headers})
#target_link_libraries(TestTimeToPositionResolution ${ROOT_LIBRARIES} ${TOMOLIB} Analysis)

