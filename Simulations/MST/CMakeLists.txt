cmake_minimum_required(VERSION 2.8)
project(MST)

file(GLOB MAC_FILES ${PROJECT_SOURCE_DIR}/MacFiles/*)
message(STATUS ${MAC_FILES})
file(COPY ${MAC_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h)

add_executable(main main.cpp ${sources} ${headers})
target_link_libraries(main ${Geant4_LIBRARIES})
