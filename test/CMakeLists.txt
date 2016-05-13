if(EVE)
set(TEST_EXECUTABLES ${TEST_EXECUTABLES}
${CMAKE_SOURCE_DIR}/test/TestVisualization.cpp
${CMAKE_SOURCE_DIR}/test/TestVisualization2.cpp
${CMAKE_SOURCE_DIR}/test/ScintPattern.cpp
#${CMAKE_SOURCE_DIR}/test/ShowerDetectionTest.cpp

)
endif(EVE)

if(UNITTEST)
set(TEST_EXECUTABLES ${TEST_EXECUTABLES}
${CMAKE_SOURCE_DIR}/test/ShowerDetectionTest.cpp
${CMAKE_SOURCE_DIR}/test/TestSetupManager.cpp
${CMAKE_SOURCE_DIR}/test/TestCmsRpc.cpp
)
endif(UNITTEST)

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