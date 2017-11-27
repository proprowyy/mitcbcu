/*
 * bcu_failure_handle.cxx
 *
 *  Created on: 2015-2-11
 *      Author: wilson
 */


#include "bcu_failure_handle.h"
unsigned char whe_change_dev_type_failure = 0;//相同类型设备故障信息所变化
void BCUDevFailureHandle()
{
	common_big_package_t all_dev_state_info;///<所有设备状态信息
	int ret = 0;
	if( BlockBufferRead(bcu_state.all_dev_state_buffer_id,&all_dev_state_info,sizeof(all_dev_state_info)) > 0 &&
					bcu_state.this_bcu_is_active == 1 && BCUGetDataFromCCUState() == 3)
	{}
}
