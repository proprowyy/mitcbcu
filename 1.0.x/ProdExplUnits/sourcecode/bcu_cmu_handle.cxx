/*
 * bcu_cmu_handle.cxx
 *
 *  Created on: 2015-2-11
 *      Author: wilson
 */
#include "bcu_cmu_handle.h"


void BCUCMUCarTypeHandle(common_package_t common_package_info)
{
	last_control_flag = control_flag;control_flag = 300;
	diag_printf("COMMON_PACKAGE_TYPE_CMU_CAR_TYPE = %d,%d\n",COMMON_PACKAGE_TYPE_CMU_CAR_TYPE,
			common_package_info.pkg_type);
	if(common_package_info.pkg_type ==COMMON_PACKAGE_TYPE_CMU_CAR_TYPE)
	{
		last_control_flag = control_flag;control_flag = 301;
		diag_printf("##-%d-%d\n",common_package_info.common_data_u.cmu_car_type.old_car_line_no,
				common_package_info.common_data_u.cmu_car_type.state);
		if(common_package_info.common_data_u.cmu_car_type.old_car_line_no == 0)
		{
			last_control_flag = control_flag;control_flag = 302;
			AlarmTSToChangeScreen(35);
			last_control_flag = control_flag;control_flag = 303;
		}
		else if(common_package_info.common_data_u.cmu_car_type.old_car_line_no == 3)
		{///老三车
			last_control_flag = control_flag;control_flag = 304;
			AlarmTSToChangeScreen(36);
			last_control_flag = control_flag;control_flag = 305;
		}
		else if(common_package_info.common_data_u.cmu_car_type.old_car_line_no == 4)
		{///老四车
			last_control_flag = control_flag;control_flag = 306;
			AlarmTSToChangeScreen(37);
			last_control_flag = control_flag;control_flag = 307;
		}
		last_control_flag = control_flag;control_flag = 308;
	}
	last_control_flag = control_flag;control_flag = 309;
}

void BCUCMUACKStateHandle(common_package_t common_package_info)
{
	last_control_flag = control_flag;control_flag = 310;
	if(common_package_info.pkg_type == COMMON_PACKAGE_TYPE_ACK_STATE )
	{
		last_control_flag = control_flag;control_flag = 311;
		bcu_state.bcu_mcu_connect_state = common_package_info.common_data_u.cmu_ack_state.ack_state;
		if(bcu_state.bcu_mcu_connect_state == 1)
		{
			SetAudioDataDestination("CMU",232);
		}
		diag_printf("3:bcu_state.bcu_mcu_connect_state = %d\n",bcu_state.bcu_mcu_connect_state);
		last_control_flag = control_flag;control_flag = 312;
	}
	last_control_flag = control_flag;control_flag = 317;
}


void BCUCMUConnectionCatTypeAndAckState()
{
	common_package_t bcu_cmu_comminication_info;
	if(BlockBufferRead(bcu_state.bcu_cmu_car_type_buffer_id,&bcu_cmu_comminication_info,sizeof(bcu_cmu_comminication_info)) > 0 &&
			BCUGetDataFromCCUState() == 3)
	{///连挂车型处理函数
		last_control_flag = control_flag;control_flag = 323;
		BCUCMUCarTypeHandle(bcu_cmu_comminication_info);
		last_control_flag = control_flag;control_flag = 324;
	}
	if(BlockBufferRead(bcu_state.bcu_cmu_ack_state_buffer_id,&bcu_cmu_comminication_info,sizeof(bcu_cmu_comminication_info)) > 0 &&
			BCUGetDataFromCCUState() == 3)
	{///连挂ack处理函数
		last_control_flag = control_flag;control_flag = 325;
		BCUCMUACKStateHandle(bcu_cmu_comminication_info);
		last_control_flag = control_flag;control_flag = 326;
	}
}
