include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
file(GLOB headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_subdirectory(test)

