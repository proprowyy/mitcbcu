/*
 * Copyright (c) 2013, MIT, All right reserved.
 *
 * File Name:	bcu_gd_sync.c
 * File Identification: 
 * Abstract:  
 *
 * Current Version:	V2013_0415_0.0.1
 * Author:	AlvinChang
 * Completion Date:	2013-04-16
 *
 * Last Version:
 * Last Author:
 * Finishing Date:
 */

#include "bcu_gd_sync.h"
#include "bcu_get_data_from_ccu_type.h"

//static global_device_info_state_t  *Bcu_gd_table =NULL;
static unsigned int send_package_serial_number = 0;

///< add begin, 1010
static unsigned int local_dev_number = 0;
unsigned char db_version_last_type = 0;
#if 0
static dev_version_table_t all_bcu_version_table[CONFIG_BCU_TOTAL];
static unsigned int all_bcu_version_table_items = CONFIG_BCU_TOTAL;
static dev_version_table_t all_ccu_version_table[CONFIG_CCU_TOTAL];
static unsigned int  all_ccu_version_table_items = CONFIG_CCU_TOTAL;
static dev_version_table_t all_eamp_version_table[CONFIG_EAMP_TOTAL];
static unsigned int  all_eamp_version_table_items = CONFIG_EAMP_TOTAL;
static dev_version_table_t all_pcu_version_table[CONFIG_PCU_TOTAL];
static unsigned int  all_pcu_version_table_items = CONFIG_PCU_TOTAL;
#endif
pthread_mutex_t mutex_dev_version_table;
///< add end, 1010


#if 0
int bcu_alarm_sync_waiting_timeout = -1;
cyg_handle_t   bcu_counter_handle_sync_waiting; //counter attached to alarm
cyg_handle_t   bcu_alarm_handle_sync_waiting;   //alarm handle
cyg_alarm   bcu_alarm_object_sync_waiting;
#endif

//extern pthread_mutex_t mutex_of_global_device_info;
extern int ReadType(void);
extern int ReadNumWilson(void);
extern char *zhw_networ_get_local_ip(void);
extern unsigned char BCUGetDataFromCCUState(void);
extern int update_start;

void BcuAlarmSyncWaiting(cyg_handle_t alarm_handle, cyg_addrword_t data)
{
        AlarmTimeOutProcess( (void *)data);

        //something else to do here

        return ;
}

int BcuCheckDriverKey(int device_type, int device_no)
{
	int ret = -1;

	if( device_type == 6 && device_no == 1 ) //BCU-1
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}

	return ret;
}
int BcuGdGetLocalDevIO(int *device_type, int *device_no)
{
	int  dev_type = -1;
	int  dev_no = -1;

        dev_type = ReadType();
        dev_no = ReadNumWilson();

	if( NULL != device_type)
	{
		*device_type = dev_type;
	}

	if( NULL != device_no)
	{
		*device_no = dev_no;
	}

      return 0;
}


