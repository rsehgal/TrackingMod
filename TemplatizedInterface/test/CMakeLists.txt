add_executable(GlassRpc_test GlassRpc_test.cpp ${sources} ${headers})
target_link_libraries(GlassRpc_test ${ROOT_LIBRARIES} Geom Eve)