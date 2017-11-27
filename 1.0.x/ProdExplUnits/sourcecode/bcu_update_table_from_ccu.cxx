/*
 * Copyright (c) 2013, MIT, All right reserved.
 *
 * File Name:	bcu_update_table_from_ccu.c
 * File Identification: 
 * Abstract:  
 *
 * Current Version:	V2013_0415_0.0.1
 * Author:	 H.Product Department
 * Completion Date:	2013-07-12
 *
 * Last Version:
 * Last Author:
 * Finishing Date:
 */
#include "bcu_update_table_from_ccu.h"

unsigned char whether_have_finished_get_data_from_ccu = 0;
unsigned char whether_bcu_have_get_part_info_from_ccu = 0;//当前BCU 是否获取部分CCU 数据库信息


unsigned char BCUGetSyncState(void)
{
	return (whether_have_finished_get_data_from_ccu == 1)? 1 : 0;
}

unsigned char BCUGetDataFromCCUState(void)
{///<BCU 获取CCU数据库状态
	return 3;//whether_have_finished_get_data_from_ccu;
}

void BCUSetDataFromCCUState(int value)
{
	whether_have_finished_get_data_from_ccu = value;

	return ;
}
 
#ifdef CONFIG_BCU_SYNC_FROM_CCU_NET

unsigned char whether_begin_updata_line3_region_info = 0;///<add by wilson 2013-07-12
unsigned char whether_begin_updata_line4_region_info = 0;///<add by wilson 2013-07-12

