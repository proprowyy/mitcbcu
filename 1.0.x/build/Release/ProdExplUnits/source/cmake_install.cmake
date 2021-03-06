# Install script for directory: /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE FILES "/home/benty/workspace/Projects/ProdExpl/1.0.x/build/Release_BCU/ProdExplUnits/source/ProdTest")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest")
    FILE(RPATH_REMOVE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
	EXECUTE_PROCESS(COMMAND rm ProdTest_1.0.x.srec ProdTest_1.0.x ProdTest  ProdTest_1.0.x.img ProdTest_1.0.x.elf  WORKING_DIRECTORY ~/workspace/Products/1.0.x/)	
	EXECUTE_PROCESS(COMMAND //bin/-objcopy -O  binary ProdTest ProdTest_1.0.x.bin WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND //bin/-objcopy -O srec ProdTest ProdTest_1.0.x.srec WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND cp	ProdTest ./ProdTest_1.0.x.elf WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND cp   ProdTest_1.0.x.srec /home/benty/workspace/Products/1.0.x/ WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND	dd if=/dev/zero of=ProdTest_1.0.x.img bs=512 count=2880 WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND dd if=ProdTest.bin of=ProdTest_1.0.x.img bs=512 conv=notrunc WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND cp  ProdTest_1.0.x.srec ProdTest_1.0.x ProdTest_1.0.x.img ProdTest_1.0.x.elf /home/benty/workspace/Products/1.0.x/   WORKING_DIRECTORY /usr/local/)
	EXECUTE_PROCESS(COMMAND rm ProdTest_1.0.x.srec ProdTest_1.0.x ProdTest  ProdTest_1.0.x.img ProdTest_1.0.x.elf WORKING_DIRECTORY /usr/local/)	


ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

