/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称：data_struct.c
 * 文件标识：
 * 摘    要：控制信息结构体
 *
 * 当前版本：V1.0.0
 * 作    者：wilson
 * 完成日期：
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */
#include "data_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcu_function.h"

//触摸屏填充的结构体
//send_infomation_t send_info_screen_to_control =
//{
//		BCUSendInfoInit,
//		BCUSetInformation,
//		"0",
//		0,
//		0,
//		0,
//		{{0}},
//		0,
//		{{0}},
//		0,
//		0
//
//};

//网络发送数据包
volatile network_send_package_t network_send_package =
{
		//NetworkSendInfoFill,
		"",
		0,
		0,
		{0}
};

////控制信息（发送结构体）初始化
//void BCUSendInfoInit(void)
//{
//	send_info_screen_to_control.src_devices_no = 0x00;
//	strcpy((char *)send_info_screen_to_control.src_devices_name,"");
//	memset((void *)&send_info_screen_to_control.event_info_ann,0,sizeof(send_info_screen_to_control.event_info_ann));
//	memset((void *)&send_info_screen_to_control.event_info_intercom,0,sizeof(send_info_screen_to_control.event_info_intercom));
//	send_info_screen_to_control.sequence = 0;
//}


////控制信息（发送结构体）填充
////入口参数
////[0]事件类型     [1]源设备名称   [2]源设备号    [3]状态请求结束   [4]线路号
////[5]文件序列号   [6]播报次数     [7]是否到站    [8]站代码         [9]对讲状态下的目标设备号
//void BCUSetInformation(unsigned char event_type,char *src_devices_name,unsigned char src_devices_no,unsigned char state_request,unsigned char request_or_over,
//					  unsigned char line_number,unsigned int broadcast_sequence,unsigned char broadcast_times,
//		              unsigned char arrived_station,unsigned char current_station_code,int dst_device_number)
//{
//	int current_event = (int)event_type;
//	strcpy((char *)send_info_screen_to_control.src_devices_name ,src_devices_name);
//	send_info_screen_to_control.src_devices_no = src_devices_no;
//
//	send_info_screen_to_control.event_ptt_state = 0;//wilson 2012-12-29
//
//
//	if(current_event == OCC_EVENT || current_event == LIVE_ANN_EVENT || current_event == EMERG_ANN_EVENT ||
//	   current_event == EMERG_ANN_EVENT || current_event == MANUAL_ANN_EVENT ||
//	   current_event == TMS_AUTO_ANN_EVENT || current_event == SKIP_STATION)
//	{
//		send_info_screen_to_control.event_type_ann = event_type;
//
//		send_info_screen_to_control.event_type_intercom = INTERCOM_IDLE;
//	}
//	else if(current_event == D2P_INTERCOMM_EVENT || current_event == D2D_INTERCOMM_EVENT || current_event == D2D_HANGUP_D2P_EVENT)
//	{
//		send_info_screen_to_control.event_type_intercom = event_type;
//		send_info_screen_to_control.event_type_ann = ANN_IDLE;
//	}
//	else
//	{
//		send_info_screen_to_control.event_type_ann = ANN_IDLE;
//		send_info_screen_to_control.event_type_intercom = INTERCOM_IDLE;
//	}
//	switch(current_event)
//	{
//			case SKIP_STATION:
//				send_info_screen_to_control.station_information.line_number = line_number;
//				send_info_screen_to_control.station_information.station_no = current_station_code;
//				send_info_screen_to_control.station_information.effective = state_request;
//				break;
//			case TMS_AUTO_ANN_EVENT:
//				send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_active = state_request;
//				send_info_screen_to_control.event_info_ann.tms_auto_announce.line_number= line_number;
//				send_info_screen_to_control.event_info_ann.tms_auto_announce.current_station_code = current_station_code;
//				send_info_screen_to_control.event_info_ann.tms_auto_announce.stop_or_leave = arrived_station;
//				if(request_or_over == 0)
//				{
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_pre_read = 0;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_begin_or_over  = 0;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_response = 0;
//				}
//				else if(request_or_over == 1)
//				{
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_pre_read = 0;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_begin_or_over  = 1;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_response = 0;
//				}
//				else if(request_or_over == 2)
//				{
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_pre_read = 1;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_begin_or_over  = 0;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_response = 0;
//				}
//				else
//				{
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_pre_read = 1;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_begin_or_over  = 1;
//					send_info_screen_to_control.event_info_ann.tms_auto_announce.tms_auto_response = 0;
//				}
//				break;
//
//		case OCC_EVENT:
//			send_info_screen_to_control.event_info_ann.occ_announce.occ_active = state_request;
//			send_info_screen_to_control.event_info_ann.occ_announce.occ_begin_or_over = request_or_over;
//			send_info_screen_to_control.event_info_ann.occ_announce.occ_response = 0;
//			break;
//		case LIVE_ANN_EVENT:
//			send_info_screen_to_control.event_info_ann.live_announce.live_active = state_request;
//			send_info_screen_to_control.event_info_ann.live_announce.live_begin_or_over = request_or_over;
//			send_info_screen_to_control.event_info_ann.live_announce.live_response = 0;
//			break;
//		case EMERG_ANN_EVENT:
//			send_info_screen_to_control.event_info_ann.emerg_announce.emerg_active = state_request;
//			send_info_screen_to_control.event_info_ann.emerg_announce.emerg_response = 0;
//			if(request_or_over == 0)
//			{
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_begin_or_over = 0;
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_sequence = broadcast_sequence;
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_times = broadcast_times;
//			}
//			else
//			{
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_begin_or_over = 1;
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_sequence = 0;
//				send_info_screen_to_control.event_info_ann.emerg_announce.emerg_times = 0;
//			}
//			break;
//		case MANUAL_ANN_EVENT:
//			send_info_screen_to_control.event_info_ann.manual_annnounce.manual_active = state_request;
//			send_info_screen_to_control.event_info_ann.manual_annnounce.line_number= line_number;
//			send_info_screen_to_control.event_info_ann.manual_annnounce.current_station_code = current_station_code;
//			send_info_screen_to_control.event_info_ann.manual_annnounce.manual_response = 0;
//			if(request_or_over == 0)//开始广播
//			{
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_pre_read = 0;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_begin_or_over = 0;
//
//				send_info_screen_to_control.event_info_ann.manual_annnounce.current_station_code = current_station_code;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.stop_or_leave = arrived_station;
//			}
//			else if(request_or_over == 1)//结束广播
//			{
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_pre_read = 0;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_begin_or_over = 1;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.current_station_code = 0;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.stop_or_leave = 0;
//			}
//			else if(request_or_over == 2)//预读（临时用此来代替）
//			{
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_pre_read = 1;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_begin_or_over = 0;
//			}
//			else
//			{
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_pre_read = 1;
//				send_info_screen_to_control.event_info_ann.manual_annnounce.manual_begin_or_over = 1;
//		   }
//			break;
//		case D2D_INTERCOMM_EVENT:
//			send_info_screen_to_control.event_info_intercom.d2d_intercomm.d2d_intercomm_active = state_request;
//			if(request_or_over == 0)
//			{
//				send_info_screen_to_control.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 0;
//				send_info_screen_to_control.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no = dst_device_number;
//			}
//			else
//			{
//				send_info_screen_to_control.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over = 1;
//			}
//			break;
//		case D2P_INTERCOMM_EVENT:
//			send_info_screen_to_control.event_info_intercom.d2p_intercomm.d2p_intercomm_active = state_request;
//			send_info_screen_to_control.event_info_intercom.d2p_intercomm.d2p_intercomm_response = 0;
//
//			send_info_screen_to_control.event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over = request_or_over;
//			send_info_screen_to_control.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no = dst_device_number;
//			send_info_screen_to_control.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no = bcu_state.bcu_info.devices_no;
//
//			break;
//
//		case LINE_NUMBER_CHANGE_EVENT:
//			printf("LINE_NUMBER_CHANGE_EVENT: %d\n",line_number);
//			send_info_screen_to_control.event_type_line_no = line_number;
//			break;
//		default:
//			break;
//	}
//}

//判断设备类型
int GetDeviceType(char *devices_name)
{
	int ret = 0;
	if( strcmp(devices_name,"BCU") == 0)
	{
		ret = TYPE_OF_BCU;
	}

	if(strcmp(devices_name,"CCU") == 0)
	{
		ret = TYPE_OF_CCU;
	}

	if(strcmp(devices_name,"EMAP") == 0)
	{
		ret = TYPE_OF_EAMP;
	}

	if(strcmp(devices_name,"PCU") == 0)
	{
		ret = TYPE_OF_PECU;
	}
	return ret;
}
