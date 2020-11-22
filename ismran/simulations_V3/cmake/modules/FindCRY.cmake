# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# CRY_FOUND          If the ROOT is found
# CRY_INCLUDE_DIR    PATH to the include directory
# CRY_LIBRARIES      Most common libraries
# CRY_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable CRYROOT exists
set(CRYROOT $ENV{CRYROOT})

find_library(CRY_LIBRARIES libcry.so PATHS ${CRYROOT}/lib)
if (CRY_LIBRARIES) 
   set(CRY_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" CRYROOT  ${CRY_LIBRARIES})
   set(CRY_INCLUDE_DIR ${CRYROOT}/inc)
   set(CRY_LIBRARY_DIR ${CRYROOT}/lib)
   message(STATUS "Found CRY library in ${CRY_LIBRARIES}")		
else()
   message(STATUS "CRY library not found; try to set a CRYROOT environment variable to the base installation path or add -DCRYROOT= to the cmake command")	
endif()

