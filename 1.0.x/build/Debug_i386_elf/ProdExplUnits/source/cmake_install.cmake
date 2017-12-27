# Install script for directory: /home/benty/workspace/Projects/ProdExpl/1.0.x/ProdExplUnits/source

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/srv/tftpboot/i386_elf/")
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
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf"
         RPATH "")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE FILES "/home/benty/workspace/Projects/ProdExpl/1.0.x/build/Debug_i386_elf/ProdExplUnits/source/ProdTest_i386_elf")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf")
    FILE(RPATH_REMOVE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest_i386_elf")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
	#EXECUTE_PROCESS(COMMAND rm	ProdTest_i386_elf.srec ProdTest ProdTest_i386_elf.img ProdTest_i386_elf.elf WORKING_DIRECTORY /srv/tftpboot/i386_elf/)	
	EXECUTE_PROCESS(COMMAND /opt/ecos/gnutools/i386-elf/bin/i386-elf-objcopy -O  binary ProdTest ProdTest_i386_elf.bin WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND /opt/ecos/gnutools/i386-elf/bin/i386-elf-objcopy -O srec ProdTest ProdTest_i386_elf.srec WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND cp	ProdTest ./ProdTest_i386_elf.elf WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND cp   ProdTest_i386_elf.srec /home/benty/workspace/Products/i386_elf/ WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND	dd if=/dev/zero of=ProdTest_i386_elf.img bs=512 count=2880 WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND dd if=ProdTest.bin of=ProdTest_i386_elf.img bs=512 conv=notrunc WORKING_DIRECTORY /srv/tftpboot/i386_elf//)
	EXECUTE_PROCESS(COMMAND cp  ProdTest_i386_elf.srec ProdTest_i386_elf ProdTest_i386_elf.img ProdTest_i386_elf.elf /home/benty/workspace/Products/i386_elf/   WORKING_DIRECTORY /srv/tftpboot/i386_elf//)

ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