int BcuSendDbSyncReqCommon(int cmd_tx_buffer_id, dev_config_table_item_t  *pdev_config_info, int sub_event, int dest_dev_no)
{
      int ret = -1;
      common_big_package_t  tmp_net_cmd = {"\0", 0};
      pa_common_information_t *pcomm_cmd_info = (pa_common_information_t *)&(tmp_net_cmd.common_big_data_u.pa_common_cmd_info);

      if( NULL== pdev_config_info )
      {
             return -1;
      }

      //we need server dev_name and dev_no
      //strcpy(tmp_net_cmd.dst_dev_name, pdev_config_info->db_server_dev_name);
      //tmp_net_cmd.dst_dev_number = pdev_config_info->db_server_dev_no;
      strcpy(tmp_net_cmd.dst_dev_name, "CCU");
      tmp_net_cmd.dst_dev_number = dest_dev_no; ///< MUL_SYNC_DST_NO;
      strcpy(tmp_net_cmd.src_dev_name, pdev_config_info->dev_name);
      tmp_net_cmd.src_dev_number = pdev_config_info->dev_no;
      tmp_net_cmd.pkg_type = COMMON_PACKAGE_TYPE_SYNC_GD;
      pcomm_cmd_info->event_type_common = EVENT_TYPE_SYS_UPDATE_SYNC;
      pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req = 1;
      pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res = 0;
      pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask = sub_event;
      pcomm_cmd_info->sequence = ++send_package_serial_number;

    debug_print(("BcuSendDbSyncReqCommon:  dest=%s-%d\n", tmp_net_cmd.dst_dev_name, tmp_net_cmd.dst_dev_number));
    debug_print(("BcuSendDbSyncReqCommon:  src=%s-%d, seq=%d \n", tmp_net_cmd.src_dev_name, tmp_net_cmd.src_dev_number, pcomm_cmd_info->sequence));
    ret = BlockBufferWrite(cmd_tx_buffer_id, (const void *)&tmp_net_cmd, sizeof(tmp_net_cmd));
    debug_print(("BcuSendDbSyncReqCommon:ret = %d\n",ret));
    return ret;
}
int BcuGdUpdateDevTftpStatus( common_big_package_t *pnet_recv_cmd )
{
	int ret = -1;
	void *recv_buffer = NULL;
	unsigned int recv_len = 0;
	unsigned char  subevent_type = 0;
        pa_common_information_t *pcomm_cmd_info = NULL;

	if( NULL == pnet_recv_cmd )
	{
		return -1;
	}
	pcomm_cmd_info = (pa_common_information_t *)&(pnet_recv_cmd->common_big_data_u.pa_common_cmd_info);
	recv_buffer = (void *)(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context);
	recv_len = pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context_len;
	subevent_type = pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask;
	debug_print(("BcuGdUpdateDevTftpStatus:  sub_evt=%d\n", subevent_type));

	switch( subevent_type )
	{
		case SUBEVENT_TYPE_PCU_UPDATED:
	        {

	        }
		break;
              
		case SUBEVENT_TYPE_PCU_UPDATING:
		{

		}		
		break;
              
		case SUBEVENT_TYPE_EAMP_UPDATED:
		{

		}			
		break;
              
		case SUBEVENT_TYPE_EAMP_UPDATING:
		{

		}
		break;
             
		default:
		break;
	  }
	
          return ret;
}
int BcuGdGetDbFromServerCcu( common_big_package_t *pnet_recv_cmd )
{
	int ret = -1;
	void *recv_buffer = NULL;
	unsigned int recv_len = 0;
	unsigned char  subevent_type = 0;
        pa_common_information_t *pcomm_cmd_info = NULL;

	if( NULL == pnet_recv_cmd )
	{
		return -1;
	}
	pcomm_cmd_info = (pa_common_information_t *)&(pnet_recv_cmd->common_big_data_u.pa_common_cmd_info);
	recv_buffer = (void *)(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context);
	recv_len = pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context_len;
	subevent_type = pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask;
	debug_print(("Bcu, GetDbFromCcu:  sub_evt=%d\n", subevent_type));

	switch( subevent_type )
	{
		case SUBEVENT_TYPE_STATION_BCU_TS_PASSWORD:
		{
//			diag_printf("SUBEVENT_TYPE_STATION_BCU_TS_PASSWORD\n");
			pu_ts_password_table_row_t *p = (pu_ts_password_table_row_t *)recv_buffer;
			if(  recv_len == sizeof(pu_ts_password_table_row_t) + 4 )
			{
//				diag_printf("BCU TS password - %s\n",p->current_row_info.ts_password);
				strcpy(bcu_state.device_volume.correct_password,p->current_row_info.ts_password);
			}
		}
		break;
		case SUBEVENT_TYPE_LINE_STATION_TABLE_SYNC:///<所有站代码
		{
			line_station_code_row_t  *pcurrent_station_row = (line_station_code_row_t  *)recv_buffer;
//			debug_print(("Bcu, line station:  recv_len=%d, len=%d, line=%d \n", recv_len, sizeof(line_station_code_row_t), pcurrent_station_row->line_no));
//			debug_print(("Bcu, line station:  total=%d, curr=%d\n", pcurrent_station_row->total_row, pcurrent_station_row->current_index));
//			debug_print(("Bcu, line station,row:  %d, %d, ", pcurrent_station_row->current_row_info.station_no, pcurrent_station_row->current_row_info.fix_across_station));
//			debug_print(("%d, %s, ", pcurrent_station_row->current_row_info.spec_open_door, pcurrent_station_row->current_row_info.station_name));
//			debug_print(("%d, %s, ", pcurrent_station_row->current_row_info.spec_open_door, pcurrent_station_row->current_row_info.station_name_english));
//			debug_print(("%s, %s \n", pcurrent_station_row->current_row_info.station_audio, pcurrent_station_row->current_row_info.station_display));
			if(  recv_len == sizeof(line_station_code_row_t) + 4 )
			{
				BcuUpdateLcdLineStationArray( pcurrent_station_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_LINE_SECTION_TABLE_SYNC:///<线路区间信息
		{
			line_section_row_t  *pcurrent_section_row = (line_section_row_t  *)recv_buffer;
			debug_print(("Bcu, line section:  recv_len=%d, len=%d, line=%d \n",
					recv_len,
					sizeof(line_section_row_t),
					pcurrent_section_row->line_no));
			debug_print(("Bcu, line section:  total=%d, curr=%d \n",
					pcurrent_section_row->total_row,
					pcurrent_section_row->current_index));
			debug_print(("Bcu, line section,row:  %d, %d, ",
					pcurrent_section_row->current_row_info.line_no,
					pcurrent_section_row->current_row_info.line_direction));
			debug_print(("%d, %s, ",
					pcurrent_section_row->current_row_info.spec_cycle,
					pcurrent_section_row->current_row_info.short_name));
			debug_print(("%s, %s, ",
					pcurrent_section_row->current_row_info.start_time,
					pcurrent_section_row->current_row_info.end_time));
			debug_print(("%d, %d, ",
					pcurrent_section_row->current_row_info.default_section,
					pcurrent_section_row->current_row_info.section_no));
			debug_print(("%d, %d \n",
					pcurrent_section_row->current_row_info.start_station,
					pcurrent_section_row->current_row_info.end_station));
			if(  recv_len == sizeof(line_section_row_t) + 4 )
			{
				BcuUpdateLcdLineRegionArray(pcurrent_section_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_DEFAULT_LINE_TABLE_SYNC: ///< TLine
		{
			default_line_table_row_t  *pdefault_line_table_row = (default_line_table_row_t  *)recv_buffer;
//			debug_print(("Bcu, default line:  recv_len=%d, len=%d\n", recv_len, sizeof(default_line_table_row_t)));
//			debug_print(("Bcu, default line:  total=%d, curr=%d \n", pdefault_line_table_row->total_row, pdefault_line_table_row->current_index));
//			debug_print(("Bcu, default line,row:  line=%d, default=%d \n", pdefault_line_table_row->current_row_info.line_no, pdefault_line_table_row->current_row_info.default_line));
			if(  recv_len == sizeof(default_line_table_row_t)+4 )
			{
				BcuUpdateLcdDefaultLineArray( pdefault_line_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_EMERG_TABLE_SYNC:
		{
			emerg_ann_table_row_t  *pemerg_ann_table_row = (emerg_ann_table_row_t  *)recv_buffer;
			debug_print(("Bcu, emerg table:  recv_len=%d, len=%d, line=%d \n", recv_len, sizeof(emerg_ann_table_row_t), pemerg_ann_table_row->line_no));
			debug_print(("Bcu, emerg table:  total=%d, curr=%d\n", pemerg_ann_table_row->total_row, pemerg_ann_table_row->current_index));
			debug_print(("Bcu, emerg table,row:  %d, %d, ", pemerg_ann_table_row->current_row_info.emerg_ann_id, pemerg_ann_table_row->current_row_info.line_no));
			debug_print(("%d, %s, ", pemerg_ann_table_row->current_row_info.repeat_times, pemerg_ann_table_row->current_row_info.audio_path));
			debug_print(("QC:%s, %s \n", pemerg_ann_table_row->current_row_info.display_message, pemerg_ann_table_row->current_row_info.emerg_ann_name));

			if(  recv_len == sizeof(emerg_ann_table_row_t)+4 )
			{
				BcuUpdateLcdEmergAnnArray( pemerg_ann_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_IP_TABLE_SYNC:
		{
			ip_mac_table_row_t  *pip_mac_table_row = (ip_mac_table_row_t  *)recv_buffer;
//			debug_print(("Bcu, ip table:  recv_len=%d, len=%d \n", recv_len, sizeof(ip_mac_table_row_t)));
//			debug_print(("Bcu, ip table:  total=%d, curr=%d \n", pip_mac_table_row->total_row, pip_mac_table_row->current_index));
//			debug_print(("Bcu, ip table,row:  %s, %d, ", pip_mac_table_row->current_row_info.device_name, pip_mac_table_row->current_row_info.device_no));
//			debug_print(("%s, %s, ", pip_mac_table_row->current_row_info.ip_address, pip_mac_table_row->current_row_info.mac_address));
//			debug_print(("%d \n", pip_mac_table_row->current_row_info.server_whether));

			if(  recv_len == sizeof(ip_mac_table_row_t)+4 )
			{
				BcuUpdateNetIpArray( pip_mac_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_ANN_VERSION_TABLE_SYNC:
		{
			ann_db_version_table_row_t  *pann_db_version_table_row = (ann_db_version_table_row_t  *)recv_buffer;
//			debug_print(("Bcu, ann version table:  recv_len=%d, len=%d \n", recv_len, sizeof(ann_db_version_table_row_t)));
//			debug_print(("Bcu, ann version table:  total=%d, curr=%d \n", pann_db_version_table_row->total_row, pann_db_version_table_row->current_index));
//			debug_print(("Bcu, ann version table,row: %d, %s, ", pann_db_version_table_row->current_row_info.version_id, pann_db_version_table_row->current_row_info.short_name));
//			debug_print(("%s, %s \n", pann_db_version_table_row->current_row_info.time, pann_db_version_table_row->current_row_info.current_version));
			
			if(  recv_len == sizeof(ann_db_version_table_row_t)+4 )
			{
				BcuUpdateAnnVersionArray( pann_db_version_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_CONFIG_VERSION_TABLE_SYNC:
		{
			config_db_version_table_row_t  *pconfig_db_version_table_row = (config_db_version_table_row_t  *)recv_buffer;
//			debug_print(("Bcu, config version table:  recv_len=%d, len=%d \n", recv_len, sizeof(config_db_version_table_row_t)));
//			debug_print(("Bcu, config version table:  total=%d, curr=%d \n", pconfig_db_version_table_row->total_row, pconfig_db_version_table_row->current_index));
//			debug_print(("Bcu, config version table,row:  %d, %s, ", pconfig_db_version_table_row->current_row_info.version_id, pconfig_db_version_table_row->current_row_info.short_name));
//			debug_print(("%s, %s \n", pconfig_db_version_table_row->current_row_info.time, pconfig_db_version_table_row->current_row_info.current_version));
			
			if(  recv_len == sizeof(config_db_version_table_row_t)+4 )
			{
				BcuUpdateConfigVersionArray( pconfig_db_version_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_PRIORITY_TABLE_SYNC:
		{
			priority_table_row_t  *pprior_table_row = (priority_table_row_t  *)recv_buffer;
//			debug_print(("Bcu, prior table:  recv_len=%d, len=%d \n", recv_len, sizeof(priority_table_row_t)));
//			debug_print(("Bcu, prior  table:  total=%d, curr=%d \n", pprior_table_row->total_row, pprior_table_row->current_index));
//			debug_print(("Bcu, prior table,row:  %d, %s \n", pprior_table_row->current_row_info.priority_id, pprior_table_row->current_row_info.event_type));

			if(  recv_len == sizeof(priority_table_row_t)+4 )
			{
				BcuUpdatePriorArray( pprior_table_row );
			}
		}
		break;
              
		case SUBEVENT_TYPE_DEV_NUMBER_TABLE_SYNC:
		{
			device_number_table_row_t  *pdev_number_table_row = (device_number_table_row_t  *)recv_buffer;
//			debug_print(("Bcu,  dev number table:  recv_len=%d, len=%d \n", recv_len, sizeof(device_number_table_row_t)));
//			debug_print(("Bcu, dev number table:  total=%d, curr=%d \n", pdev_number_table_row->total_row, pdev_number_table_row->current_index));
//			debug_print(("Bcu, dev number table,row:  %d, %s \n", pdev_number_table_row->current_row_info.device_number_count, pdev_number_table_row->current_row_info.device_name));
			
			if(  recv_len == sizeof(device_number_table_row_t)+4 )
			{
				//BcuUpdateDevNumberArray( pdev_number_table_row );
			}
		}
		break;
		
		case SUBEVENT_TYPE_RECORD_PATH_TABLE_SYNC:
		{
			record_path_table_row_t  *precord_path_table_row = (record_path_table_row_t  *)recv_buffer;
//			debug_print(("Bcu,  record path table:  recv_len=%d, len=%d \n", recv_len, sizeof(record_path_table_row_t)));
//			debug_print(("Bcu,  record path table:  total=%d, curr=%d \n", precord_path_table_row->total_row, precord_path_table_row->current_index));
//			debug_print(("Bcu,  record path table,row:  %s, %s \n", precord_path_table_row->current_row_info.even_type, precord_path_table_row->current_row_info.path_name));
//
			if(  recv_len == sizeof(record_path_table_row_t)+4 )
			{
				//BcuUpdateRecordPathArray( precord_path_table_row );
			}
		}
		break;

		case SUBEVENT_TYPE_LOG_PATH_TABLE_SYNC: ///< CCU don't need to send this table
		{
			log_path_table_row_t  *plog_path_table_row = (log_path_table_row_t  *)recv_buffer;
			debug_print(("Bcu,  log path table:  recv_len=%d, len=%d \n", recv_len, sizeof(device_number_table_row_t)));
			
			if(  recv_len == sizeof(log_path_table_row_t)+4 )
			{
				//BcuUpdateLogPathArray( plog_path_table_row );
			}
		}
		break;

		case SUBEVENT_TYPE_STATION_DOOR_SIDE_SYNC: ///< door side
		{
			int ix = 0;
			line_station_door_side_t  *pline_station_door_side = (line_station_door_side_t  *)recv_buffer;
//			debug_print(("Bcu,  door side:  recv_len=%d, len=%d \n", recv_len, sizeof(line_station_door_side_t)));
//			debug_print(("Bcu,  door side:  line=%d, section=%d \n", pline_station_door_side->line_no, pline_station_door_side->section_no));
//			debug_print(("Bcu,  door side:  %d, %d \n", pline_station_door_side->total_bytes, pline_station_door_side->array_items));
			for(ix=0; ix<pline_station_door_side->array_items; ix++)
			{
//				debug_print(("Bcu,  door side:  station=%d ", pline_station_door_side->door_side_array[ix].station_no));
//				debug_print(("door side = %d \n", pline_station_door_side->door_side_array[ix].door_side));
			}
			diag_printf("recv_len = %d,%d\n",recv_len,pline_station_door_side->total_bytes);
			if(  recv_len == pline_station_door_side->total_bytes+4 )
			{
				BcuUpdateDoorSideArray( pline_station_door_side );
			}
		}
		break;	

		case SUBEVENT_TYPE_STATION_BCU_VOLUME_SYNC: ///< tell BCU volume when pu
		{
			pu_volume_table_row_t  *pbcu_vol_table_row = (pu_volume_table_row_t  *)recv_buffer;
			debug_print(("Bcu,  pu-vol table:  recv_len=%d, len=%d \n", recv_len, sizeof(pu_volume_table_row_t)));
			
			if(  recv_len == sizeof(pu_volume_table_row_t)+4 )
			{
				BcuUpdateBcuVolArray( pbcu_vol_table_row );
			}
		}
		break;		
              
		default:
		break;
	  }
      return ret;
}
int BcuDisplayRecvCommNetPackage(common_big_package_t *pcomm_net_recv_cmd)
{
      int ret = -1;
      pa_common_information_t *pcomm_cmd_info = NULL;

      if( NULL== pcomm_net_recv_cmd )
      {
             return -1;
      }

      pcomm_cmd_info = (pa_common_information_t *)&(pcomm_net_recv_cmd->common_big_data_u.pa_common_cmd_info);
      debug_print(("BcuDisplayRecvCommNetPackage:  type=%d, seq=%d\n", pcomm_cmd_info->event_type_common, pcomm_cmd_info->sequence));	  
      debug_print(("BcuDisplayRecvCommNetPackage:  sub_evt=%d\n", pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask));	 

      debug_print(("BcuDisplayRecvCommNetPackage:  dest=%s-%d\n", pcomm_net_recv_cmd->dst_dev_name, pcomm_net_recv_cmd->dst_dev_number));
      debug_print(("BcuDisplayRecvCommNetPackage:  src=%s-%d\n", pcomm_net_recv_cmd->src_dev_name, pcomm_net_recv_cmd->src_dev_number));
      debug_print(("BcuDisplayRecvCommNetPackage:  pkg_type=%d\n", pcomm_net_recv_cmd->pkg_type));

      debug_print(("BcuDisplayRecvCommNetPackage:  context_len=%d\n", pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context_len));	  
      //debug_print(("BcuDisplayRecvCommNetPackage:  source_ip=%s\n", pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_source_ip));	
      debug_print(("BcuDisplayRecvCommNetPackage:  req=%d\n", pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req));
      debug_print(("BcuDisplayRecvCommNetPackage:  res=%d\n\n", pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res));	  
	  
     return ret;      
}
int BcuSendCommonNetResponse(int net_cmd_tx_buffer_id, common_big_package_t *pcomm_net_recv_cmd)
{
      int ret = -1;
      common_big_package_t  tmp_net_cmd = {"\0", 0};
      pa_common_information_t *pcomm_cmd_info = (pa_common_information_t *)&(tmp_net_cmd.common_big_data_u.pa_common_cmd_info);
      pa_common_information_t *precv_comm_cmd_info = NULL;	  

      if( NULL== pcomm_net_recv_cmd )
      {
             return -1;
      }

      precv_comm_cmd_info = (pa_common_information_t *)&(pcomm_net_recv_cmd->common_big_data_u.pa_common_cmd_info);
      if( precv_comm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req==1 &&
	  precv_comm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res==0 )
      {
               memcpy((void *)&tmp_net_cmd, (const void *)pcomm_net_recv_cmd, sizeof(common_big_package_t));
               strcpy(tmp_net_cmd.dst_dev_name, pcomm_net_recv_cmd->src_dev_name);
//               tmp_net_cmd.dst_dev_number = pcomm_net_recv_cmd->src_dev_number;
               tmp_net_cmd.dst_dev_number = 251;//组播地址--Qing-Chen-2015-4-7
               strcpy(tmp_net_cmd.src_dev_name, pcomm_net_recv_cmd->dst_dev_name);
				if( pcomm_net_recv_cmd->dst_dev_number == MUL_DST_NO||pcomm_net_recv_cmd->dst_dev_number == MUL_SYNC_DST_NO  )
					tmp_net_cmd.src_dev_number = local_dev_number;
				else
					tmp_net_cmd.src_dev_number = pcomm_net_recv_cmd->dst_dev_number;
               pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res = 1;
               
               debug_print_z(("BcuSendCommonNetResponse:  dest=%s-%d\n", tmp_net_cmd.dst_dev_name, tmp_net_cmd.dst_dev_number));
               debug_print_z(("BcuSendCommonNetResponse:  src=%s-%d\n", tmp_net_cmd.src_dev_name, tmp_net_cmd.src_dev_number));
               debug_print_z(("BcuSendCommonNetResponse:  type=%d, seq=%d\n", pcomm_cmd_info->event_type_common,pcomm_cmd_info->sequence));
               debug_print_z(("BcuSendCommonNetResponse:  sub_type=%d \n", tmp_net_cmd.common_big_data_u.pa_common_cmd_info.event_info_common.gd_sync_cmd_info.gd_sync_mask));
               
               ret = BlockBufferWrite(net_cmd_tx_buffer_id, (const void *)&tmp_net_cmd, sizeof(tmp_net_cmd));
      }
      else
      {
            debug_print(("BcuSendCommonNetResponse:  not request\n"));
      }
	  
     return ret;      
}
int BcuReadDeviceVersionTableItemInit(void)
{
	pthread_mutex_init(&mutex_dev_version_table, NULL);

	return 0;
}
int BcuSaveDeviceVersionTable(common_big_package_t *net_recv_cmd)
{
	return 0;
}
#if 1  ///< network timeout retry,2014
void *BcuGdThreadEntry(void *arg)
{ ///< 目前使用这里,2014-09-18注
	dev_config_table_item_t  bcu_config_info = {0}; ///< 上电后,保存设备号设备名称等参数
	common_big_package_t net_recv_cmd = {"\0",0}; ///< 获取CCU数据库网络包
	pa_common_information_t *pcomm_cmd_info = (pa_common_information_t *)&(net_recv_cmd.common_big_data_u.pa_common_cmd_info);

	unsigned char server_ccu_sync_cmd  = 0;
	int gd_net_cmd_tx_buffer_id = 0; ///< 缓冲区,发送数据库网络包
	int gd_net_cmd_rx_buffer_id = 0; ///< 缓冲区,接收数据库网络包
	int alarm_id = 0; ///< 本地定时器ID
	char *local_ip = NULL;
        unsigned char bcu_to_ccu_net_ok = 0; ///< <0>表示BCU-CCU之间网络不通; <1>通
        unsigned char ccu_enter_bep_phase = 0; //< <1>表示CCU进入"对BCU,EAMP,PCU进行db服务阶段"; <0>表示BCU暂时无法与CCU通信
        unsigned char bcu_update_finish = 0; ///< <1>表示BCU已经完成更新
        unsigned char detect_active_ccu_count = 0; ///< 统计所有CCU发来的响应包,例如为5,则可能CCU1发2次,CCU2发3次
	unsigned char ready_req_cmd  = 0; ///< <0>表示可以再次发READY包; <1>表示不再发READY包
        unsigned char ready_package_response_count = 0; ///< 统计READY包的响应次数
        unsigned char ready_package_request_timeouts = 0; ///< 统计READY包的超时次数
        unsigned char receive_ccu_table_item_flag = 0; ///< <0>表示还没有收到表格数据; <1>表示收到表格数据
        unsigned char table_pacakage_timeouts = 0; ///< 统计接收表格数据时的超时次数
        unsigned char current_phase = 0; ///< 当前阶段,详见下面for(;;)
//        unsigned char all_dev_version_req_cmd  = 0; ///< add, 1010
//	memset((void *)all_eamp_version_table, 0, sizeof(all_eamp_version_table));
//	memset((void *)all_pcu_version_table, 0, sizeof(all_pcu_version_table));
//	memset((void *)all_ccu_version_table, 0, sizeof(all_ccu_version_table));
//	memset((void *)all_bcu_version_table, 0, sizeof(all_bcu_version_table));
		
	gd_net_cmd_rx_buffer_id = BlockBufferOpen("bcu-gd-net-cmd-Rx"); ///< Init: create 8 <common_big_package_t>, 4KiB
	ZhwBufferRegister(2,COMMON_PACKAGE_TYPE_SYNC_GD, 1, gd_net_cmd_rx_buffer_id);

	gd_net_cmd_tx_buffer_id  = BlockBufferOpen("network-udp-common-big-send");

	BcuGdGetLocalDevIO((int *)&bcu_config_info.dev_type, (int *)&bcu_config_info.dev_no); ///< 获取BCU设备类型、设备号
	strcpy(bcu_config_info.dev_name, "BCU");
	strcpy(bcu_config_info.db_server_dev_name, "CCU");
	bcu_config_info.db_server_dev_no = 1; ///< default: CCU-1 is db server
	local_ip = zhw_network_get_local_ip(); ///<
	strcpy(bcu_config_info.dev_ip, local_ip);
	local_dev_number = bcu_config_info.dev_no; ///< add, 1010
	
	bcu_config_info.driver_key = BcuCheckDriverKey(bcu_config_info.dev_type, bcu_config_info.dev_no); //[1]: this BCU is active

	//memset((void *)&gd_dev_state_count_array, 0xFF, sizeof(gd_dev_state_count_array));

        alarm_id = AlarmCreate("buc-gd-sync-wait", 100, 100, BcuAlarmSyncWaiting); ///< 创建本地定时器,100表示1秒
        AlarmEnable(alarm_id); ///< 启动定时器,上电后,就开始发送探测包
        
	int loop_time = 0;

  ///< 本线程主循环体,只有2大部分: switch状态检测、接收解析命令
	for(;;)
	{
		if(loop_time++ >= 1000)
		{
			debug_print(("current_phase=%d, %d,%d,%d,update_start = %d\ncontrol_flag = %d-%d\n sample_flag=%d-%d\n thread_change_flag=%d-%d\n network_flag = %d-%d\n %d,%d,%d,%d,%d\n",
					current_phase,control_priority,network_priority,ts_priority,update_start,
					last_control_flag,control_flag,last_sample_flag ,sample_flag,
					last_thread_change_flag, thread_change_flag,last_network_flag,network_flag,
					snd_timer_flag,deamon_timer_flag,key_timer_flag,sync_timer_flag,network_count));
			loop_time = 0;			
		}

		if(0) ///< (1==AlarmCheckTimeOut(alarm_id) ) ///< debug
		{
                      BCUSetDataFromCCUState(2);
		}

    ///< 进入线程循环体后,使用switch维护状态判断
		switch( current_phase )
		{
                       case 0: ///< pu,即上电后第一个状态
                       {
                              if( 1==ccu_enter_bep_phase ) ///< 远程CCU是否进入"与BCU,EAMP,PCU进行db更新阶段"
                              { ///< ccu_enter_bep_phase变量修改见下面(即收到CCU的网络包)
                                    current_phase = 1; ///< 设定下一次状态
                                    AlarmEnable(alarm_id); ///< 启动定时器,用于多次发送SYNC_BEP包,直到收到相应,关闭定时器
                                    server_ccu_sync_cmd = 0; ///< 为<1>表示收到CCU相应
                                    bcu_to_ccu_net_ok = 0; ///< 该变量为0,表示BCU-CCU之间网络不通
                                    ///< 清空,将记录某个CCU响应次数,db_ccu_status[1]表示CCU-1,db_ccu_status[2]表示CCU-2
                                    memset((void *)bcu_config_info.db_ccu_status, 0, sizeof(bcu_config_info.db_ccu_status));
				    debug_print(("Bcu Sync thread:   next state=1*** \n"));
			      }
		       }
		       break;

                       case 1: ///< 本阶段统计CCU响应次数,谁的次数多,则以谁为主CCU
                       {
				if( 1 || 2== BCUGetDataFromCCUState() ) ///< 检测触摸屏是否就绪,否则一直等待
				{
					     ///< 由于bcu_to_ccu_net_ok =0,因此一直停留在该阶段,此时我们一直不断发包,然后接收响应
				       if( 1==bcu_to_ccu_net_ok && 1==ccu_enter_bep_phase ) ///< BCU-CCU网络良好,并且CCU进入BEP阶段
				       {
				       	  ///< 至此,BCU已经收到CCU多次响应,现在检查哪个CCU回复次数最多
						if( bcu_config_info.db_ccu_status[1].flag == 1 && bcu_config_info.db_ccu_status[2].flag == 2 )
						{
		                                       if( bcu_config_info.db_ccu_status[1].connect_count < bcu_config_info.db_ccu_status[2].connect_count  )
		                                       {
                                                             bcu_config_info.db_server_dev_no = 2;
						       }
						}
						else if( bcu_config_info.db_ccu_status[1].flag == 2 && bcu_config_info.db_ccu_status[2].flag == 1 )
						{
		                                       if( bcu_config_info.db_ccu_status[1].connect_count > bcu_config_info.db_ccu_status[2].connect_count  )
		                                       {
                                                             bcu_config_info.db_server_dev_no = 1;
						       }
						}
						else
						{
						     debug_print(("Bcu Sync thread:  maybe CCU offline \n"));
						     if( bcu_config_info.db_ccu_status[1].connect_count < bcu_config_info.db_ccu_status[2].connect_count  )
		                                       {
                                                             bcu_config_info.db_server_dev_no = 2;
						       }
		                                       else if( bcu_config_info.db_ccu_status[1].connect_count > bcu_config_info.db_ccu_status[2].connect_count  )
		                                       {
                                                             bcu_config_info.db_server_dev_no = 1;
						       }
						}

///< 本段: 虽然统计CCU响应次数,但为了加快速度,CCU1-BCU1、CCU2-BCU2进行配对
///< 如果改成#if 0,则以上面统计次数为准
#if 1 ///< def CONFIG_BCU1_CCU1
{
     if( bcu_config_info.dev_no==1 ) ///< we are BCU-1
                       {
                             if(bcu_config_info.db_ccu_status[1].connect_count > 0 ) ///< 只要收到CCU1的一个响应,就以CCU1为主
                             {
                                            bcu_config_info.db_server_dev_no = 1;
	     }
                             else if(bcu_config_info.db_ccu_status[2].connect_count > 0 ) ///< 如果CCU1没有任何响应,只能看CCU2
                             {
                                            bcu_config_info.db_server_dev_no = 2;
	     }
       }
     if( bcu_config_info.dev_no==2 ) ///< we are BCU-2
                       {
                             if(bcu_config_info.db_ccu_status[2].connect_count > 0 ) ///< 只要收到CCU2的一个响应,就以CCU2为主
                             {
                                            bcu_config_info.db_server_dev_no = 2;
	     }
                             else if(bcu_config_info.db_ccu_status[1].connect_count > 0 )///< 如果CCU2没有任何响应,只能看CCU1
                             {
                                            bcu_config_info.db_server_dev_no = 1;
	     }
       }
}
#endif

						debug_print(("Bcu Sync thread,<case=1>:   send READY request, dest_no=%d \n", bcu_config_info.db_server_dev_no));
						     debug_print(("Bcu Sync thread,1710:  CCU-1:  %d,%d \n",
							 	bcu_config_info.db_ccu_status[1].flag,
                                                                bcu_config_info.db_ccu_status[1].connect_count));	
						     debug_print(("Bcu Sync thread,1710:  CCU-2:  %d,%d \n",
							 	bcu_config_info.db_ccu_status[2].flag,
                                                                bcu_config_info.db_ccu_status[2].connect_count));
            ///< 至此,确定了哪个CCU为主,下面向该CCU发起READY_SYNC请求包(点对点)
						if( BcuSendDbSyncReqCommon(gd_net_cmd_tx_buffer_id, &bcu_config_info, SUBEVENT_TYPE_READY_SYNC_REQ, 
							bcu_config_info.db_server_dev_no) > 0 )
						{
                                                     AlarmDisable(alarm_id);						
                                                     AlarmEnable(alarm_id); ///< 启动定时器,准备多次发送READY_SYNC请求包
						     current_phase = 2; ///< 下一次状态
                                                     ready_package_request_timeouts = 0; ///< 发送READY_SYNC请求包,超时次数
						     ready_package_response_count = 0;
						     ready_req_cmd = 0;
						     debug_print(("Bcu Sync thread:   next state=2*** \n"));
						}
				       }
				}
		       }
		       break;

                       case 2: ///< 本阶段多次发送READY_SYNC_REQ包,用于CCU判断对哪个BCU发送表格
                       {
			      if( ready_package_request_timeouts >= 10 ) ///< 连续发送10次READY_SYNC_REQ包,没有任何响应
			      {
                                     debug_print(("Bcu Sync thread:  case=2, timeout>=10,back to case=0 \n"));
				     ///< reset something,复位变量,重新回到上电状态,即case 0
                                    AlarmDisable(alarm_id);
                                    bcu_to_ccu_net_ok = 0;
                                    ccu_enter_bep_phase = 0;
                                    bcu_update_finish = 0;
                                    detect_active_ccu_count = 0;
                                    ready_package_response_count = 0;
                                    ready_package_request_timeouts = 0;
                                    receive_ccu_table_item_flag = 0;
                                    table_pacakage_timeouts = 0;
                                    server_ccu_sync_cmd = 0;
                                    ready_req_cmd  = 0;
                                    current_phase = 0;
                                     memset((void *)bcu_config_info.db_ccu_status, 0, sizeof(bcu_config_info.db_ccu_status));
				     AlarmEnable(alarm_id);				 
			      }
				  
			      if( ready_package_response_count >= 3 ) ///< 收到3次READY_SYNC_REQ包响应
			      {
                                    AlarmDisable(alarm_id);
				    current_phase =3; ///< 下一次状态(接收表格)
                                    AlarmEnable(alarm_id); ///< 启动定时器,用于检测是否按时收到表格
                                    table_pacakage_timeouts = 0;
                                        update_start = 1;
                                    debug_print(("Bcu Sync thread,02:   next state=3*** \n"));
			      }
		       }
		       break;	

                       case 3: ///< 本阶段: 接收数据表格,每次都是表格的一行
                       {
			      if( table_pacakage_timeouts >= 10 )
			      {
                                     debug_print(("Bcu Sync thread:  case=3, timeout>=10, back to case=0 \n"));
				     ///< reset something
                                    AlarmDisable(alarm_id);
                                    bcu_to_ccu_net_ok = 0;
                                    ccu_enter_bep_phase = 0;
                                    bcu_update_finish = 0;
                                    detect_active_ccu_count = 0;
                                    ready_package_response_count = 0;
                                    ready_package_request_timeouts = 0;
                                    receive_ccu_table_item_flag = 0;
                                    table_pacakage_timeouts = 0;
                                    server_ccu_sync_cmd = 0;
                                    ready_req_cmd  = 0;
                                    current_phase = 0;
                                     memset((void *)bcu_config_info.db_ccu_status, 0, sizeof(bcu_config_info.db_ccu_status));
				     AlarmEnable(alarm_id);
			      }
				  
                              if( 1==AlarmCheckTimeOut(alarm_id)  && 0==receive_ccu_table_item_flag )
                              { ///< 规定时间内,没有收到一行表格数据
                                    AlarmDisable(alarm_id);
				    AlarmEnable(alarm_id);
                                    table_pacakage_timeouts++; ///< 超时次数加1
			      }
                              else if( 0==AlarmCheckTimeOut(alarm_id) && 1==receive_ccu_table_item_flag )
                              { ///< 规定时间内收到数据,复位
                                    AlarmDisable(alarm_id);
				    AlarmEnable(alarm_id);
                                    receive_ccu_table_item_flag = 0; ///< reset
			      }
                              else if( 1==AlarmCheckTimeOut(alarm_id) && 1==receive_ccu_table_item_flag )
                              { ///< 尽管超时,但还是收到数据了,也复位receive标志变量
                                    AlarmDisable(alarm_id);
				    AlarmEnable(alarm_id);
                                    receive_ccu_table_item_flag = 0; ///< reset
			      }
			      else
			      {
                                    //debug_print(("***Bcu Sync thread:  case=3, unsupported event*** \n"));
			      }
		       }
		       break;	

		       case 4: ///< 当收到END包时,进入本阶段
			{
                              AlarmDisable(alarm_id); ///< 关闭定时器
                              current_phase = 5; ///< 进入阶段5(空闲状态),在阶段时,依然接收解析命令包,但不会有状态变化了
                              update_start = 0;
			}
			break;

                       default:  ///< 
		       break;			   
		}

    ///< 接收、解析命令部分
		if(BlockBufferRead(gd_net_cmd_rx_buffer_id, &net_recv_cmd,  sizeof(common_big_package_t)) > 0)
		{
                        //BcuDisplayRecvCommNetPackage(&net_recv_cmd); ///< 打印接收的命令,用于调试
			if( pcomm_cmd_info->event_type_common == EVENT_TYPE_SYS_UPDATE_SYNC ) ///< 类型必须为SYS_UPDATE_SYNC,否则不处理
			{
				if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask==SUBEVENT_TYPE_DB_SERVER_CHANGED )
				{ ///< 本段目前未用
					db_server_update_t *tmptr = (db_server_update_t *)(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context);				
				       #if 0
					strcpy(bcu_config_info.db_server_dev_name, tmptr->server_dev_name);
					strcpy(bcu_config_info.db_server_ip, tmptr->server_dev_ip);
					bcu_config_info.db_server_dev_no = tmptr->server_dev_no;
					debug_print(("Bcu Sync thread:  db server changed: new=%s_%d \n", bcu_config_info.db_server_dev_name, bcu_config_info.db_server_dev_no));
                                        BcuSendCommonNetResponse( gd_net_cmd_tx_buffer_id,  &net_recv_cmd);
					#else
					debug_print(("Bcu Sync thread:  db server changed: new dev=%s_%d, ", tmptr->server_dev_name, tmptr->server_dev_no));
					debug_print(("ip=%s \n", tmptr->server_dev_ip));
					#endif
				}
				else if(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask == SUBEVENT_TYPE_DB_VERSION_SYNC_BEP )
				{ ///< 子类型为BEP,即对BCU\EAMP\PCU进行db更新
					   ///< gd_sync_context是数组,格式依次为:
					   ///< [config_db版本字符串]
					   ///< [ann_db版本字符串]
					   ///< [固定标识]: 固定为"SERVER",长度按照7字节计算
					   ///< [服务器设备名称]
					   ///< [服务器设备号]
					   ///< [主或次服务器标志]: <1>主服务器; <2>次服务器
					   ///< [校验码]: 4字节,但目前未用,保留
				     char *tmptr = (char *)(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_context);
				     int *tmp_dev_no = NULL;
					 
				     debug_print(("Bcu Sync thread:  receive BEP sync package,  %d, %d\n", 
					 	pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req,
					 	pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res));
				     //if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req == 1 &&
                                     //     pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res == 0 )
				     if( 0 ) ///< 注: 这里不执行
				     {
                                           AlarmDisable(alarm_id);
					   tmptr += CONFIG_CCU_VERSION_STRING_LEN; ///< config db string
					   tmptr += CONFIG_CCU_VERSION_STRING_LEN; ///< ann db string
					   if( 0==strcmp(tmptr, "SERVER") )
					   {
					           tmptr += 7;
                                                   //strcpy(bcu_config_info.db_server_dev_name, tmptr);
                                                   tmptr += DEVICE_NAME_LENGTH;	
						   tmp_dev_no = (int *)tmptr;
						   //bcu_config_info.db_server_dev_no = (*tmp_dev_no);
						   tmp_dev_no = (int *)(tmptr+4);
						   if( net_recv_cmd.src_dev_number >= 0 && net_recv_cmd.src_dev_number < 4 )
						   {
                                                        bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].flag = (*tmp_dev_no); ///< flag of ip table for ccu
                                                        bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].connect_count++;
						   }
					   }
                                            server_ccu_sync_cmd = 1;
				            ccu_enter_bep_phase = 1;
				     }
				     else if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req == 1 &&
                                                 pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res == 1 )
				     { ///< 处理响应包
  		                            debug_print(("Bcu Sync thread, receive BEP res:  seq=%d_%d, size=%d \n",
  		                        		                        send_package_serial_number,
  									   	 pcomm_cmd_info->sequence,
  									   	 sizeof(common_big_package_t) ));
	                                     if( 0==AlarmCheckTimeOut(alarm_id) && send_package_serial_number==pcomm_cmd_info->sequence)
	                                     { ///< 规定时间内,收到准确的响应包(包号相同)
	                                            AlarmDisable(alarm_id);
						    tmptr += CONFIG_CCU_VERSION_STRING_LEN; ///< config db string
						    tmptr += CONFIG_CCU_VERSION_STRING_LEN; ///< ann db string
                                                    if( 0==strcmp(tmptr, "SERVER") ) ///< 检测固定标识
	                                            {
	                                                      debug_print(("Bcu Sync thread, receive Res:  current db server=%s_%d \n", bcu_config_info.db_server_dev_name, 
	                                                                                bcu_config_info.db_server_dev_no));
	                                                      debug_print(("Bcu Sync thread, receive Res:  src_dev=%s_%d \n", net_recv_cmd.src_dev_name, 
	                                                                                net_recv_cmd.src_dev_number));
	                                                      tmptr += 7;
	                                                      //strcpy(bcu_config_info.db_server_dev_name, tmptr);
	                                                      tmptr += DEVICE_NAME_LENGTH;
	                                                      tmp_dev_no = (int *)tmptr;
	                                                      //bcu_config_info.db_server_dev_no = (*tmp_dev_no);
							      tmptr += 4; ///< ip flag: <1>: primary CCU; <2>: backup CCU
							      if( net_recv_cmd.src_dev_number >= 0 && net_recv_cmd.src_dev_number < 4 )
							      {
		                                                    bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].flag = (unsigned char)(*((int *)tmptr)); ///< 记录CCU服务器主次标志
		                                                    bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].connect_count++; ///< 统计CCU响应次数
                                   debug_print(("Bcu Sync thread, db_ccu_status:  CCU-%d:  %d_%d \n",
                                               net_recv_cmd.src_dev_number,
										           bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].flag,
                                              bcu_config_info.db_ccu_status[net_recv_cmd.src_dev_number].connect_count));
							      }
	                                  debug_print(("Bcu Sync thread, receive Res:  new db server=%s_%d \n", bcu_config_info.db_server_dev_name,
	                                                                                        bcu_config_info.db_server_dev_no));
	                                            }
				                    ccu_enter_bep_phase = 1; ///< CCU进入bep阶段
	                                            server_ccu_sync_cmd = 1; ///< 收到CCU命令
						    if( 1==current_phase ) ///< 如果是阶段1,即BCU持续发包,探测BCU-CCU之间的网络连接状况
						    {
                                                           if( detect_active_ccu_count >= 5 )
                                                           {
                                                                 bcu_to_ccu_net_ok = 1; ///< 连续5次收到响应,表明网络良好
							   }
							  else
							  {
							  	   ///< 如果未收到5次响应包,则继续发包
                                                                AlarmEnable(alarm_id);
                                                                detect_active_ccu_count++; ///< 超时次数加1
                                                                server_ccu_sync_cmd = 0;
							  }
						   }
	                                     }
				     }
				}
				else if(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask >= SUBEVENT_TYPE_LINE_STATION_TABLE_SYNC &&
				           pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask <= SUBEVENT_TYPE_STATION_BCU_TS_PASSWORD )
				{
					///< 本段主要是处理表格数据,每个表格对应一个子类型
					//debug_print(("Bcu Sync thread:  sync package--55\n"));
					BcuSendCommonNetResponse(gd_net_cmd_tx_buffer_id, &net_recv_cmd); ///< 收到一行,回发一个响应包(CCU检测该包,决定是否重新发该行数据)
					if( 0==bcu_update_finish ) ///< (bcu_state.this_bcu_is_active != 0)
					{
						BcuGdGetDbFromServerCcu( &net_recv_cmd ); ///< 更新数据表格,BCU保存在对应数组中--update
					}
                                        receive_ccu_table_item_flag = 1; ///< 0变1,表示收到数据,该变量在上面switch段中复位
				}
				else if(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask >= SUBEVENT_TYPE_PCU_UPDATED &&
				           pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask <= SUBEVENT_TYPE_EAMP_UPDATING )
				{
					 ///< 本段暂时未用,保留
				       debug_print(("Bcu Sync thread:  eamp/pcu, tftp update status \n"));
                                       BcuGdUpdateDevTftpStatus( &net_recv_cmd );
				}
				else if(pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask == SUBEVENT_TYPE_READY_SYNC_REQ )
				{
					    ///< 处理READY_SYNC包
				       debug_print(("Bcu Sync thread:  READY package: req=%d,res=%d,seq=%d_%d, %d \n", 
					   	pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req,
					   	pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res,
					   	send_package_serial_number,
					   	pcomm_cmd_info->sequence,
					   	ready_package_response_count));
				       if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req == 1 &&
                                            pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res == 1 )
				       { ///< 只处理响应包
                                                if( 0==AlarmCheckTimeOut(alarm_id) && send_package_serial_number==pcomm_cmd_info->sequence)
                                                { ///< 规定时间内,收到准确的响应包(包号相同)
                                                       AlarmDisable(alarm_id);
                                                       ready_req_cmd = 1;
						       if( 2==current_phase ) ///< 如果是阶段2
						       {
                                                              ready_package_response_count++; ///< 统计一次
                                                              AlarmEnable(alarm_id);
                                                              ready_req_cmd = 0; ///< 开定时器,再发一包
						       }
					        }
                                                else if( 1==AlarmCheckTimeOut(alarm_id)  && 0==ready_package_response_count )
                                                { ///< 超时了,重新发
                                                      AlarmDisable(alarm_id);
						       if( 2==current_phase )
						       {
                                                              ready_package_request_timeouts++; ///< 超时次数加1
                                                              AlarmEnable(alarm_id);
                                                              ready_req_cmd = 0;
						       }													  
                                                }											
				       }
				}				
				else if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask == SUBEVENT_TYPE_END_SYNC )
				{
					///< 收到CCU发来的END包,BCU结束更新
					debug_print(("Bcu Sync thread:  sync end package--77\n"));
					BcuSendCommonNetResponse(gd_net_cmd_tx_buffer_id, &net_recv_cmd); ///< just response End_sync package
					cyg_thread_delay(100);
					BCUSetDataFromCCUState(1); ///< 通知触摸屏,更新结束
					bcu_update_finish = 1; ///< finish update, don't again
                                        current_phase = 4;
				}
				else if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_mask == SUBEVENT_TYPE_ALL_DEV_VERSION ) ///< add, 1010
				{
                                  if( pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_req == 1 && 
				           pcomm_cmd_info->event_info_common.gd_sync_cmd_info.gd_sync_res == 0 )
                                  {						 
                                        if ( 0==BcuSaveDeviceVersionTable( (common_big_package_t *)&net_recv_cmd ) ) ///< saved
                                        {
                                             BcuSendCommonNetResponse(gd_net_cmd_tx_buffer_id, &net_recv_cmd);
					     }
				      }
				}
			}
		}
                
		if( !server_ccu_sync_cmd && 1==AlarmCheckTimeOut(alarm_id))
		{
			///< 本段定时,多次发送SYNC_BEP包,阶段0、1都是发该包,然后统计次数
			///< server_ccu_sync_cmd=0有效
			debug_print(("Bcu Sync thread:  try to request BEP \n"));
			AlarmDisable(alarm_id);
			AlarmEnable(alarm_id); ///< restart this timer
			BcuSendDbSyncReqCommon(gd_net_cmd_tx_buffer_id, &bcu_config_info, SUBEVENT_TYPE_DB_VERSION_SYNC_BEP, 
				                                            MUL_SYNC_DST_NO);
		}

		if( !ready_req_cmd && 1==AlarmCheckTimeOut(alarm_id))
		{
			///< 本段定时,多次发送READY_SYNC包,阶段2发送该包,然后统计次数
			///< ready_req_cmd=0有效
			debug_print(("Bcu Sync thread:  try to request READY \n"));
			AlarmDisable(alarm_id);
			AlarmEnable(alarm_id); ///< restart this timer
			BcuSendDbSyncReqCommon(gd_net_cmd_tx_buffer_id, &bcu_config_info, SUBEVENT_TYPE_READY_SYNC_REQ, 
                                                                            bcu_config_info.db_server_dev_no);	
		}	

	        sched_yield(); ///< 主动让出CPU使用权
	}
	
	return NULL;
}
#endif

