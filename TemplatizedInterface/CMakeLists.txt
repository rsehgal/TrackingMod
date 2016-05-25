include_directories(${PROJECT_SOURCE_DIR}/CoordinateGenerator/inc)
file(GLOB headers ${headers} ${PROJECT_SOURCE_DIR}/CoordinateGenerator/inc/*.h)
file(GLOB sources ${sources} ${PROJECT_SOURCE_DIR}/CoordinateGenerator/src/*.cpp)

file(GLOB headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_subdirectory(test)