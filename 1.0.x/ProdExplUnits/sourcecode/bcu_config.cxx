/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： bcu_config.c
 * 文件标识：
 * 摘    要：The source file of current bcu configure
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-09
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#include "bcu_config.h"
#include "bcu_function.h"
#include "network/zhw_dev_config_info.h"
extern bcu_state_t  bcu_state;

/*get current device name id and car number */
int GetCurrentDeviceInfomation(device_id_info_t *this_device_id_info)
{
#if 1
	struct dev_type_and_no temp_dev_type_and_no;
	struct global_info temp_global_info;

	temp_dev_type_and_no = ReadFromIO();
	temp_global_info = ReadGlobalArray(temp_dev_type_and_no.dev_type,NULL,temp_dev_type_and_no.dev_number,1);
	strcpy((char *)this_device_id_info->device_name,(char *) temp_global_info.dev_name);
	this_device_id_info->device_no = temp_dev_type_and_no.dev_number;
	this_device_id_info->dev_type = temp_dev_type_and_no.dev_type;
#else
	strcpy((char *)this_device_id_info->device_name,"BCU");
	this_device_id_info->device_no = ReadNum();
	this_device_id_info->dev_type = ReadType();
#endif
	return 0;
}

/*Acquire other device number*/
void GetOtherDeviceNo(device_id_info_t this_device_id_info)
{
	if(this_device_id_info.device_no == 1)
	{
		bcu_state.opposite_bcu_no = 2;
	}
	else
	{
		bcu_state.opposite_bcu_no = 1;
	}
}

/*Update current device information*/
void UpdateCurrentDeviceInformation(device_bcu_state_t *param_bcu_device_info,device_id_info_t param_bcu_device)
{
	param_bcu_device_info->devices_no =  param_bcu_device.device_no;
	strcpy( param_bcu_device_info->devices_name, param_bcu_device.device_name);
	bcu_state.dev_type = param_bcu_device.dev_type;
}