int BcuUpdateLogPathArray( log_path_table_row_t *plog_path_table_row )
{
	strcpy((char *)bcu_state.log_file_path,(char *)plog_path_table_row->current_row_info.path_name);
	return 0;
}
int BcuUpdateDoorSideArray( line_station_door_side_t *pline_station_door_side )
{
        int ret = -1;
		
	if( NULL != pline_station_door_side)
	{
               int tmp_len = 0;
	       int i = 0;

	       debug_print(("update door side:  %d, %d, %d, %d\n", pline_station_door_side->line_no, pline_station_door_side->section_no, 
		  	              pline_station_door_side->total_bytes, pline_station_door_side->array_items));
	      tmp_len = sizeof(line_station_door_side_t) + (pline_station_door_side->array_items)*(sizeof(line_station_door_side_array_t));
	      if( pline_station_door_side->total_bytes != tmp_len )
	      {
	           debug_print(("update door side, package wrong:  %d\n", tmp_len));
	           return -1;
	      }

           #if 1
		      for(i=0; i<pline_station_door_side->array_items; i++)
		      {
	              if(pline_station_door_side->door_side_array[i].station_no > 0 &&
	            		  pline_station_door_side->door_side_array[i].station_no <= MAX_STATION_NUMBER)
	              {
		    	  bcu_state.pp_station_info[pline_station_door_side->line_no-3][pline_station_door_side->door_side_array[i].station_no].open_door_side[0] =
	                		 pline_station_door_side->door_side_array[i].door_side[0];
	               bcu_state.pp_station_info[pline_station_door_side->line_no-3][pline_station_door_side->door_side_array[i].station_no].open_door_side[1] =
	                		 pline_station_door_side->door_side_array[i].door_side[1];
	              }
		      }
	      ret = 0;
	      #endif		  
	}
	return ret;
}
int BcuUpdateRecordPathArray( record_path_table_row_t *record_path_table_row_t )
{
	strcpy((char *)bcu_state.record_data_path,(char *)record_path_table_row_t->current_row_info.path_name);
	return 0;
}
int BcuUpdateDevNumberArray( device_number_table_row_t *pdev_number_table_row )
{
	return 0;
}
int BcuUpdateConfigVersionArray( config_db_version_table_row_t *pconfig_db_version_table_row )
{
#if 0
   strncpy((char *)bcu_state.device_db_version.bcu_db_version[bcu_state.bcu_info.devices_no-1][1],
		   (char *)pconfig_db_version_table_row->current_row_info.current_version,10);

   strncpy((char *)bcu_state.device_db_version.ccu_db_version[bcu_state.bcu_info.devices_no-1][1],
		   (char *)pconfig_db_version_table_row->current_row_info.current_version,10);
#endif
	return 0;
}
int BcuUpdateAnnVersionArray( ann_db_version_table_row_t *pann_db_version_table_row )
{
#if 0
   strncpy((char *)bcu_state.device_db_version.bcu_db_version[bcu_state.bcu_info.devices_no-1][0],
		   (char *)pann_db_version_table_row->current_row_info.current_version,10);

   strncpy((char *)bcu_state.device_db_version.ccu_db_version[bcu_state.bcu_info.devices_no-1][0],
		   (char *)pann_db_version_table_row->current_row_info.current_version,10);
#endif
	return 0;
}
int BcuUpdateNetIpArray( ip_mac_table_row_t *pip_mac_table_row )
{
	ZhwUpdateBCUIpArray(pip_mac_table_row);
	return 0;
}
int BcuUpdateLcdEmergAnnArray( emerg_ann_table_row_t *pemerg_ann_table_row )
{}
int BcuUpdateLcdDefaultLineArray( default_line_table_row_t  *pdefault_line_table_row )
{///<BCU 获取默认线路号
	if(pdefault_line_table_row->current_row_info.default_line == 1)
	{
		bcu_state.this_line_no = pdefault_line_table_row->current_row_info.line_no;
		if(whether_bcu_have_get_part_info_from_ccu == 0)
		{
			whether_bcu_have_get_part_info_from_ccu   = 1;
			last_control_flag = control_flag;control_flag = 318;
			AlarmTSToChangeScreen(38);
			last_control_flag = control_flag;control_flag = 319;
		}
		diag_printf("default line no is %d\n",bcu_state.this_line_no);
	}
	return 0;
}
int BcuUpdateLcdLineStationArray( line_station_code_row_t  *pcurrent_station_row )
{///<BCU 获取线路区间站台信息
	if(pcurrent_station_row->current_index < MAX_STATION_NUMBER)
	{
		//bcu_state.pp_station_info[pcurrent_station_row->line_no - 3][pcurrent_station_row->current_row_info.station_no].cursor = 0;
		if(pcurrent_station_row->current_row_info.fix_across_station == 1)
		{
			bcu_state.pp_station_info[pcurrent_station_row->line_no - 3][pcurrent_station_row->current_row_info.station_no].effective= 0;
		}
		else
		{
			bcu_state.pp_station_info[pcurrent_station_row->line_no - 3][pcurrent_station_row->current_row_info.station_no].effective= 1;
		}

		strcpy((char *)bcu_state.pp_station_info[pcurrent_station_row->line_no - 3][pcurrent_station_row->current_row_info.station_no].station_chinese_name,
				(char *)pcurrent_station_row->current_row_info.station_name);

//		bcu_state.pp_station_info[pcurrent_station_row->line_no - 3][pcurrent_station_row->current_row_info.station_no].open_door_side =
//				pcurrent_station_row->current_row_info.spec_open_door;
	}
	bcu_state.bcu_region_info[pcurrent_station_row->line_no - 3].total_stations_number = pcurrent_station_row->total_row;
	return 0;
}
int BcuUpdateLcdLineRegionArray(line_section_row_t *pcurrent_section_row)
{///<BCU 获取线路区间信息
	unsigned char max_region_num = 0;

	if(pcurrent_section_row->line_no == 3 && whether_begin_updata_line3_region_info == 0)
	{
		whether_begin_updata_line3_region_info = 1;
		bcu_state.bcu_region_info[0].total_regions_number = pcurrent_section_row->total_row;
	}

	if(pcurrent_section_row->line_no == 4 && whether_begin_updata_line4_region_info == 0)
	{
		whether_begin_updata_line4_region_info = 1;
		bcu_state.bcu_region_info[1].total_regions_number = pcurrent_section_row->total_row;
	}

	if(pcurrent_section_row->line_no == 3)
	{
		max_region_num = LINE3_DEFULT_REGIONS_NUMBER;
	}
	else if(pcurrent_section_row->line_no == 4)
	{
		max_region_num = LINE4_DEFULT_REGIONS_NUMBER;
	}

	if(pcurrent_section_row->current_index < max_region_num )
	{

		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->line_no =
				pcurrent_section_row->current_row_info.line_no;
		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->region_no =
				pcurrent_section_row->current_row_info.section_no;
		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->start_station_no =
				pcurrent_section_row->current_row_info.start_station;
		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->over_station_no =
				pcurrent_section_row->current_row_info.end_station;
		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->running_direction =
				pcurrent_section_row->current_row_info.line_direction+1;
		if((bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->running_direction == 1)
		{
			(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->running_direction = 2;
		}
		else
		{
			(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->running_direction = 1;
		}
#if 0
		if(pcurrent_section_row->line_no == 4)
		{
			if(pcurrent_section_row->current_row_info.start_station == pcurrent_section_row->current_row_info.end_station)
			{
				(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info+pcurrent_section_row->current_index - 1)->whether_the_cycle_bus = 1;
			}
			else
			{
				(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->whether_the_cycle_bus = 0;
			}
		}
		else
		{
			(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->whether_the_cycle_bus = 0;
		}
#else
		(bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].p_regin_info + pcurrent_section_row->current_index - 1)->whether_the_cycle_bus =
				pcurrent_section_row->current_row_info.spec_cycle;
#endif
		if(pcurrent_section_row->current_row_info.default_section == 1)
		{
			bcu_state.bcu_region_info[pcurrent_section_row->line_no - 3].default_region_no =
					pcurrent_section_row->current_row_info.section_no;

		}
	}
	return 0;
}
int BcuUpdatePriorArray( priority_table_row_t *pprior_table_row )
{///<BCU 获取状态优先级
	int ret = -1;
	event_priority_table_t db_priority_table_item;

	if( NULL != pprior_table_row )
	{
		strcpy(db_priority_table_item.event_name, pprior_table_row->current_row_info.event_type);
		db_priority_table_item.event_priority = pprior_table_row->current_row_info.priority_id;
		debug_print(("BcuUpdatePriorArray: %s-%d\n", db_priority_table_item.event_name, db_priority_table_item.event_priority));
		if ( 0 == BcuAnnStateCreatePriorityNode( &db_priority_table_item ) )
		{
			ret = 0;
		}
	}

	return ret;
}
int BcuUpdateBcuVolArray( pu_volume_table_row_t *pbcu_vol_table_row ) ///< add, 20140806
{///<BCU获取默认音量信息
      pu_volume_table_item_t  *pcurrent_row_info = NULL;
      int lcd_d2d_volume = 0;
      int lcd_d2p_volume = 0;
      int lcd_d2eamp_volume = 0;
      int lcd_d2listen_volume = 0;	  
	  
      if( NULL==pbcu_vol_table_row )
      {
            return -1;
      }

       pcurrent_row_info = (pu_volume_table_item_t *)&(pbcu_vol_table_row->current_row_info);
       debug_print(("BcuUpdatePriorArray: dd=%d, dp=%d, de=%d, dl=%d \n", 
	   	            pcurrent_row_info->d2d_volume, pcurrent_row_info->d2p_volume,
	   	            pcurrent_row_info->d2eamp_volume, pcurrent_row_info->d2listen_volume));
       diag_printf("BcuUpdatePriorArray: dd=%d, dp=%d, de=%d, dl=%d \n",
	   	            pcurrent_row_info->d2d_volume, pcurrent_row_info->d2p_volume,
	   	            pcurrent_row_info->d2eamp_volume, pcurrent_row_info->d2listen_volume);
       bcu_state.device_volume.d2d_volume  = pcurrent_row_info->d2d_volume;
       bcu_state.device_volume.intercomm_volume = pcurrent_row_info->d2p_volume;
       bcu_state.device_volume.car_volume = pcurrent_row_info->d2eamp_volume;
       bcu_state.device_volume.monitor_volume = pcurrent_row_info->d2listen_volume;
	   
       return 0;
}
#endif

