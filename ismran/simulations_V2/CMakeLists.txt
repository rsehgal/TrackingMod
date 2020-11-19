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

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

add_definitions(-DFOR_SIMULATION)

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
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cc ../inc/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h ${PROJECT_SOURCE_DIR}/inc/*.hh ../../base/Vector3D.h) 

file(GLOB sourcesAnalysis_V2 ../analysis_V2/src/*.cpp)
add_library(Analysis_V2 SHARED ${sourcesAnalysis_V2})
target_link_libraries(Analysis_V2 ${ROOT_LIBRARIES})

#ROOT_GENERATE_DICTIONARY(G__Tomo ../analysis_V2/inc/Point3D.h ../analysis_V2/inc/ScintillatorBar_V2.h ../analysis_V2/inc/SingleMuonTrack.h LINKDEF ../analysis_V2/inc/DictLinkDef.h)
#add_library(Anallib SHARED ../analysis_V2/src/ScintillatorBar_V2.cpp ../analysis_V2/src/SingleMuonTrack.cpp ../analysis_V2/src/Point3D.cpp  G__Tomo.cxx)
#target_link_libraries(Anallib ${ROOT_LIBRARIES})

add_executable(mainIsmran_V2 main.cpp ${sources} ${headers})
if(CRY)
	target_link_libraries(mainIsmran_V2 cryg4interface ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${TOMOLIB} Analysis_V2 Analysis_V2)# Anallib)
else()
	target_link_libraries(mainIsmran_V2 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${TOMOLIB} Analysis_V2 Analysis_V2)# Anallib)
endif(CRY)
#target_link_libraries(main ${Geant4_LIBRARIES} Tomo)
