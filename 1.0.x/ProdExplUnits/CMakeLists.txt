##### CMake input file to build ProdExplUnits

#include ose library
INCLUDE_DIRECTORIES( ${ECOS_INCLUDE}/include )
INCLUDE_DIRECTORIES( ${BASICLIBINCLUDE}/LibUtilities/include )
INCLUDE_DIRECTORIES( ${BASICLIBINCLUDE}/LibModules/include )
INCLUDE_DIRECTORIES( ${BASICLIBINCLUDE}/LibData/include )

INCLUDE_DIRECTORIES( ${PROD_EXPL_SOURCE_DIR}/ProdExplLib/include )

# Make sure the linker can find the libraries once they are built.
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)
#LINK_DIRECTORIES(${BASICLIBLIBRARY}/LibUtilities)
#LINK_DIRECTORIES(${BASICLIBLIBRARY}/LibModules)
LINK_DIRECTORIES(${ECOS_LIBRARY}/include)
LINK_DIRECTORIES(${ECOS_LIBRARY}/lib)

#AUX_SOURCE_DIRECTORY( . PROD_EXPL_UNITS_SRCS )
SET(PROD_EXPL_UNITS_SRCS
 sourcecode/alarm_pcu_audio.cxx
 sourcecode/alarm_audio_data.cxx
 sourcecode/bcu_function.cxx
 sourcecode/bcu_main.cxx
 sourcecode/thread_config.cxx
 sourcecode/bcu_config.cxx
 sourcecode/buffer_read_write_fun.cxx
 sourcecode/sound_driver.cxx
  sourcecode/log_record.cxx
 sourcecode/audio_sample.cxx
  sourcecode/thread_comm.cxx
  sourcecode/bcu_ann.cxx
  sourcecode/bcu_intercom.cxx
 sourcecode/alarm_list_fun.cxx
 sourcecode/data_struct_bcu.cxx
  sourcecode/bcu_state_machine.cxx
 sourcecode/bcu_update_table_from_ccu.cxx
  sourcecode/bcu_gd_sync.cxx
 sourcecode/bcu_extern_button.cxx
  sourcecode/function_for_network.cxx
 sourcecode/bcu_cmu_handle.cxx
 sourcecode/bcu_failure_handle.cxx
 sourcecode/TouchScreen_horizontal.cxx
 sourcecode/tftp_server_apply.cxx
 sourcecode/tftp_server_fun.cxx
  
 	sourcecode/network/zhw_shell_function.cxx  
	sourcecode/network/zhw_buffer_init.cxx
	sourcecode/network/zhw_dev_config_info.cxx
	sourcecode/network/zhw_dev_selftest_module.cxx
	sourcecode/network/zhw_middle_forward.cxx
	sourcecode/network/zhw_mount_romfs.cxx
	sourcecode/network/zhw_network_main.cxx
	sourcecode/network/zhw_read_from_db_test.cxx
	sourcecode/network/zhw_set_into_mul.cxx
	sourcecode/network/zhw_socket_init.cxx
	sourcecode/network/zhw_sockfd_server_init.cxx
	sourcecode/network/zhw_use_ioctl_to_set_net.cxx
	sourcecode/network/zhw_apply_common_func.cxx
	sourcecode/network/add_ip_to_mul.cxx
	sourcecode/network/udp_recv_fun.cxx
	sourcecode/network/udp_send_fun.cxx
	sourcecode/network/zhw_bcu_to_cmu.cxx
	
) 


# Strip release build
IF( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
  SET( CMAKE_EXE_LINKER_FLAGS  "-Wl,-strip-all")
ENDIF( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

ADD_EXECUTABLE(ProdTest ${PROD_EXPL_UNITS_SRCS})

# Link the executable to the libraries.
TARGET_LINK_LIBRARIES(ProdTest
#  ${CMAKE_INSTALL_PREFIX}/lib/LibModules.a
)
TARGET_LINK_LIBRARIES(ProdTest  target )

#ININIIADHIAHDANDIANIDANI

INSTALL( TARGETS ProdTest  RUNTIME DESTINATION ./ )

INSTALL( CODE "
	EXECUTE_PROCESS(COMMAND rm  ProdTest_${BUILD}.srec ProdTest_${BUILD}.bin  ProdTest_${BUILD}.img ProdTest_${BUILD}.elf WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)	
#	EXECUTE_PROCESS(COMMAND rm ProdTest_${BUILD}.srec ProdTest_${BUILD}.bin  ProdTest_${BUILD}.img ProdTest_${BUILD}.elf   WORKING_DIRECTORY  /home/benty/workspace/Products/${BUILD}/)	

	
	EXECUTE_PROCESS(COMMAND ${CROSSTOOL}/${TOOLS_PREFIX}/bin/${TOOLS_PREFIX}-objcopy -O  binary ProdTest.elf ProdTest_${BUILD}.bin WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
	EXECUTE_PROCESS(COMMAND ${CROSSTOOL}/${TOOLS_PREFIX}/bin/${TOOLS_PREFIX}-objcopy -O srec ProdTest.elf ProdTest_${BUILD}.srec WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
	EXECUTE_PROCESS(COMMAND cp	ProdTest.elf ./ProdTest_${BUILD}.elf WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
#	EXECUTE_PROCESS(COMMAND	dd if=/dev/zero of=ProdTest_${BUILD}.img bs=512 count=2880 WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
#	EXECUTE_PROCESS(COMMAND dd if=ProdTest_${BUILD}.bin of=ProdTest_${BUILD}.img bs=512 conv=notrunc WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
	EXECUTE_PROCESS(COMMAND cp  ProdTest_${BUILD}.srec ProdTest_${BUILD}.bin  ProdTest_${BUILD}.img ProdTest_${BUILD}.elf /srv/tftpboot/cbcu/   WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)
#	EXECUTE_PROCESS(COMMAND rm ProdTest.elf WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/)	
")

