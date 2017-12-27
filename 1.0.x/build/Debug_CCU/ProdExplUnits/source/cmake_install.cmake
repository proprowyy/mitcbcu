# Install script for directory: /home/benty/workspace/Projects/ProdCCU/1.0.x/ProdExplUnits/source

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/srv/tftpboot/ccu/")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE EXECUTABLE FILES "/home/benty/workspace/Projects/ProdCCU/1.0.x/build/Debug_CCU/ProdExplUnits/source/ProdTest.elf")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest.elf" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest.elf")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/opt/ecos/gnutools/arm-eabi/bin/arm-eabi-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./ProdTest.elf")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
	EXECUTE_PROCESS(COMMAND rm  ProdTest_CCU.srec ProdTest_CCU.bin  ProdTest_CCU.img ProdTest_CCU.elf WORKING_DIRECTORY /srv/tftpboot/ccu//)	
#	EXECUTE_PROCESS(COMMAND rm ProdTest_CCU.srec ProdTest_CCU.bin  ProdTest_CCU.img ProdTest_CCU.elf   WORKING_DIRECTORY  /home/benty/workspace/Products/CCU/)	

	
	EXECUTE_PROCESS(COMMAND /opt/ecos/gnutools/arm-eabi/bin/arm-eabi-objcopy -O  binary ProdTest.elf ProdTest_CCU.bin WORKING_DIRECTORY /srv/tftpboot/ccu//)
	EXECUTE_PROCESS(COMMAND /opt/ecos/gnutools/arm-eabi/bin/arm-eabi-objcopy -O srec ProdTest.elf ProdTest_CCU.srec WORKING_DIRECTORY /srv/tftpboot/ccu//)
	EXECUTE_PROCESS(COMMAND cp	ProdTest.elf ./ProdTest_CCU.elf WORKING_DIRECTORY /srv/tftpboot/ccu//)
#	EXECUTE_PROCESS(COMMAND	dd if=/dev/zero of=ProdTest_CCU.img bs=512 count=2880 WORKING_DIRECTORY /srv/tftpboot/ccu//)
#	EXECUTE_PROCESS(COMMAND dd if=ProdTest_CCU.bin of=ProdTest_CCU.img bs=512 conv=notrunc WORKING_DIRECTORY /srv/tftpboot/ccu//)
	EXECUTE_PROCESS(COMMAND cp  ProdTest_CCU.srec ProdTest_CCU.bin  ProdTest_CCU.img ProdTest_CCU.elf /home/benty/workspace/Products/CCU/   WORKING_DIRECTORY /srv/tftpboot/ccu//)
#	EXECUTE_PROCESS(COMMAND rm ProdTest.elf WORKING_DIRECTORY /srv/tftpboot/ccu//)	

endif()

