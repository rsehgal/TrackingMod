# - Finds Vc installation ( the wrapper library to SIMD intrinsics )
# This module sets up Vc information 
# It defines:
# MYSQL_FOUND          If the ROOT is found
# MYSQL_INCLUDE_DIR    PATH to the include directory
# MYSQL_LIBRARIES      Most common libraries
# MYSQL_LIBRARY_DIR    PATH to the library directory 

# look if an environment variable MYSQLROOT exists
set(MYSQL_LIB_DIR /usr/lib64/mysql)
set(MYSQL_INC_DIR /usr/include/mysql)

find_library(MYSQL_LIBRARIES libmysqlclient.so PATHS ${MYSQL_LIB_DIR})
if (MYSQL_LIBRARIES) 
   set(MYSQL_FOUND TRUE)	
   #string(REPLACE "/lib/libVc.a" "" MYSQLROOT  ${MYSQL_LIBRARIES})
   set(MYSQL_INCLUDE_DIR ${MYSQL_INC_DIR})
   set(MYSQL_LIBRARY_DIR ${MYSQL_LIB_DIR})
   message(STATUS "Found MYSQL library in ${MYSQL_LIBRARIES}")		
else()
   message(STATUS "MYSQL library not found; try to set a MYSQLROOT environment variable to the base installation path or add -DMYSQLROOT= to the cmake command")	
endif()

