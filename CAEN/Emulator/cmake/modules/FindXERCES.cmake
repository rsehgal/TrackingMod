# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# XERCES_FOUND          If the ROOT is found
# XERCES_INCLUDE_DIR    PATH to the include directory
# XERCES_LIBRARIES      Most common libraries
# XERCES_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable XERCESROOT exists
set(XERCESROOT $ENV{XERCESROOT})

find_library(XERCES_LIBRARIES libxerces-c.so PATHS ${XERCESROOT}/lib)
if (XERCES_LIBRARIES) 
   set(XERCES_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" XERCESROOT  ${XERCES_LIBRARIES})
   set(XERCES_INCLUDE_DIR ${XERCESROOT}/include)
   set(XERCES_LIBRARY_DIR ${XERCESROOT}/lib)
   message(STATUS "Found XERCES library in ${XERCES_LIBRARIES}")		
else()
   message(STATUS "XERCES library not found; try to set a XERCESROOT environment variable to the base installation path or add -DXERCESROOT= to the cmake command")	
endif()

