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

#----------------------------------------------------------------------------
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

#find_package(Geant4 REQUIRED)
#include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(../..)
include_directories(.)
include_directories(../inc)
include_directories(../../base)
include_directories(../analysis_V2/inc)
include_directories(../../Helpers/inc)
include_directories(../../TemplatizedInterface/inc)
include_directories(Eve/inc)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
#ROOT_GENERATE_DICTIONARY(G__Tomo ../inc/Point3D.h LINKDEF ../inc/DictLinkDef.h)

file(GLOB visualizerSources Eve/src/*.cpp *.cpp ../../Helpers/src/CommonFunc.cpp ../../Helpers/src/Track.cpp) 
#file(GLOB visualizerSources Eve/src/*.cpp *.cpp ../../Helpers/src/CommonFunc.cpp ../../Helpers/src/Track.cpp ../analysis_V2/src/Point3D.cpp ../inc/PsBar.cpp) 
add_library(Visualizer SHARED ${visualizerSources})
target_link_libraries(Visualizer ${ROOT_LIBRARIES} Eve Geom)


#file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h ../../base/Vector3D.h ../ml/ml.h) 

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
    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Visualizer )
 endforeach()
endmacro()

collect_tests("${TESTSDIR}")
build_executables("${tests}")

#add_executable(TestTimeToPositionResolution test/TestTimeToPositionResolution.cpp ${sources} ${headers})
#target_link_libraries(TestTimeToPositionResolution ${ROOT_LIBRARIES} ${TOMOLIB} Analysis)

