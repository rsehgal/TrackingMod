set(TEST_EXECUTABLES
${CMAKE_SOURCE_DIR}/test/TestVisualization.cpp
)

macro(build_executables EXECUTABLES)
  foreach(EXECUTABLE ${EXECUTABLES})
    get_filename_component(TARGET_NAME ${EXECUTABLE} NAME_WE)
    add_executable(${TARGET_NAME} ${EXECUTABLE} ${sources} ${headers})
  if(EVE)
    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom Eve)
  else()
    target_link_libraries(${TARGET_NAME} ${ROOT_LIBRARIES} Geom)
  endif(EVE)
  endforeach()
endmacro()

build_executables("${TEST_EXECUTABLES}")