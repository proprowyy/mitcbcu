# CMake input file to build BasicLib2
 if(COMMAND cmake_policy)
     cmake_minimum_required(VERSION 2.6)
     cmake_policy(SET CMP0003 NEW)
 endif(COMMAND cmake_policy)

MESSAGE(STATUS "include CMakeGen.txt" )
  include ( ../../BasicLib/1.0.x/Base/CMakeGen.txt)

# Global settings and flags
SET( CMAKE_VERBECOS_MAKEFILE OFF)

#IF( ARCH STREQUAL "ARM-V7M")
#  SET( ECOS_INCLUDE ${BUILD_BASEDIR}/os/ecos/Shen/3.0.1/3.0.1_install )
#ENDIF()

IF(ARCH STREQUAL "i386_elf")
  SET( ECOS_INCLUDE ${BUILD_BASEDIR}/os/ecos/vmware/ecos_vm/3.0.x/3.0.1_install )
  #SET( ECOS_INCLUDE ${BUILD_BASEDIR}/os/ecos/vmware/3.0.1/test/3.0.2/3.0.2_install)
ENDIF()


SET( BASICLIB_INCLUDE ${BUILD_BASEDIR}/BasicLib/1.0.x/ )
SET( PROD_EXPL_SOURCE_DIR ${BUILD_BASEDIR}/ProdEAMP/1.0.x/ )

### setup debug/release libraries
SET( ECOS_LIBRARY ${ECOS_INCLUDE} )
SET( BASICLIB_LIBRARY ${BASICLIB_INCLUDE}/build/${CMAKE_BUILD_TYPE}_${BUILD} )



PROJECT(PROD_EXPL)
ADD_SUBDIRECTORY(ProdExplLib)
#ADD_SUBDIRECTORY(ProdExplDrivers)
ADD_SUBDIRECTORY(ProdExplUnits)

