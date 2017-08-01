# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# XMLDOM_FOUND          If the ROOT is found
# XMLDOM_INCLUDE_DIR    PATH to the include directory
# XMLDOM_LIBRARIES      Most common libraries
# XMLDOM_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable XMLDOMROOT exists
set(XMLDOMROOT $ENV{XMLDOMROOT})

find_library(XMLDOM_LIBRARIES libxmldom.so PATHS ${XMLDOMROOT}/lib)
if (XMLDOM_LIBRARIES) 
   set(XMLDOM_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" XMLDOMROOT  ${XMLDOM_LIBRARIES})
   set(XMLDOM_INCLUDE_DIR ${XMLDOMROOT}/include)
   set(XMLDOM_LIBRARY_DIR ${XMLDOMROOT}/lib)
   message(STATUS "Found XMLDOM library in ${XMLDOM_LIBRARIES}")		
else()
   message(STATUS "XMLDOM library not found; try to set a XMLDOMROOT environment variable to the base installation path or add -DXMLDOMROOT= to the cmake command")	
endif()

