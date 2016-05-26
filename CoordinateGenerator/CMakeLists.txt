include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/TemplatizedInterface/inc)
file(GLOB headers ${headers} ${PROJECT_SOURCE_DIR}/TemplatizedInterface/inc/*.h)
file(GLOB sources ${sources} ${PROJECT_SOURCE_DIR}/TemplatizedInterface/src/*.cpp)
file(GLOB headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_subdirectory(test)

