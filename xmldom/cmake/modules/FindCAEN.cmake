# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# CAEN_FOUND          If the ROOT is found
# CAEN_INCLUDE_DIR    PATH to the include directory
# CAEN_LIBRARIES      Most common libraries
# CAEN_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable CAENROOT exists
set(CAENROOT $ENV{CAENROOT})

find_library(CAEN_LIBRARIES libCAENVME.so PATHS ${CAENROOT}/lib)
if (CAEN_LIBRARIES) 
   set(CAEN_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" CAENROOT  ${CAEN_LIBRARIES})
   set(CAEN_INCLUDE_DIR ${CAENROOT}/include)
   set(CAEN_LIBRARY_DIR ${CAENROOT}/lib)
   message(STATUS "Found CAEN library in ${CAEN_LIBRARIES}")		
else()
   message(STATUS "CAEN library not found; try to set a CAENROOT environment variable to the base installation path or add -DCAENROOT= to the cmake command")	
endif()

