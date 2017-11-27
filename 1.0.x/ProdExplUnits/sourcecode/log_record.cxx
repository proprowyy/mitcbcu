/*
 * Copyright (c) 2012，MIT
 * All right reserved.
 *
 * 文件名称： log_record.c
 * 文件标识：
 * 摘    要：The source file of log record
 *
 * 当前版本： V1.1.0
 * 作    者： wilson
 * 完成日期：2013-01-11
 *
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 */

#include "log_record.h"
#include "bcu_function.h"

struct timeval tp_log;

/*show the detail of cmd information*/
int DisplayNetworkCmd(network_send_package_t  param_cmd_info)
{
#if 1
	debug_print(("destination device:%s-%d\n",param_cmd_info.dst_devices_name,param_cmd_info.dst_devices_no));
	debug_print(("source device:%s%d\n",param_cmd_info.send_information.src_devices_name,param_cmd_info.send_information.src_devices_no));

	debug_print(("event_type_ann = %d\n",param_cmd_info.send_information.event_type_ann));
	debug_print(("event_type_intercom = %d\n",param_cmd_info.send_information.event_type_intercom));
	debug_print(("event_ptt_state = %d\n",param_cmd_info.send_information.event_ptt_state));

	if(param_cmd_info.send_information.event_type_ann == MANUAL_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  manual ann:package number %d\n",param_cmd_info.send_information.sequence));
		debug_print(("manual_active = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.manual_active));
		debug_print(("manual_begin_or_over = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.manual_begin_or_over));
		debug_print(("manual_pre_read = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.manual_pre_read));

       debug_print(("manual_response = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.manual_response));
		debug_print(("line_number = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.line_number));
		debug_print(("current_station_code = %d,\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.current_station_code));
		debug_print(("door_side = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.door_side));
		debug_print(("stop_or_leave = %d\n",param_cmd_info.send_information.event_info_ann.manual_annnounce.stop_or_leave));
	}
	else if(param_cmd_info.send_information.event_type_ann == EMERG_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  emerg ann:package number %d\n",param_cmd_info.send_information.sequence));
		debug_print(("emerg_active = %d\n",param_cmd_info.send_information.event_info_ann.emerg_announce.emerg_active));
		debug_print(("emerg_begin_or_over = %d\n",param_cmd_info.send_information.event_info_ann.emerg_announce.emerg_begin_or_over));
		debug_print(("emerg_response = %d\n",param_cmd_info.send_information.event_info_ann.emerg_announce.emerg_response));
		debug_print(("emerg_sequence = %d\n",param_cmd_info.send_information.event_info_ann.emerg_announce.emerg_sequence));
		debug_print(("emerg_times = %d\n",param_cmd_info.send_information.event_info_ann.emerg_announce.emerg_times));
	}
	else if(param_cmd_info.send_information.event_type_ann == LIVE_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  live ann:package number %d\n",param_cmd_info.send_information.sequence));
		debug_print(("live_active = %d\n",param_cmd_info.send_information.event_info_ann.live_announce.live_active));
		debug_print(("live_begin_or_over = %d\n",param_cmd_info.send_information.event_info_ann.live_announce.live_begin_or_over));
		debug_print(("live_response = %d\n",param_cmd_info.send_information.event_info_ann.live_announce.live_response));
	}
	else if(param_cmd_info.send_information.event_type_ann == TMS_AUTO_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  tms auto ann:package number %d\n",param_cmd_info.send_information.sequence));
		debug_print(("tms_auto_active = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.tms_auto_active));
		debug_print(("tms_auto_begin_or_over = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.tms_auto_begin_or_over));
       debug_print(("tms_auto_response = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.tms_auto_response));
		debug_print(("line_number = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.line_number));
		debug_print(("current_station_code = %d,\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.current_station_code));
		debug_print(("door_side = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.door_side));
		debug_print(("stop_or_leave = %d\n",param_cmd_info.send_information.event_info_ann.tms_auto_announce.stop_or_leave));
	}
	else if(param_cmd_info.send_information.event_type_ann == LINE_NUMBER_CHANGE_EVENT)
	{
		debug_print(("get_or_set = %d\n",param_cmd_info.send_information.event_info_ann.line_info.get_or_set));
		debug_print(("line_no = %d\n",param_cmd_info.send_information.event_info_ann.line_info.line_no));
		debug_print(("region_no = %d\n",param_cmd_info.send_information.event_info_ann.line_info.region_no));
		debug_print(("region_update_type = %d\n",param_cmd_info.send_information.event_info_ann.line_info.region_update_type));
		debug_print(("start_station_no = %d\n",param_cmd_info.send_information.event_info_ann.line_info.start_station_no));
		debug_print(("terminal_station_no = %d\n",param_cmd_info.send_information.event_info_ann.line_info.terminal_station_no));
		debug_print(("up_or_down = %d\n",param_cmd_info.send_information.event_info_ann.line_info.up_or_down));
		debug_print(("whether_the_cycle = %d\n",param_cmd_info.send_information.event_info_ann.line_info.whether_the_cycle));
	}
	else if(param_cmd_info.send_information.event_type_ann == ANN_IDLE)
	{
		if(param_cmd_info.send_information.event_type_intercom == D2P_INTERCOMM_EVENT)
		{
			debug_print(("D2P active = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_active));
			debug_print(("D2P dst bcu no = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no));
			debug_print(("D2P dst pcu no = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no));
			debug_print(("D2P pending = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_pending));
			debug_print(("D2P request or over = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over));
			debug_print(("D2P response = %d\n",param_cmd_info.send_information.event_info_intercom.d2p_intercomm.d2p_intercomm_response));
		}
		else if(param_cmd_info.send_information.event_type_intercom == D2D_INTERCOMM_EVENT)
		{
			debug_print(("D2D active = %d\n",param_cmd_info.send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_active));
			debug_print(("D2D dst bcu no = %d\n",param_cmd_info.send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no));
			debug_print(("D2D ppt state = %d\n",param_cmd_info.send_information.event_info_intercom.d2d_intercomm.d2d_ppt_state));
			debug_print(("D2D request or over = %d\n",param_cmd_info.send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over));
			debug_print(("D2D response = %d\n",param_cmd_info.send_information.event_info_intercom.d2d_intercomm.d2d_intercomm_response));
		}
		else
		{
			debug_print(("Debug cmd info:  idle ann:package number %d\n",param_cmd_info.send_information.sequence));
		}
	}
	else
	{
		debug_print(("This is a error package\n"));
	}
#endif
	return 0;
}

/*show the detail of cmd information*/
int DisplaySendCmd(send_infomation_t  param_cmd_info)
{
#if 1
//	debug_print(("event_type_ann = %s\n",mapping_table[param_cmd_info.event_type_ann].event_name));
	//debug_print(("event_type_intercom = %s\n",mapping_table[param_cmd_info.event_type_intercom].event_name));
	debug_print(("event_type_line_no = %d\n",param_cmd_info.event_info_ann.line_info.line_no));
	debug_print(("event_ptt_state = %d\n",param_cmd_info.event_ptt_state));
	debug_print(("package nu  = %d\n",param_cmd_info.sequence));
	if(param_cmd_info.event_type_ann == SKIP_STATION)
	{
		debug_print(("Debug cmd info:  skip station\n"));
		debug_print(("Debug cmd info:  skip station:line_number %d\n",param_cmd_info.event_info_ann.station_information.line_number));
		debug_print(("Debug cmd info:  skip station:station_no %d\n",param_cmd_info.event_info_ann.station_information.station_no));
		debug_print(("Debug cmd info:  skip station:effective %d\n",param_cmd_info.event_info_ann.station_information.effective));
	}
	else if(param_cmd_info.event_type_ann == MANUAL_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  manual ann:package number %d\n",param_cmd_info.sequence));
		debug_print(("manual_active = %d\n",param_cmd_info.event_info_ann.manual_annnounce.manual_active));
		debug_print(("manual_begin_or_over = %d\n",param_cmd_info.event_info_ann.manual_annnounce.manual_begin_or_over));
		debug_print(("manual_pre_read = %d\n",param_cmd_info.event_info_ann.manual_annnounce.manual_pre_read));

       debug_print(("manual_response = %d\n",param_cmd_info.event_info_ann.manual_annnounce.manual_response));
		debug_print(("line_number = %d\n",param_cmd_info.event_info_ann.manual_annnounce.line_number));
		debug_print(("current_station_code = %d,\n",param_cmd_info.event_info_ann.manual_annnounce.current_station_code));
		debug_print(("door_side = %d\n",param_cmd_info.event_info_ann.manual_annnounce.door_side));
		debug_print(("stop_or_leave = %d\n",param_cmd_info.event_info_ann.manual_annnounce.stop_or_leave));
		debug_print(("start_station_code = %d\n",param_cmd_info.event_info_ann.manual_annnounce.start_station_code));
		debug_print(("end_station_code = %d\n",param_cmd_info.event_info_ann.manual_annnounce.end_station_code));
	}
	else if(param_cmd_info.event_type_ann == EMERG_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  emerg ann:package number %d\n",param_cmd_info.sequence));
		debug_print(("emerg_active = %d\n",param_cmd_info.event_info_ann.emerg_announce.emerg_active));
		debug_print(("emerg_begin_or_over = %d\n",param_cmd_info.event_info_ann.emerg_announce.emerg_begin_or_over));
		debug_print(("emerg_response = %d\n",param_cmd_info.event_info_ann.emerg_announce.emerg_response));
		debug_print(("emerg_sequence = %d\n",param_cmd_info.event_info_ann.emerg_announce.emerg_sequence));
		debug_print(("emerg_times = %d\n",param_cmd_info.event_info_ann.emerg_announce.emerg_times));
	}
	else if(param_cmd_info.event_type_ann == LIVE_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  live ann:package number %d\n",param_cmd_info.sequence));
		debug_print(("live_active = %d\n",param_cmd_info.event_info_ann.live_announce.live_active));
		debug_print(("live_begin_or_over = %d\n",param_cmd_info.event_info_ann.live_announce.live_begin_or_over));
		debug_print(("live_response = %d\n",param_cmd_info.event_info_ann.live_announce.live_response));
	}
	else if(param_cmd_info.event_type_ann == TMS_AUTO_ANN_EVENT)
	{
		debug_print(("Debug cmd info:  tms auto ann:package number %d\n",param_cmd_info.sequence));
		debug_print(("tms_auto_active = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.tms_auto_active));
		debug_print(("tms_auto_begin_or_over = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.tms_auto_begin_or_over));
       debug_print(("tms_auto_response = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.tms_auto_response));
		debug_print(("line_number = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.line_number));
		debug_print(("current_station_code = %d,\n",param_cmd_info.event_info_ann.tms_auto_announce.current_station_code));
		debug_print(("door_side = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.door_side));
		debug_print(("stop_or_leave = %d\n",param_cmd_info.event_info_ann.tms_auto_announce.stop_or_leave));
	}
	else if(param_cmd_info.event_type_ann == ANN_IDLE)
	{
		if(param_cmd_info.event_type_intercom == D2P_INTERCOMM_EVENT)
		{
			debug_print(("D2P active = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_active));
			debug_print(("D2P dst bcu no = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_bcu_device_no));
			debug_print(("D2P dst pcu no = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_pcu_device_no));
			debug_print(("D2P pending = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_pending));
			debug_print(("D2P request or over = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_request_or_over));
			debug_print(("D2P response = %d\n",param_cmd_info.event_info_intercom.d2p_intercomm.d2p_intercomm_response));
		}
		else if(param_cmd_info.event_type_intercom == D2D_INTERCOMM_EVENT)
		{
			debug_print(("D2D active = %d\n",param_cmd_info.event_info_intercom.d2d_intercomm.d2d_intercomm_active));
			debug_print(("D2D dst bcu no = %d\n",param_cmd_info.event_info_intercom.d2d_intercomm.d2d_intercomm_bcu_device_no));
			debug_print(("D2D ppt state = %d\n",param_cmd_info.event_info_intercom.d2d_intercomm.d2d_ppt_state));
			debug_print(("D2D request or over = %d\n",param_cmd_info.event_info_intercom.d2d_intercomm.d2d_intercomm_request_or_over));
			debug_print(("D2D response = %d\n",param_cmd_info.event_info_intercom.d2d_intercomm.d2d_intercomm_response));
		}
		else
		{
			debug_print(("Debug cmd info:  idle ann:package number %d\n",param_cmd_info.sequence));
		}
	}
	else
	{
		debug_print(("This is a error package\n"));
	}
#endif
	return 0;
}

void WriteNetworkErrorToLog(char *info)
{
      return ;
}

