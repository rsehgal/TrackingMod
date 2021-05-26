cmake_minimum_required(VERSION 2.8)
project(MST)

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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

option(CRY "Enable CRY library for cosmic event generation" OFF)
option(PRINT_EVENT_INFO "Printing some information from the event" OFF)
option(USE_CALIBRATION "Use Calibration data for Sampling of time" OFF)
option(CROSS_GEOMETRY "Simulation with Cross geometry" OFF)


if(PRINT_EVENT_INFO)
add_definitions(-DPRINT_EVENT_INFO)
endif(PRINT_EVENT_INFO)


if(CROSS_GEOMETRY)
add_definitions(-DCROSS_GEOMETRY)
endif(CROSS_GEOMETRY)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

add_definitions(-DFOR_SIMULATION)
add_definitions(-DNEW_GEN)


if(USE_CALIBRATION)
add_definitions(-DUSE_CALIBRATION)
endif(USE_CALIBRATION)
#----------------------------------------------------------------------------
if(CRY)
add_definitions(-DUSE_CRY)
endif(CRY)

if(USE_RND)
add_definitions(-DRANDOM_GUN)
endif(USE_RND)

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

file(GLOB MAC_FILES ${PROJECT_SOURCE_DIR}/MacFiles/*)
message(STATUS ${MAC_FILES})
#file(COPY ${MAC_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)
file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(../..)
include_directories(../inc)
include_directories(../../base)
include_directories(../analysis_V2/inc)
include_directories(../../CRY_Manual_Interface)
include_directories(../../Cry/inc)

file(GLOB headersCry ${headersCry} ../../Cry/inc/*.h )
file(GLOB sourcesCry ${sourcesCry} ../../Cry/src/*.cc)
add_library(cry SHARED ${sourcesCry})

add_library(cry_manual_interface SHARED ../../CRY_Manual_Interface/cry_manual_interface.cpp ../../CRY_Manual_Interface/MuonReader.cpp )
target_link_libraries(cry_manual_interface cry ${ROOT_LIBRARIES})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cc ../inc/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h ${PROJECT_SOURCE_DIR}/inc/*.hh ../../base/Vector3D.h) 
file(GLOB sourcesAnalysis_V2 ../analysis_V2/src/*.cpp ../inc/*.cpp)
add_library(Analysis_V2 SHARED ${sourcesAnalysis_V2})
target_link_libraries(Analysis_V2 ${ROOT_LIBRARIES})

add_executable(mainIsmran_V3 main.cpp ${sources} ${headers})
target_link_libraries(mainIsmran_V3  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Analysis_V2 cry_manual_interface)

add_executable(TestManualCry TestManualCry.cpp)
target_link_libraries(TestManualCry cry_manual_interface ${ROOT_LIBRARIES})

add_executable(MuonRead MuonRead.cpp)
target_link_libraries(MuonRead cry_manual_interface ${ROOT_LIBRARIES})

add_executable(MuonRead_V2 MuonRead_V2.cpp)
target_link_libraries(MuonRead_V2 cry_manual_interface ${ROOT_LIBRARIES})

add_executable(ExactHitRead ExactHitRead.cpp)
target_link_libraries(ExactHitRead ${ROOT_LIBRARIES} Analysis_V2)
#target_link_libraries(main ${Geant4_LIBRARIES} Tomo)
