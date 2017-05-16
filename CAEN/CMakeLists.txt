if(EMULATOR)
add_definitions(-DUSE_EMULATOR)
add_subdirectory(Emulator)
endif(EMULATOR)


add_subdirectory(HVWrapper)
add_subdirectory(TDC)
add_subdirectory(test)
